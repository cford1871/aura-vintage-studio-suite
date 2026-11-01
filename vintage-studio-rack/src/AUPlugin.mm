#import <Cocoa/Cocoa.h>
#import <AudioUnit/AudioUnit.h>
#import <AudioUnit/AUCocoaUIView.h>
#import <AudioToolbox/AudioToolbox.h>
#include <vector>
#include <cstring>
#include "../include/VSR9512X_DSP.h"
#include "../include/VSR9512X_UI.h"

static const AudioUnitPropertyID kVSRProperty_EnginePtr = 64001;

@interface VSR9512X_AUCocoaViewFactory : NSObject <AUCocoaUIBase>
@end

@implementation VSR9512X_AUCocoaViewFactory

- (unsigned)interfaceVersion { return 0; }
- (NSString*)description { return @"VSR-9512X Studio Effects Processor"; }

- (NSView*)uiViewForAudioUnit:(AudioUnit)inAudioUnit withSize:(NSSize)inPreferredSize {
    vsr::VSR9512XEngine* engine = nullptr;
    UInt32 dataSize = sizeof(engine);
    AudioUnitGetProperty(inAudioUnit, kVSRProperty_EnginePtr, kAudioUnitScope_Global, 0, &engine, &dataSize);

    NSRect rect = NSMakeRect(0, 0, 980, 624);
    VSR9512XHardwareView* view = [[VSR9512XHardwareView alloc] initWithFrame:rect engine:engine];
    [view setParamCallback:[inAudioUnit](int paramId, float value) {
        AudioUnitSetParameter(inAudioUnit, static_cast<AudioUnitParameterID>(paramId), kAudioUnitScope_Global, 0, value, 0);
        AudioUnitParameter param = { inAudioUnit, static_cast<AudioUnitParameterID>(paramId), kAudioUnitScope_Global, 0 };
        AUParameterListenerNotify(nullptr, nullptr, &param);
    }];
    return view;
}

@end

struct PropListener {
    AudioUnitPropertyID id;
    AudioUnitPropertyListenerProc proc;
    void* userData;
};

struct VSR9512XAUInstance {
    AudioComponentPlugInInterface iface;
    AudioComponentInstance instance = nullptr;
    vsr::VSR9512XEngine engine;
    AudioStreamBasicDescription inFormat{};
    AudioStreamBasicDescription outFormat{};
    AURenderCallbackStruct inputCallback{};
    AudioUnitConnection connection{};
    UInt32 maxFramesPerSlice = 4096;
    UInt32 inPlaceProcessing = 1;
    UInt32 bypassEffect = 0;
    bool initialized = false;
    AUPreset currentPreset{ 0, CFSTR("01. Classic S950 + SP1200 Hybrid") };
    std::vector<float> inBufL;
    std::vector<float> inBufR;
    std::vector<PropListener> listeners;

    void notifyListeners(AudioUnitPropertyID propId, AudioUnitScope scope, AudioUnitElement elem) {
        for (const auto& l : listeners) {
            if (l.id == propId && l.proc) {
                l.proc(l.userData, instance, propId, scope, elem);
            }
        }
    }
};

static void InitDefaultASBD(AudioStreamBasicDescription& asbd, Float64 sr = 48000.0, UInt32 channels = 2) {
    std::memset(&asbd, 0, sizeof(asbd));
    asbd.mSampleRate = sr;
    asbd.mFormatID = kAudioFormatLinearPCM;
    asbd.mFormatFlags = kAudioFormatFlagsNativeFloatPacked | kAudioFormatFlagIsNonInterleaved;
    asbd.mBytesPerPacket = sizeof(Float32);
    asbd.mFramesPerPacket = 1;
    asbd.mBytesPerFrame = sizeof(Float32);
    asbd.mChannelsPerFrame = channels;
    asbd.mBitsPerChannel = 32;
}

static OSStatus VSR_Open(void* self, AudioComponentInstance mInstance) {
    auto* au = static_cast<VSR9512XAUInstance*>(self);
    au->instance = mInstance;
    InitDefaultASBD(au->inFormat, 48000.0, 2);
    InitDefaultASBD(au->outFormat, 48000.0, 2);
    au->inBufL.resize(4096, 0.0f);
    au->inBufR.resize(4096, 0.0f);
    au->engine.prepare(48000.0);
    return noErr;
}

static OSStatus VSR_Close(void* self) {
    auto* au = static_cast<VSR9512XAUInstance*>(self);
    delete au;
    return noErr;
}

