#import <Cocoa/Cocoa.h>
#import <AudioToolbox/AudioToolbox.h>
#include "../include/VSRMaster_DSP.h"
#include "../include/VSRMaster_UI.h"

@interface VSRMasterStandaloneAppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate> {
    NSWindow* _window;
    VSRMasterHardwareView* _hwView;
    vsrm::VSRMasterEngine _engine;
    AudioComponentInstance _outputUnit;
}
@end

static OSStatus StandaloneRenderProc(void* inRefCon,
                                     AudioUnitRenderActionFlags* ioActionFlags,
                                     const AudioTimeStamp* inTimeStamp,
                                     UInt32 inBusNumber,
                                     UInt32 inNumberFrames,
                                     AudioBufferList* ioData) {
    auto* eng = static_cast<vsrm::VSRMasterEngine*>(inRefCon);
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

@implementation VSRMasterStandaloneAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification*)notification {
    _engine.prepare(48000.0);

    NSRect frame = NSMakeRect(70, 90, 1280, 720);
    NSUInteger style = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable;
    _window = [[NSWindow alloc] initWithContentRect:frame
                                          styleMask:style
                                            backing:NSBackingStoreBuffered
                                              defer:NO];
    [_window setTitle:@"AURA MASTER v2.0 // Resizable Master Bus Console (VST3 / AU)"];
    [_window setMinSize:NSMakeSize(800, 450)];
    [_window setMaxSize:NSMakeSize(2560, 1440)];
    [_window setContentAspectRatio:NSMakeSize(16, 9)];
    [_window setDelegate:self];
    [_window setBackgroundColor:[NSColor colorWithCalibratedRed:0.055 green:0.063 blue:0.078 alpha:1.0]];

    _hwView = [[VSRMasterHardwareView alloc] initWithFrame:NSMakeRect(0, 0, 1280, 720) engine:&_engine];
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
        VSRMasterStandaloneAppDelegate* delegate = [[VSRMasterStandaloneAppDelegate alloc] init];
        [app setDelegate:delegate];
        [app run];
    }
    return 0;
}
