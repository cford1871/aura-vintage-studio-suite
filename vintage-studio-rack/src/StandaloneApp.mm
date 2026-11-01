#import <Cocoa/Cocoa.h>
#import <AudioToolbox/AudioToolbox.h>
#include "../include/VSR9512X_DSP.h"
#include "../include/VSR9512X_UI.h"

@interface VSRStandaloneAppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate> {
    NSWindow* _window;
    VSR9512XHardwareView* _hwView;
    vsr::VSR9512XEngine _engine;
    AudioComponentInstance _outputUnit;
    std::vector<float> _zeroL;
    std::vector<float> _zeroR;
}
@end

static OSStatus StandaloneRenderProc(void* inRefCon,
                                     AudioUnitRenderActionFlags* ioActionFlags,
                                     const AudioTimeStamp* inTimeStamp,
                                     UInt32 inBusNumber,
                                     UInt32 inNumberFrames,
                                     AudioBufferList* ioData) {
    auto* eng = static_cast<vsr::VSR9512XEngine*>(inRefCon);
    if (!ioData || ioData->mNumberBuffers == 0) return noErr;
    float* outL = static_cast<float*>(ioData->mBuffers[0].mData);
    float* outR = (ioData->mNumberBuffers > 1 && ioData->mBuffers[1].mData)
                  ? static_cast<float*>(ioData->mBuffers[1].mData)
                  : nullptr;
    std::memset(outL, 0, inNumberFrames * sizeof(float));
    if (outR) std::memset(outR, 0, inNumberFrames * sizeof(float));
    eng->processBlock(outL, outR ? outR : outL, outL, outR, static_cast<int>(inNumberFrames));
    return noErr;
}

@implementation VSRStandaloneAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification*)notification {
    _engine.prepare(48000.0);

    NSRect frame = NSMakeRect(120, 120, 980, 624);
    NSUInteger style = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable;
    _window = [[NSWindow alloc] initWithContentRect:frame
                                          styleMask:style
                                            backing:NSBackingStoreBuffered
                                              defer:NO];
    [_window setTitle:@"VSR-9512X // Vintage Sampler & Studio Mastering Effects Processor (VST3 / AU)"];
    [_window setDelegate:self];
    [_window setBackgroundColor:[NSColor colorWithCalibratedRed:0.06 green:0.07 blue:0.08 alpha:1.0]];

    _hwView = [[VSR9512XHardwareView alloc] initWithFrame:NSMakeRect(0, 0, 980, 624) engine:&_engine];
    [_window setContentView:_hwView];
    [_window center];
    [_window makeKeyAndOrderFront:nil];
    [_window makeFirstResponder:_hwView];

    [self startCoreAudio];

    // Support "--skin <0..4>", "--preset <0..7>", and "--export-preview <path.png>"
    NSArray<NSString*>* args = [[NSProcessInfo processInfo] arguments];
    for (NSUInteger i = 1; i + 1 < [args count]; ++i) {
        if ([args[i] isEqualToString:@"--skin"]) {
            _engine.setSkin([args[i + 1] intValue]);
        } else if ([args[i] isEqualToString:@"--preset"]) {
            _engine.loadPreset([args[i + 1] intValue]);
        }
    }
    for (NSUInteger i = 1; i + 1 < [args count]; ++i) {
        if ([args[i] isEqualToString:@"--export-preview"]) {
            NSString* outPath = args[i + 1];
            _engine.setDemoLoopPlaying(true);
            float dummyInL[2048] = {0}, dummyInR[2048] = {0}, dummyOutL[2048] = {0}, dummyOutR[2048] = {0};
            for (int b = 0; b < 18; ++b) {
                for (int s = 0; s < 2048; ++s) {
                    dummyInL[s] = 0.55f * std::sin(2.0 * M_PI * 110.0 * (b * 2048 + s) / 48000.0)
                                + 0.25f * std::sin(2.0 * M_PI * 440.0 * (b * 2048 + s) / 48000.0);
                    dummyInR[s] = dummyInL[s];
                }
                _engine.processBlock(dummyInL, dummyInR, dummyOutL, dummyOutR, 2048);
            }
            [_hwView display];
            NSBitmapImageRep* rep = [_hwView bitmapImageRepForCachingDisplayInRect:[_hwView bounds]];
            [_hwView cacheDisplayInRect:[_hwView bounds] toBitmapImageRep:rep];
            NSData* pngData = [rep representationUsingType:NSBitmapImageFileTypePNG properties:@{}];
            [pngData writeToFile:outPath atomically:YES];
            [NSApp terminate:nil];
        }
    }
}

- (void)startCoreAudio {
    AudioComponentDescription desc{};
    desc.componentType = kAudioUnitType_Output;
    desc.componentSubType = kAudioUnitSubType_DefaultOutput;
    desc.componentManufacturer = kAudioUnitManufacturer_Apple;

    AudioComponent comp = AudioComponentFindNext(nullptr, &desc);
    if (!comp) return;
    if (AudioComponentInstanceNew(comp, &_outputUnit) != noErr) return;

    AudioStreamBasicDescription asbd{};
    asbd.mSampleRate = 48000.0;
    asbd.mFormatID = kAudioFormatLinearPCM;
    asbd.mFormatFlags = kAudioFormatFlagsNativeFloatPacked | kAudioFormatFlagIsNonInterleaved;
    asbd.mBytesPerPacket = sizeof(Float32);
    asbd.mFramesPerPacket = 1;
    asbd.mBytesPerFrame = sizeof(Float32);
    asbd.mChannelsPerFrame = 2;
    asbd.mBitsPerChannel = 32;

    AudioUnitSetProperty(_outputUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, 0, &asbd, sizeof(asbd));

    AURenderCallbackStruct cb{};
    cb.inputProc = StandaloneRenderProc;
    cb.inputProcRefCon = &_engine;
    AudioUnitSetProperty(_outputUnit, kAudioUnitProperty_SetRenderCallback, kAudioUnitScope_Input, 0, &cb, sizeof(cb));

    if (AudioUnitInitialize(_outputUnit) == noErr) {
        AudioOutputUnitStart(_outputUnit);
    }
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)sender {
    return YES;
}

- (void)applicationWillTerminate:(NSNotification*)notification {
    if (_outputUnit) {
        AudioOutputUnitStop(_outputUnit);
        AudioUnitUninitialize(_outputUnit);
        AudioComponentInstanceDispose(_outputUnit);
        _outputUnit = nullptr;
    }
}

@end

int main(int argc, const char* argv[]) {
    @autoreleasepool {
        NSApplication* app = [NSApplication sharedApplication];
        [app setActivationPolicy:NSApplicationActivationPolicyRegular];
        VSRStandaloneAppDelegate* delegate = [[VSRStandaloneAppDelegate alloc] init];
        [app setDelegate:delegate];
        [app run];
    }
    return 0;
}