static OSStatus VSR_Initialize(void* self) {
    auto* au = static_cast<VSR9512XAUInstance*>(self);
    if (au->inFormat.mChannelsPerFrame != au->outFormat.mChannelsPerFrame) {
        return kAudioUnitErr_FormatNotSupported;
    }
    au->engine.prepare(au->outFormat.mSampleRate > 0 ? au->outFormat.mSampleRate : 48000.0);
    au->inBufL.resize(au->maxFramesPerSlice, 0.0f);
    au->inBufR.resize(au->maxFramesPerSlice, 0.0f);
    au->initialized = true;
    return noErr;
}

static OSStatus VSR_Uninitialize(void* self) {
    auto* au = static_cast<VSR9512XAUInstance*>(self);
    au->initialized = false;
    return noErr;
}

static OSStatus VSR_GetPropertyInfo(void* self,
                                    AudioUnitPropertyID inID,
                                    AudioUnitScope inScope,
                                    AudioUnitElement inElement,
                                    UInt32* outDataSize,
                                    Boolean* outWritable) {
    switch (inID) {
        case kAudioUnitProperty_StreamFormat:
            if (inScope != kAudioUnitScope_Input && inScope != kAudioUnitScope_Output) return kAudioUnitErr_InvalidScope;
            if (inElement != 0) return kAudioUnitErr_InvalidElement;
            if (outDataSize) *outDataSize = sizeof(AudioStreamBasicDescription);
            if (outWritable) *outWritable = true;
            return noErr;
        case kAudioUnitProperty_SampleRate:
            if (inScope != kAudioUnitScope_Input && inScope != kAudioUnitScope_Output) return kAudioUnitErr_InvalidScope;
            if (inElement != 0) return kAudioUnitErr_InvalidElement;
            if (outDataSize) *outDataSize = sizeof(Float64);
            if (outWritable) *outWritable = true;
            return noErr;
        case kAudioUnitProperty_ElementCount:
            if (outDataSize) *outDataSize = sizeof(UInt32);
            if (outWritable) *outWritable = false;
            return noErr;
        case kAudioUnitProperty_ParameterList:
            if (inElement != 0) return kAudioUnitErr_InvalidElement;
            if (outDataSize) *outDataSize = (inScope == kAudioUnitScope_Global) ? (vsr::kNumParams * sizeof(AudioUnitParameterID)) : 0;
            if (outWritable) *outWritable = false;
            return noErr;
        case kAudioUnitProperty_ParameterInfo:
            if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
            if (inElement >= vsr::kNumParams) return kAudioUnitErr_InvalidParameter;
            if (outDataSize) *outDataSize = sizeof(AudioUnitParameterInfo);
            if (outWritable) *outWritable = false;
            return noErr;
        case kAudioUnitProperty_Latency:
        case kAudioUnitProperty_TailTime:
            if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
            if (outDataSize) *outDataSize = sizeof(Float64);
            if (outWritable) *outWritable = false;
            return noErr;
        case kAudioUnitProperty_MaximumFramesPerSlice:
        case kAudioUnitProperty_InPlaceProcessing:
        case kAudioUnitProperty_BypassEffect:
            if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
            if (outDataSize) *outDataSize = sizeof(UInt32);
            if (outWritable) *outWritable = true;
            return noErr;
        case kAudioUnitProperty_MakeConnection:
            if (inScope != kAudioUnitScope_Input) return kAudioUnitErr_InvalidScope;
            if (inElement != 0) return kAudioUnitErr_InvalidElement;
            if (outDataSize) *outDataSize = sizeof(AudioUnitConnection);
            if (outWritable) *outWritable = true;
            return noErr;
        case kAudioUnitProperty_SetRenderCallback:
            if (inScope != kAudioUnitScope_Input) return kAudioUnitErr_InvalidScope;
            if (inElement != 0) return kAudioUnitErr_InvalidElement;
            if (outDataSize) *outDataSize = sizeof(AURenderCallbackStruct);
            if (outWritable) *outWritable = true;
            return noErr;
        case kAudioUnitProperty_SupportedNumChannels:
            if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
            if (outDataSize) *outDataSize = 2 * sizeof(AUChannelInfo);
            if (outWritable) *outWritable = false;
            return noErr;
        case kAudioUnitProperty_FactoryPresets:
            if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
            if (outDataSize) *outDataSize = sizeof(CFArrayRef);
            if (outWritable) *outWritable = false;
            return noErr;
        case kAudioUnitProperty_PresentPreset:
        case kAudioUnitProperty_CurrentPreset:
            if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
            if (outDataSize) *outDataSize = sizeof(AUPreset);
            if (outWritable) *outWritable = true;
            return noErr;
        case kAudioUnitProperty_ClassInfo:
            if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
            if (outDataSize) *outDataSize = sizeof(CFPropertyListRef);
            if (outWritable) *outWritable = true;
            return noErr;
        case kAudioUnitProperty_CocoaUI:
            if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
            if (outDataSize) *outDataSize = sizeof(AudioUnitCocoaViewInfo);
            if (outWritable) *outWritable = false;
            return noErr;
        case kVSRProperty_EnginePtr:
            if (outDataSize) *outDataSize = sizeof(void*);
            if (outWritable) *outWritable = false;
            return noErr;
        default:
            return kAudioUnitErr_InvalidProperty;
    }
}

static OSStatus VSR_GetProperty(void* self,
                                AudioUnitPropertyID inID,
                                AudioUnitScope inScope,
                                AudioUnitElement inElement,
                                void* outData,
                                UInt32* ioDataSize) {
    auto* au = static_cast<VSR9512XAUInstance*>(self);
    switch (inID) {
        case kAudioUnitProperty_StreamFormat:
            if (inScope != kAudioUnitScope_Input && inScope != kAudioUnitScope_Output) return kAudioUnitErr_InvalidScope;
            if (inElement != 0) return kAudioUnitErr_InvalidElement;
            if (ioDataSize) *ioDataSize = sizeof(AudioStreamBasicDescription);
            if (outData) {
                *static_cast<AudioStreamBasicDescription*>(outData) = (inScope == kAudioUnitScope_Input) ? au->inFormat : au->outFormat;
            }
            return noErr;
        case kAudioUnitProperty_SampleRate:
            if (inScope != kAudioUnitScope_Input && inScope != kAudioUnitScope_Output) return kAudioUnitErr_InvalidScope;
            if (inElement != 0) return kAudioUnitErr_InvalidElement;
            if (ioDataSize) *ioDataSize = sizeof(Float64);
            if (outData) {
                *static_cast<Float64*>(outData) = (inScope == kAudioUnitScope_Input) ? au->inFormat.mSampleRate : au->outFormat.mSampleRate;
            }
            return noErr;
        case kAudioUnitProperty_ElementCount:
            if (ioDataSize) *ioDataSize = sizeof(UInt32);
            if (outData) {
                *static_cast<UInt32*>(outData) = (inScope == kAudioUnitScope_Input || inScope == kAudioUnitScope_Output || inScope == kAudioUnitScope_Global) ? 1 : 0;
            }
            return noErr;
        case kAudioUnitProperty_ParameterList: {
            if (inElement != 0) return kAudioUnitErr_InvalidElement;
            if (inScope != kAudioUnitScope_Global) {
                if (ioDataSize) *ioDataSize = 0;
                return noErr;
            }
            if (ioDataSize) *ioDataSize = vsr::kNumParams * sizeof(AudioUnitParameterID);
            if (outData) {
                auto* list = static_cast<AudioUnitParameterID*>(outData);
                for (int i = 0; i < vsr::kNumParams; ++i) list[i] = static_cast<AudioUnitParameterID>(i);
            }
            return noErr;
        }
        case kAudioUnitProperty_ParameterInfo: {
            if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
            if (inElement >= vsr::kNumParams) return kAudioUnitErr_InvalidParameter;
            if (ioDataSize) *ioDataSize = sizeof(AudioUnitParameterInfo);
            if (outData) {
                auto& info = *static_cast<AudioUnitParameterInfo*>(outData);
                std::memset(&info, 0, sizeof(info));
                const auto& m = vsr::kParamTable[inElement];
                strncpy(info.name, m.name, sizeof(info.name) - 1);
                info.cfNameString = CFStringCreateWithCString(kCFAllocatorDefault, m.name, kCFStringEncodingUTF8);
                info.minValue = m.minVal;
                info.maxValue = m.maxVal;
                info.defaultValue = m.defaultVal;
                info.flags = kAudioUnitParameterFlag_IsWritable | kAudioUnitParameterFlag_IsReadable |
                             kAudioUnitParameterFlag_HasCFNameString | kAudioUnitParameterFlag_CFNameRelease;
                if (m.isBoolean) {
                    info.unit = kAudioUnitParameterUnit_Boolean;
                } else if (m.isStepped) {
                    info.unit = kAudioUnitParameterUnit_Indexed;
                } else if (strcmp(m.unit, "dB") == 0) {
                    info.unit = kAudioUnitParameterUnit_Decibels;
                } else if (strcmp(m.unit, "Hz") == 0) {
                    info.unit = kAudioUnitParameterUnit_Hertz;
                } else if (strcmp(m.unit, "%") == 0) {
                    info.unit = kAudioUnitParameterUnit_Percent;
                } else if (strcmp(m.unit, "ms") == 0) {
                    info.unit = kAudioUnitParameterUnit_Milliseconds;
                } else {
                    info.unit = kAudioUnitParameterUnit_Generic;
                }
            }
            return noErr;
        }
        case kAudioUnitProperty_Latency:
            if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
            if (ioDataSize) *ioDataSize = sizeof(Float64);
            if (outData) *static_cast<Float64*>(outData) = 0.0;
            return noErr;
        case kAudioUnitProperty_TailTime:
            if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
            if (ioDataSize) *ioDataSize = sizeof(Float64);
            if (outData) *static_cast<Float64*>(outData) = 0.05;
            return noErr;
        case kAudioUnitProperty_MaximumFramesPerSlice:
            if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
            if (ioDataSize) *ioDataSize = sizeof(UInt32);
            if (outData) *static_cast<UInt32*>(outData) = au->maxFramesPerSlice;
            return noErr;
        case kAudioUnitProperty_InPlaceProcessing:
            if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
            if (ioDataSize) *ioDataSize = sizeof(UInt32);
            if (outData) *static_cast<UInt32*>(outData) = au->inPlaceProcessing;
            return noErr;
        case kAudioUnitProperty_BypassEffect:
            if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
            if (ioDataSize) *ioDataSize = sizeof(UInt32);
            if (outData) *static_cast<UInt32*>(outData) = au->bypassEffect;
            return noErr;
        case kAudioUnitProperty_SupportedNumChannels: {
            if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
            if (ioDataSize) *ioDataSize = 2 * sizeof(AUChannelInfo);
            if (outData) {
                auto* chans = static_cast<AUChannelInfo*>(outData);
                chans[0].inChannels = 1; chans[0].outChannels = 1;
                chans[1].inChannels = 2; chans[1].outChannels = 2;
            }
            return noErr;
        }
        case kAudioUnitProperty_FactoryPresets: {
            if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
            if (ioDataSize) *ioDataSize = sizeof(CFArrayRef);
            if (outData) {
                CFMutableArrayRef arr = CFArrayCreateMutable(kCFAllocatorDefault, vsr::kNumPresets, nullptr);
                static AUPreset sPresets[vsr::kNumPresets];
                for (int i = 0; i < vsr::kNumPresets; ++i) {
                    sPresets[i].presetNumber = i;
                    sPresets[i].presetName = CFStringCreateWithCString(kCFAllocatorDefault, vsr::kFactoryPresets[i].name, kCFStringEncodingUTF8);
                    CFArrayAppendValue(arr, &sPresets[i]);
                }
                *static_cast<CFArrayRef*>(outData) = arr;
            }
            return noErr;
        }
        case kAudioUnitProperty_PresentPreset:
        case kAudioUnitProperty_CurrentPreset: {
            if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
            if (ioDataSize) *ioDataSize = sizeof(AUPreset);
            if (outData) {
                *static_cast<AUPreset*>(outData) = au->currentPreset;
                if (inID == kAudioUnitProperty_PresentPreset && au->currentPreset.presetName) {
                    CFRetain(au->currentPreset.presetName);
                }
            }
            return noErr;
        }
        case kAudioUnitProperty_ClassInfo: {
            if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
            if (ioDataSize) *ioDataSize = sizeof(CFPropertyListRef);
            if (outData) {
                CFMutableDictionaryRef dict = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
                int version = 1;
                CFNumberRef vNum = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &version);
                int type = 'aufx', sub = 'V95X', manu = 'BSKZ';
                CFNumberRef tNum = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &type);
                CFNumberRef sNum = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &sub);
                CFNumberRef mNum = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &manu);
                CFDictionarySetValue(dict, CFSTR(kAUPresetVersionKey), vNum);
                CFDictionarySetValue(dict, CFSTR(kAUPresetTypeKey), tNum);
                CFDictionarySetValue(dict, CFSTR(kAUPresetSubtypeKey), sNum);
                CFDictionarySetValue(dict, CFSTR(kAUPresetManufacturerKey), mNum);
                CFDictionarySetValue(dict, CFSTR(kAUPresetNameKey), au->currentPreset.presetName ? au->currentPreset.presetName : CFSTR("Default"));
                CFRelease(vNum); CFRelease(tNum); CFRelease(sNum); CFRelease(mNum);

                CFMutableDataRef data = CFDataCreateMutable(kCFAllocatorDefault, vsr::kNumParams * sizeof(float));
                for (int i = 0; i < vsr::kNumParams; ++i) {
                    float val = au->engine.getParam(i);
                    CFDataAppendBytes(data, reinterpret_cast<const UInt8*>(&val), sizeof(float));
                }
                CFDictionarySetValue(dict, CFSTR(kAUPresetDataKey), data);
                CFRelease(data);

                *static_cast<CFPropertyListRef*>(outData) = dict;
            }
            return noErr;
        }
        case kAudioUnitProperty_CocoaUI: {
            if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
            if (ioDataSize) *ioDataSize = sizeof(AudioUnitCocoaViewInfo);
            if (outData) {
                auto& viewInfo = *static_cast<AudioUnitCocoaViewInfo*>(outData);
                CFBundleRef bundle = CFBundleGetBundleWithIdentifier(CFSTR("com.beatskillz.vsr9512x.au"));
                if (!bundle) {
                    NSBundle* nsB = [NSBundle bundleForClass:[VSR9512X_AUCocoaViewFactory class]];
                    if (nsB) viewInfo.mCocoaAUViewBundleLocation = (__bridge_retained CFURLRef)[nsB bundleURL];
                } else {
                    viewInfo.mCocoaAUViewBundleLocation = CFBundleCopyBundleURL(bundle);
                }
                viewInfo.mCocoaAUViewClass[0] = CFStringCreateWithCString(kCFAllocatorDefault, "VSR9512X_AUCocoaViewFactory", kCFStringEncodingUTF8);
            }
            return noErr;
        }
        case kVSRProperty_EnginePtr: {
            if (ioDataSize) *ioDataSize = sizeof(void*);
            if (outData) *static_cast<vsr::VSR9512XEngine**>(outData) = &au->engine;
            return noErr;
        }
        default:
            return kAudioUnitErr_InvalidProperty;
    }
}

static OSStatus VSR_SetProperty(void* self,
                                AudioUnitPropertyID inID,
                                AudioUnitScope inScope,
                                AudioUnitElement inElement,
                                const void* inData,
                                UInt32 inDataSize) {
    auto* au = static_cast<VSR9512XAUInstance*>(self);
    switch (inID) {
        case kAudioUnitProperty_StreamFormat: {
            if (inScope != kAudioUnitScope_Input && inScope != kAudioUnitScope_Output) return kAudioUnitErr_InvalidScope;
            if (inElement != 0) return kAudioUnitErr_InvalidElement;
            if (au->initialized) return kAudioUnitErr_Initialized;
            if (!inData || inDataSize < sizeof(AudioStreamBasicDescription)) return kAudioUnitErr_InvalidPropertyValue;
            const auto& fmt = *static_cast<const AudioStreamBasicDescription*>(inData);
            if (fmt.mFormatID != kAudioFormatLinearPCM) return kAudioUnitErr_FormatNotSupported;
            if (fmt.mChannelsPerFrame < 1 || fmt.mChannelsPerFrame > 2) return kAudioUnitErr_FormatNotSupported;
            if (fmt.mBitsPerChannel != 32 || !(fmt.mFormatFlags & kAudioFormatFlagIsFloat)) return kAudioUnitErr_FormatNotSupported;
            if (inScope == kAudioUnitScope_Input) au->inFormat = fmt;
            else au->outFormat = fmt;
            if (fmt.mSampleRate > 0) au->engine.prepare(fmt.mSampleRate);
            return noErr;
        }
        case kAudioUnitProperty_SampleRate: {
            if (inScope != kAudioUnitScope_Input && inScope != kAudioUnitScope_Output) return kAudioUnitErr_InvalidScope;
            if (inElement != 0) return kAudioUnitErr_InvalidElement;
            if (au->initialized) return kAudioUnitErr_Initialized;
            if (!inData || inDataSize < sizeof(Float64)) return kAudioUnitErr_InvalidPropertyValue;
            Float64 sr = *static_cast<const Float64*>(inData);
            if (inScope == kAudioUnitScope_Input) au->inFormat.mSampleRate = sr;
            else au->outFormat.mSampleRate = sr;
            au->engine.prepare(sr);
            return noErr;
        }
        case kAudioUnitProperty_MaximumFramesPerSlice:
            if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
            if (au->initialized) return kAudioUnitErr_Initialized;
            if (!inData || inDataSize < sizeof(UInt32)) return kAudioUnitErr_InvalidPropertyValue;
            au->maxFramesPerSlice = *static_cast<const UInt32*>(inData);
            au->inBufL.resize(au->maxFramesPerSlice, 0.0f);
            au->inBufR.resize(au->maxFramesPerSlice, 0.0f);
            au->notifyListeners(kAudioUnitProperty_MaximumFramesPerSlice, inScope, inElement);
            return noErr;
        case kAudioUnitProperty_InPlaceProcessing:
            if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
            if (!inData || inDataSize < sizeof(UInt32)) return kAudioUnitErr_InvalidPropertyValue;
            au->inPlaceProcessing = *static_cast<const UInt32*>(inData);
            return noErr;
        case kAudioUnitProperty_BypassEffect:
            if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
            if (!inData || inDataSize < sizeof(UInt32)) return kAudioUnitErr_InvalidPropertyValue;
            au->bypassEffect = *static_cast<const UInt32*>(inData);
            au->notifyListeners(kAudioUnitProperty_BypassEffect, inScope, inElement);
            return noErr;
        case kAudioUnitProperty_MakeConnection: {
            if (inScope != kAudioUnitScope_Input) return kAudioUnitErr_InvalidScope;
            if (inElement != 0) return kAudioUnitErr_InvalidElement;
            if (!inData || inDataSize < sizeof(AudioUnitConnection)) return kAudioUnitErr_InvalidPropertyValue;
            au->connection = *static_cast<const AudioUnitConnection*>(inData);
            if (au->connection.sourceAudioUnit) {
                AudioStreamBasicDescription srcFmt{};
                UInt32 sz = sizeof(srcFmt);
                if (AudioUnitGetProperty(au->connection.sourceAudioUnit,
                                         kAudioUnitProperty_StreamFormat,
                                         kAudioUnitScope_Output,
                                         au->connection.sourceOutputNumber,
                                         &srcFmt, &sz) == noErr) {
                    if (srcFmt.mFormatID != kAudioFormatLinearPCM ||
                        srcFmt.mChannelsPerFrame < 1 || srcFmt.mChannelsPerFrame > 2 ||
                        srcFmt.mBitsPerChannel != 32 || !(srcFmt.mFormatFlags & kAudioFormatFlagIsFloat)) {
                        return kAudioUnitErr_FormatNotSupported;
                    }
                    au->inFormat = srcFmt;
                }
            }
            return noErr;
        }
        case kAudioUnitProperty_SetRenderCallback:
            if (inScope != kAudioUnitScope_Input) return kAudioUnitErr_InvalidScope;
            if (inElement != 0) return kAudioUnitErr_InvalidElement;
            if (!inData || inDataSize < sizeof(AURenderCallbackStruct)) return kAudioUnitErr_InvalidPropertyValue;
            au->inputCallback = *static_cast<const AURenderCallbackStruct*>(inData);
            return noErr;
        case kAudioUnitProperty_PresentPreset:
        case kAudioUnitProperty_CurrentPreset: {
            if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
            if (!inData || inDataSize < sizeof(AUPreset)) return kAudioUnitErr_InvalidPropertyValue;
            const auto& preset = *static_cast<const AUPreset*>(inData);
            au->currentPreset = preset;
            if (preset.presetNumber >= 0 && preset.presetNumber < vsr::kNumPresets) {
                au->engine.loadPreset(preset.presetNumber);
                au->currentPreset.presetName = CFStringCreateWithCString(kCFAllocatorDefault, vsr::kFactoryPresets[preset.presetNumber].name, kCFStringEncodingUTF8);
            }
            return noErr;
        }
        case kAudioUnitProperty_ClassInfo: {
            if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
            if (!inData) return kAudioUnitErr_InvalidPropertyValue;
            CFDictionaryRef dict = *static_cast<const CFDictionaryRef*>(inData);
            if (!dict || CFGetTypeID(dict) != CFDictionaryGetTypeID()) return kAudioUnitErr_InvalidPropertyValue;
            CFDataRef data = static_cast<CFDataRef>(CFDictionaryGetValue(dict, CFSTR(kAUPresetDataKey)));
            if (data && CFDataGetLength(data) == vsr::kNumParams * static_cast<CFIndex>(sizeof(float))) {
                const float* vals = reinterpret_cast<const float*>(CFDataGetBytePtr(data));
                for (int i = 0; i < vsr::kNumParams; ++i) au->engine.setParam(i, vals[i]);
            }
            return noErr;
        }
        default:
            return kAudioUnitErr_InvalidProperty;
    }
}

static OSStatus VSR_GetParameter(void* self,
                                 AudioUnitParameterID inID,
                                 AudioUnitScope inScope,
                                 AudioUnitElement inElement,
                                 AudioUnitParameterValue* outValue) {
    auto* au = static_cast<VSR9512XAUInstance*>(self);
    if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
    if (inID >= vsr::kNumParams) return kAudioUnitErr_InvalidParameter;
    if (outValue) *outValue = au->engine.getParam(static_cast<int>(inID));
    return noErr;
}

static OSStatus VSR_SetParameter(void* self,
                                 AudioUnitParameterID inID,
                                 AudioUnitScope inScope,
                                 AudioUnitElement inElement,
                                 AudioUnitParameterValue inValue,
                                 UInt32 inBufferOffsetInFrames) {
    auto* au = static_cast<VSR9512XAUInstance*>(self);
    if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
    if (inID >= vsr::kNumParams) return kAudioUnitErr_InvalidParameter;
    au->engine.setParam(static_cast<int>(inID), inValue);
    return noErr;
}

static OSStatus VSR_Render(void* self,
                           AudioUnitRenderActionFlags* ioActionFlags,
                           const AudioTimeStamp* inTimeStamp,
                           UInt32 inOutputBusNumber,
                           UInt32 inNumberFrames,
                           AudioBufferList* ioData) {
    auto* au = static_cast<VSR9512XAUInstance*>(self);
    if (!au->initialized) return kAudioUnitErr_Uninitialized;
    if (inOutputBusNumber != 0) return kAudioUnitErr_InvalidElement;
    if (inNumberFrames > au->maxFramesPerSlice) return kAudioUnitErr_TooManyFramesToProcess;
    if (!ioData || ioData->mNumberBuffers == 0) return kAudioUnitErr_InvalidParameter;

    // Prepare temporary input buffer list in case caller provided null mData pointers
    AudioBufferList* pullList = reinterpret_cast<AudioBufferList*>(alloca(sizeof(AudioBufferList) + sizeof(AudioBuffer) * ioData->mNumberBuffers));
    pullList->mNumberBuffers = ioData->mNumberBuffers;
    for (UInt32 b = 0; b < ioData->mNumberBuffers; ++b) {
        pullList->mBuffers[b].mNumberChannels = 1;
        pullList->mBuffers[b].mDataByteSize = inNumberFrames * sizeof(float);
        pullList->mBuffers[b].mData = (b == 0) ? au->inBufL.data() : au->inBufR.data();
        if (!ioData->mBuffers[b].mData) {
            ioData->mBuffers[b].mData = (b == 0) ? au->inBufL.data() : au->inBufR.data();
        }
        ioData->mBuffers[b].mDataByteSize = inNumberFrames * sizeof(float);
    }

    if (au->inputCallback.inputProc) {
        OSStatus err = au->inputCallback.inputProc(au->inputCallback.inputProcRefCon,
                                                   ioActionFlags,
                                                   inTimeStamp,
                                                   0,
                                                   inNumberFrames,
                                                   pullList);
        if (err != noErr) return err;
    } else if (au->connection.sourceAudioUnit) {
        OSStatus err = AudioUnitRender(au->connection.sourceAudioUnit,
                                       ioActionFlags,
                                       inTimeStamp,
                                       au->connection.sourceOutputNumber,
                                       inNumberFrames,
                                       pullList);
        if (err != noErr) return err;
    } else {
        return kAudioUnitErr_NoConnection;
    }

    const float* inL = static_cast<const float*>(pullList->mBuffers[0].mData);
    const float* inR = (pullList->mNumberBuffers > 1 && pullList->mBuffers[1].mData)
                       ? static_cast<const float*>(pullList->mBuffers[1].mData)
                       : inL;
    float* outL = static_cast<float*>(ioData->mBuffers[0].mData);
    float* outR = (ioData->mNumberBuffers > 1 && ioData->mBuffers[1].mData)
                  ? static_cast<float*>(ioData->mBuffers[1].mData)
                  : nullptr;

    au->engine.processBlock(inL, inR, outL, outR, static_cast<int>(inNumberFrames));
    return noErr;
}

static OSStatus VSR_Reset(void* self, AudioUnitScope inScope, AudioUnitElement inElement) {
    auto* au = static_cast<VSR9512XAUInstance*>(self);
    au->engine.prepare(au->outFormat.mSampleRate > 0 ? au->outFormat.mSampleRate : 48000.0);
    return noErr;
}

static OSStatus VSR_AddPropertyListener(void* self, AudioUnitPropertyID prop, AudioUnitPropertyListenerProc proc, void* userData) {
    auto* au = static_cast<VSR9512XAUInstance*>(self);
    au->listeners.push_back({ prop, proc, userData });
    return noErr;
}
static OSStatus VSR_RemovePropertyListenerWithUserData(void* self, AudioUnitPropertyID prop, AudioUnitPropertyListenerProc proc, void* userData) {
    auto* au = static_cast<VSR9512XAUInstance*>(self);
    au->listeners.erase(std::remove_if(au->listeners.begin(), au->listeners.end(),
        [&](const PropListener& l) { return l.id == prop && l.proc == proc && l.userData == userData; }),
        au->listeners.end());
    return noErr;
}
static OSStatus VSR_AddRenderNotify(void*, AURenderCallback, void*) { return noErr; }
static OSStatus VSR_RemoveRenderNotify(void*, AURenderCallback, void*) { return noErr; }
static OSStatus VSR_ScheduleParameters(void* self, const AudioUnitParameterEvent* inParameterEvent, UInt32 inNumParamEvents) {
    for (UInt32 i = 0; i < inNumParamEvents; ++i) {
        if (inParameterEvent[i].eventType == kParameterEvent_Immediate) {
            VSR_SetParameter(self,
                             inParameterEvent[i].parameter,
                             inParameterEvent[i].scope,
                             inParameterEvent[i].element,
                             inParameterEvent[i].eventValues.immediate.value,
                             inParameterEvent[i].eventValues.immediate.bufferOffset);
        }
    }
    return noErr;
}

static AudioComponentMethod VSR_Lookup(SInt16 selector) {
    switch (selector) {
        case kAudioUnitInitializeSelect:                         return reinterpret_cast<AudioComponentMethod>(VSR_Initialize);
        case kAudioUnitUninitializeSelect:                       return reinterpret_cast<AudioComponentMethod>(VSR_Uninitialize);
        case kAudioUnitGetPropertyInfoSelect:                    return reinterpret_cast<AudioComponentMethod>(VSR_GetPropertyInfo);
        case kAudioUnitGetPropertySelect:                        return reinterpret_cast<AudioComponentMethod>(VSR_GetProperty);
        case kAudioUnitSetPropertySelect:                        return reinterpret_cast<AudioComponentMethod>(VSR_SetProperty);
        case kAudioUnitAddPropertyListenerSelect:                return reinterpret_cast<AudioComponentMethod>(VSR_AddPropertyListener);
        case kAudioUnitRemovePropertyListenerWithUserDataSelect: return reinterpret_cast<AudioComponentMethod>(VSR_RemovePropertyListenerWithUserData);
        case kAudioUnitAddRenderNotifySelect:                    return reinterpret_cast<AudioComponentMethod>(VSR_AddRenderNotify);
        case kAudioUnitRemoveRenderNotifySelect:                 return reinterpret_cast<AudioComponentMethod>(VSR_RemoveRenderNotify);
        case kAudioUnitGetParameterSelect:                       return reinterpret_cast<AudioComponentMethod>(VSR_GetParameter);
        case kAudioUnitSetParameterSelect:                       return reinterpret_cast<AudioComponentMethod>(VSR_SetParameter);
        case kAudioUnitScheduleParametersSelect:                 return reinterpret_cast<AudioComponentMethod>(VSR_ScheduleParameters);
        case kAudioUnitRenderSelect:                             return reinterpret_cast<AudioComponentMethod>(VSR_Render);
        case kAudioUnitResetSelect:                              return reinterpret_cast<AudioComponentMethod>(VSR_Reset);
        default:                                                 return nullptr;
    }
}

extern "C" __attribute__((visibility("default")))
void* VSR9512XAUFactory(const AudioComponentDescription* inDesc) {
    auto* inst = new VSR9512XAUInstance();
    inst->iface.Open = VSR_Open;
    inst->iface.Close = VSR_Close;
    inst->iface.Lookup = VSR_Lookup;
    inst->iface.reserved = nullptr;
    return &inst->iface;
}
