#import <Cocoa/Cocoa.h>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <atomic>
#include "../include/VSRMaster_DSP.h"
#include "../include/VSRMaster_UI.h"

// ============================================================================
// Minimal Steinberg VST3 3.7 C++ Binary-Compatible ABI Definitions (macOS 64-bit)
// ============================================================================
namespace vst3 {

using int8 = int8_t;
using uint8 = uint8_t;
using int16 = int16_t;
using uint16 = uint16_t;
using int32 = int32_t;
using uint32 = uint32_t;
using int64 = int64_t;
using uint64 = uint64_t;
using tresult = int32;
using TBool = uint8;
using char8 = char;
using char16 = char16_t;
using TChar = char16;
using FIDString = const char8*;
using ParamID = uint32;
using ParamValue = double;
using Sample32 = float;
using Sample64 = double;
using SpeakerArrangement = uint64;
using TUID = char[16];

static constexpr tresult kResultOk = 0;
static constexpr tresult kResultTrue = 0;
static constexpr tresult kResultFalse = 1;
static constexpr tresult kNoInterface = -1;
static constexpr tresult kInvalidArgument = 2;
static constexpr tresult kNotImplemented = 3;

static inline bool iidEqual(const TUID a, const TUID b) {
    return std::memcmp(a, b, 16) == 0;
}

#define DEF_TUID(name, l1, l2, l3, l4) \
    static const TUID name = { \
        static_cast<char>((l1 >> 24) & 0xFF), static_cast<char>((l1 >> 16) & 0xFF), static_cast<char>((l1 >> 8) & 0xFF), static_cast<char>(l1 & 0xFF), \
        static_cast<char>((l2 >> 24) & 0xFF), static_cast<char>((l2 >> 16) & 0xFF), static_cast<char>((l2 >> 8) & 0xFF), static_cast<char>(l2 & 0xFF), \
        static_cast<char>((l3 >> 24) & 0xFF), static_cast<char>((l3 >> 16) & 0xFF), static_cast<char>((l3 >> 8) & 0xFF), static_cast<char>(l3 & 0xFF), \
        static_cast<char>((l4 >> 24) & 0xFF), static_cast<char>((l4 >> 16) & 0xFF), static_cast<char>((l4 >> 8) & 0xFF), static_cast<char>(l4 & 0xFF) \
    }

DEF_TUID(kFUnknownIID,          0x00000000, 0x00000000, 0xC0000000, 0x00000046);
DEF_TUID(kIPluginBaseIID,       0x22888DDB, 0x156E45AE, 0x8358B348, 0x08190625);
DEF_TUID(kIPluginFactoryIID,    0x7A4D811C, 0x52114A1F, 0xAED9D2EE, 0x0B43BF9F);
DEF_TUID(kIPluginFactory2IID,   0x0007B650, 0xF24B4C0B, 0xA464EDB9, 0xF00B2ABB);
DEF_TUID(kIPluginFactory3IID,   0x4555A2AB, 0xC1234E57, 0x9B122910, 0x36878931);
DEF_TUID(kIComponentIID,        0xE831FF31, 0xF2D54301, 0x928EBBEE, 0x25697802);
DEF_TUID(kIAudioProcessorIID,   0x42043F99, 0xB7DA453C, 0xA569E79D, 0x9AAEC33D);
DEF_TUID(kIEditControllerIID,   0xDCD7BBE3, 0x7742448D, 0xA874AACC, 0x979C759E);
DEF_TUID(kIPlugViewIID,         0x5BC32507, 0xD06049EA, 0xA6151B52, 0x2B755B29);
DEF_TUID(kVSRMasterProcCID,     0x5653524D, 0x41535452, 0x50524F43, 0x00120536);
DEF_TUID(kVSRMasterCtrlCID,     0x5653524D, 0x41535452, 0x4354524C, 0x00120537);

static void str8ToStr16(TChar* dst, const char* src, int maxLen = 128) {
    int i = 0;
    for (; i < maxLen - 1 && src[i] != '\0'; ++i) {
        dst[i] = static_cast<unsigned char>(src[i]);
    }
    dst[i] = 0;
}

class FUnknown {
public:
    virtual tresult queryInterface(const TUID _iid, void** obj) = 0;
    virtual uint32 addRef() = 0;
    virtual uint32 release() = 0;
};

class IBStream : public FUnknown {
public:
    virtual tresult read(void* buffer, int32 numBytes, int32* numBytesRead) = 0;
    virtual tresult write(void* buffer, int32 numBytes, int32* numBytesWritten) = 0;
    virtual tresult seek(int64 pos, int32 mode, int64* result) = 0;
    virtual tresult tell(int64* pos) = 0;
};

class IPluginBase : public FUnknown {
public:
    virtual tresult initialize(FUnknown* context) = 0;
    virtual tresult terminate() = 0;
};

struct BusInfo {
    int32 mediaType;
    int32 direction;
    int32 channelCount;
    TChar name[128];
    int32 busType;
    uint32 flags;
};

struct RoutingInfo {
    int32 mediaType;
    int32 busIndex;
    int32 channel;
};

class IComponent : public IPluginBase {
public:
    virtual tresult getControllerClassId(TUID classId) = 0;
    virtual tresult setIoMode(int32 mode) = 0;
    virtual int32 getBusCount(int32 type, int32 dir) = 0;
    virtual tresult getBusInfo(int32 type, int32 dir, int32 index, BusInfo& bus) = 0;
    virtual tresult getRoutingInfo(RoutingInfo& inInfo, RoutingInfo& outInfo) = 0;
    virtual tresult activateBus(int32 type, int32 dir, int32 index, TBool state) = 0;
    virtual tresult setActive(TBool state) = 0;
    virtual tresult setState(IBStream* state) = 0;
    virtual tresult getState(IBStream* state) = 0;
};

struct ProcessSetup {
    int32 processMode;
    int32 symbolicSampleSize;
    int32 maxSamplesPerBlock;
    Sample64 sampleRate;
};

class IParamValueQueue : public FUnknown {
public:
    virtual ParamID getParameterId() = 0;
    virtual int32 getPointCount() = 0;
    virtual tresult getPoint(int32 index, int32& sampleOffset, ParamValue& value) = 0;
    virtual tresult addPoint(int32 sampleOffset, ParamValue value, int32& index) = 0;
};

class IParameterChanges : public FUnknown {
public:
    virtual int32 getParameterCount() = 0;
    virtual IParamValueQueue* getParameterData(int32 index) = 0;
    virtual IParamValueQueue* addParameterData(const ParamID& id, int32& index) = 0;
};

struct AudioBusBuffers {
    int32 numChannels;
    uint64 silenceFlags;
    union {
        Sample32** channelBuffers32;
        Sample64** channelBuffers64;
    };
};

struct ProcessData {
    int32 processMode;
    int32 symbolicSampleSize;
    int32 numSamples;
    int32 numInputs;
    int32 numOutputs;
    AudioBusBuffers* inputs;
    AudioBusBuffers* outputs;
    IParameterChanges* inputParameterChanges;
    IParameterChanges* outputParameterChanges;
    void* inputEvents;
    void* outputEvents;
    void* processContext;
};

class IAudioProcessor : public FUnknown {
public:
    virtual tresult setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts) = 0;
    virtual tresult getBusArrangement(int32 dir, int32 index, SpeakerArrangement& arr) = 0;
    virtual tresult canProcessSampleSize(int32 symbolicSampleSize) = 0;
    virtual uint32 getLatencySamples() = 0;
    virtual tresult setupProcessing(ProcessSetup& setup) = 0;
    virtual tresult setProcessing(TBool state) = 0;
    virtual tresult process(ProcessData& data) = 0;
    virtual uint32 getTailSamples() = 0;
};

struct ViewRect {
    int32 left = 0;
    int32 top = 0;
    int32 right = 1280;
    int32 bottom = 720;
};

class IPlugFrame : public FUnknown {
public:
    virtual tresult resizeView(class IPlugView* view, ViewRect* newSize) = 0;
};

class IPlugView : public FUnknown {
public:
    virtual tresult isPlatformTypeSupported(FIDString type) = 0;
    virtual tresult attached(void* parent, FIDString type) = 0;
    virtual tresult removed() = 0;
    virtual tresult onWheel(float distance) = 0;
    virtual tresult onKeyDown(char16 key, int16 keyCode, int16 modifiers) = 0;
    virtual tresult onKeyUp(char16 key, int16 keyCode, int16 modifiers) = 0;
    virtual tresult getSize(ViewRect* size) = 0;
    virtual tresult onSize(ViewRect* newSize) = 0;
    virtual tresult onFocus(TBool state) = 0;
    virtual tresult setFrame(IPlugFrame* frame) = 0;
    virtual tresult canResize() = 0;
    virtual tresult checkSizeConstraint(ViewRect* rect) = 0;
};

struct ParameterInfo {
    ParamID id;
    TChar title[128];
    TChar shortTitle[128];
    TChar units[128];
    int32 stepCount;
    ParamValue defaultNormalizedValue;
    int32 unitId;
    int32 flags;
};

class IComponentHandler : public FUnknown {
public:
    virtual tresult beginEdit(ParamID id) = 0;
    virtual tresult performEdit(ParamID id, ParamValue valueNormalized) = 0;
    virtual tresult endEdit(ParamID id) = 0;
    virtual tresult restartComponent(int32 flags) = 0;
};

class IEditController : public IPluginBase {
public:
    virtual tresult setComponentState(IBStream* state) = 0;
    virtual tresult setState(IBStream* state) = 0;
    virtual tresult getState(IBStream* state) = 0;
    virtual int32 getParameterCount() = 0;
    virtual tresult getParameterInfo(int32 paramIndex, ParameterInfo& info) = 0;
    virtual tresult getParamStringByValue(ParamID id, ParamValue valueNormalized, TChar* string) = 0;
    virtual tresult getParamValueByString(ParamID id, TChar* string, ParamValue& valueNormalized) = 0;
    virtual ParamValue normalizedParamToPlain(ParamID id, ParamValue valueNormalized) = 0;
    virtual ParamValue plainParamToNormalized(ParamID id, ParamValue plainValue) = 0;
    virtual ParamValue getParamNormalized(ParamID id) = 0;
    virtual tresult setParamNormalized(ParamID id, ParamValue value) = 0;
    virtual tresult setComponentHandler(IComponentHandler* handler) = 0;
    virtual IPlugView* createView(FIDString name) = 0;
};

// ============================================================================
// VSR-MASTER VST3 PlugView (Hosts VSRMasterHardwareView inside NSView)
// ============================================================================
class VSRMasterPlugView : public IPlugView {
public:
    VSRMasterPlugView(vsrm::VSRMasterEngine* engine, IComponentHandler** handlerPtr)
        : engine_(engine), handlerPtr_(handlerPtr) {}

    virtual ~VSRMasterPlugView() {
        removed();
    }

    tresult queryInterface(const TUID _iid, void** obj) override {
        if (iidEqual(_iid, kFUnknownIID) || iidEqual(_iid, kIPlugViewIID)) {
            addRef();
            *obj = static_cast<IPlugView*>(this);
            return kResultOk;
        }
        *obj = nullptr;
        return kNoInterface;
    }
    uint32 addRef() override { return ++refCount_; }
    uint32 release() override {
        uint32 r = --refCount_;
        if (r == 0) delete this;
        return r;
    }

    tresult isPlatformTypeSupported(FIDString type) override {
        if (type && std::strcmp(type, "NSView") == 0) return kResultTrue;
        return kResultFalse;
    }

    tresult attached(void* parent, FIDString type) override {
        if (!parent || !type || std::strcmp(type, "NSView") != 0) return kInvalidArgument;
        NSView* parentView = (__bridge NSView*)parent;
        NSRect rect = NSMakeRect(0, 0, 1280, 720);
        hwView_ = [[VSRMasterHardwareView alloc] initWithFrame:rect engine:engine_];
        vsrm::VSRMasterEngine* eng = engine_;
        IComponentHandler** hPtr = handlerPtr_;
        [hwView_ setParamCallback:[eng, hPtr](int pid, float plainVal) {
            if (hPtr && *hPtr && eng) {
                ParamID id = static_cast<ParamID>(pid);
                ParamValue norm = static_cast<ParamValue>(eng->getParamNormalized(pid));
                (*hPtr)->beginEdit(id);
                (*hPtr)->performEdit(id, norm);
                (*hPtr)->endEdit(id);
            }
        }];
        VSRMasterPlugView* selfView = this;
        [hwView_ setResizeCallback:[selfView](CGFloat newW, CGFloat newH) {
            selfView->requestHostResize(static_cast<int32>(newW), static_cast<int32>(newH));
        }];
        [parentView addSubview:hwView_];
        return kResultOk;
    }

    void requestHostResize(int32 w, int32 h) {
        curW_ = std::clamp(w, 800, 2560);
        curH_ = std::clamp(h, 450, 1440);
        if (frame_) {
            ViewRect vr{0, 0, curW_, curH_};
            frame_->resizeView(this, &vr);
        }
        if (hwView_) {
            [hwView_ setFrame:NSMakeRect(0, 0, curW_, curH_)];
            [hwView_ setNeedsDisplay:YES];
        }
    }

    tresult removed() override {
        if (hwView_) {
            [hwView_ removeFromSuperview];
            hwView_ = nil;
        }
        return kResultOk;
    }

    tresult onWheel(float) override { return kResultFalse; }
    tresult onKeyDown(char16, int16, int16) override { return kResultFalse; }
    tresult onKeyUp(char16, int16, int16) override { return kResultFalse; }
    tresult getSize(ViewRect* size) override {
        if (!size) return kInvalidArgument;
        size->left = 0; size->top = 0; size->right = curW_; size->bottom = curH_;
        return kResultOk;
    }
    tresult onSize(ViewRect* newSize) override {
        if (!newSize) return kInvalidArgument;
        curW_ = std::clamp(newSize->right - newSize->left, 800, 2560);
        curH_ = std::clamp(newSize->bottom - newSize->top, 450, 1440);
        if (hwView_) {
            [hwView_ setFrame:NSMakeRect(0, 0, curW_, curH_)];
            [hwView_ setNeedsDisplay:YES];
        }
        return kResultOk;
    }
    tresult onFocus(TBool) override { return kResultOk; }
    tresult setFrame(IPlugFrame* frame) override { frame_ = frame; return kResultOk; }
    tresult canResize() override { return kResultTrue; }
    tresult checkSizeConstraint(ViewRect* rect) override {
        if (!rect) return kInvalidArgument;
        int32 w = std::clamp(rect->right - rect->left, 800, 2560);
        int32 h = static_cast<int32>(std::round(w * 720.0 / 1280.0));
        rect->right = rect->left + w;
        rect->bottom = rect->top + h;
        return kResultOk;
    }

private:
    std::atomic<uint32> refCount_{1};
    vsrm::VSRMasterEngine* engine_ = nullptr;
    IComponentHandler** handlerPtr_ = nullptr;
    IPlugFrame* frame_ = nullptr;
    VSRMasterHardwareView* hwView_ = nil;
    int32 curW_ = 1280;
    int32 curH_ = 720;
};

// ============================================================================
// Combined VST3 Single-Component Effect (IComponent + IAudioProcessor + IEditController)
// ============================================================================
class VSRMasterVST3Effect : public IComponent, public IAudioProcessor, public IEditController {
public:
    VSRMasterVST3Effect() {
        engine_.prepare(48000.0);
    }
    virtual ~VSRMasterVST3Effect() = default;

    // FUnknown
    tresult queryInterface(const TUID _iid, void** obj) override {
        if (iidEqual(_iid, kFUnknownIID) || iidEqual(_iid, kIPluginBaseIID) || iidEqual(_iid, kIComponentIID)) {
            addRef();
            *obj = static_cast<IComponent*>(this);
            return kResultOk;
        }
        if (iidEqual(_iid, kIAudioProcessorIID)) {
            addRef();
            *obj = static_cast<IAudioProcessor*>(this);
            return kResultOk;
        }
        if (iidEqual(_iid, kIEditControllerIID)) {
            addRef();
            *obj = static_cast<IEditController*>(this);
            return kResultOk;
        }
        *obj = nullptr;
        return kNoInterface;
    }
    uint32 addRef() override { return ++refCount_; }
    uint32 release() override {
        uint32 r = --refCount_;
        if (r == 0) delete this;
        return r;
    }

    // IPluginBase
    tresult initialize(FUnknown*) override { return kResultOk; }
    tresult terminate() override { return kResultOk; }

    // IComponent
    tresult getControllerClassId(TUID classId) override {
        std::memcpy(classId, kVSRMasterProcCID, 16);
        return kResultOk;
    }
    tresult setIoMode(int32) override { return kResultOk; }
    int32 getBusCount(int32 type, int32 dir) override {
        if (type == 0 && (dir == 0 || dir == 1)) return 1; // 1 Audio Input, 1 Audio Output
        return 0;
    }
    tresult getBusInfo(int32 type, int32 dir, int32 index, BusInfo& bus) override {
        if (type != 0 || index != 0) return kInvalidArgument;
        bus.mediaType = 0; // kAudio
        bus.direction = dir;
        bus.channelCount = 2;
        str8ToStr16(bus.name, dir == 0 ? "Master Bus In" : "Master Bus Out");
        bus.busType = 0; // kMain
        bus.flags = 1;   // kDefaultActive
        return kResultOk;
    }
    tresult getRoutingInfo(RoutingInfo&, RoutingInfo&) override { return kNotImplemented; }
    tresult activateBus(int32, int32, int32, TBool) override { return kResultOk; }
    tresult setActive(TBool) override { return kResultOk; }

    tresult setState(IBStream* state) override {
        if (!state) return kInvalidArgument;
        for (int i = 0; i < vsrm::kNumParams; ++i) {
            float v = 0.0f;
            int32 br = 0;
            if (state->read(&v, sizeof(float), &br) == kResultOk && br == sizeof(float)) {
                engine_.setParam(i, v);
            }
        }
        return kResultOk;
    }

    tresult getState(IBStream* state) override {
        if (!state) return kInvalidArgument;
        for (int i = 0; i < vsrm::kNumParams; ++i) {
            float v = engine_.getParam(i);
            int32 bw = 0;
            state->write(&v, sizeof(float), &bw);
        }
        return kResultOk;
    }

    // IAudioProcessor
    tresult setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts) override {
        if (numIns == 1 && numOuts == 1 && inputs[0] == 3 && outputs[0] == 3) return kResultTrue;
        return kResultFalse;
    }
    tresult getBusArrangement(int32, int32 index, SpeakerArrangement& arr) override {
        if (index != 0) return kInvalidArgument;
        arr = 3; // SpeakerArr::kStereo (L | R)
        return kResultOk;
    }
    tresult canProcessSampleSize(int32 symbolicSampleSize) override {
        return (symbolicSampleSize == 0) ? kResultTrue : kResultFalse; // 32-bit float
    }
    uint32 getLatencySamples() override { return 0; }
    tresult setupProcessing(ProcessSetup& setup) override {
        engine_.prepare(setup.sampleRate > 0 ? setup.sampleRate : 48000.0);
        return kResultOk;
    }
    tresult setProcessing(TBool) override { return kResultOk; }

    tresult process(ProcessData& data) override {
        if (data.inputParameterChanges) {
            int32 numChanges = data.inputParameterChanges->getParameterCount();
            for (int32 i = 0; i < numChanges; ++i) {
                if (IParamValueQueue* q = data.inputParameterChanges->getParameterData(i)) {
                    int32 pts = q->getPointCount();
                    if (pts > 0) {
                        int32 offset = 0;
                        ParamValue val = 0.0;
                        if (q->getPoint(pts - 1, offset, val) == kResultOk) {
                            engine_.setParamNormalized(static_cast<int>(q->getParameterId()), static_cast<float>(val));
                        }
                    }
                }
            }
        }
        if (data.numSamples <= 0 || data.numOutputs == 0 || !data.outputs) return kResultOk;

        const float* inL = (data.numInputs > 0 && data.inputs && data.inputs[0].numChannels > 0) ? data.inputs[0].channelBuffers32[0] : nullptr;
        const float* inR = (data.numInputs > 0 && data.inputs && data.inputs[0].numChannels > 1) ? data.inputs[0].channelBuffers32[1] : inL;
        float* outL = (data.outputs[0].numChannels > 0) ? data.outputs[0].channelBuffers32[0] : nullptr;
        float* outR = (data.outputs[0].numChannels > 1) ? data.outputs[0].channelBuffers32[1] : nullptr;

        engine_.processBlock(inL, inR, outL, outR, data.numSamples);
        return kResultOk;
    }
    uint32 getTailSamples() override { return 2400; }

    // IEditController
    tresult setComponentState(IBStream* state) override { return setState(state); }
    int32 getParameterCount() override { return vsrm::kNumParams; }
    tresult getParameterInfo(int32 paramIndex, ParameterInfo& info) override {
        if (paramIndex < 0 || paramIndex >= vsrm::kNumParams) return kInvalidArgument;
        const auto& m = vsrm::kParamTable[paramIndex];
        std::memset(&info, 0, sizeof(info));
        info.id = static_cast<ParamID>(paramIndex);
        str8ToStr16(info.title, m.name);
        str8ToStr16(info.shortTitle, m.name);
        str8ToStr16(info.units, m.unit);
        info.stepCount = m.isToggle ? 1 : (paramIndex == vsrm::kTapeMode ? 2 : 0);
        info.defaultNormalizedValue = plainParamToNormalized(info.id, m.defVal);
        info.unitId = 0;
        info.flags = 1; // kCanAutomate
        return kResultOk;
    }
    tresult getParamStringByValue(ParamID id, ParamValue valueNormalized, TChar* string) override {
        if (id >= vsrm::kNumParams || !string) return kInvalidArgument;
        double plain = normalizedParamToPlain(id, valueNormalized);
        char buf[64];
        snprintf(buf, sizeof(buf), "%.2f", plain);
        str8ToStr16(string, buf);
        return kResultOk;
    }
    tresult getParamValueByString(ParamID id, TChar*, ParamValue&) override { return kNotImplemented; }
    ParamValue normalizedParamToPlain(ParamID id, ParamValue valueNormalized) override {
        if (id >= vsrm::kNumParams) return 0.0;
        const auto& m = vsrm::kParamTable[id];
        return m.minVal + std::clamp(valueNormalized, 0.0, 1.0) * (m.maxVal - m.minVal);
    }
    ParamValue plainParamToNormalized(ParamID id, ParamValue plainValue) override {
        if (id >= vsrm::kNumParams) return 0.0;
        const auto& m = vsrm::kParamTable[id];
        if (m.maxVal <= m.minVal) return 0.0;
        return std::clamp((plainValue - m.minVal) / (m.maxVal - m.minVal), 0.0, 1.0);
    }
    ParamValue getParamNormalized(ParamID id) override {
        return engine_.getParamNormalized(static_cast<int>(id));
    }
    tresult setParamNormalized(ParamID id, ParamValue value) override {
        engine_.setParamNormalized(static_cast<int>(id), static_cast<float>(value));
        return kResultOk;
    }
    tresult setComponentHandler(IComponentHandler* handler) override {
        componentHandler_ = handler;
        return kResultOk;
    }
    IPlugView* createView(FIDString name) override {
        if (name && std::strcmp(name, "editor") == 0) {
            return new VSRMasterPlugView(&engine_, &componentHandler_);
        }
        return nullptr;
    }

private:
    std::atomic<uint32> refCount_{1};
    vsrm::VSRMasterEngine engine_;
    IComponentHandler* componentHandler_ = nullptr;
};

// ============================================================================
// VST3 Plugin Factory (IPluginFactory3)
// ============================================================================
struct PFactoryInfo {
    char8 vendor[64];
    char8 url[256];
    char8 email[128];
    int32 flags;
};

struct PClassInfo {
    TUID cid;
    int32 cardinality;
    char8 category[32];
    char8 name[64];
};

struct PClassInfo2 {
    TUID cid;
    int32 cardinality;
    char8 category[32];
    char8 name[64];
    uint32 classFlags;
    char8 subCategories[128];
    char8 vendor[64];
    char8 version[64];
    char8 sdkVersion[64];
};

struct PClassInfoW {
    TUID cid;
    int32 cardinality;
    char8 category[32];
    TChar name[64];
    uint32 classFlags;
    char8 subCategories[128];
    TChar vendor[64];
    TChar version[64];
    TChar sdkVersion[64];
};

class VSRMasterPluginFactory : public FUnknown {
public:
    tresult queryInterface(const TUID _iid, void** obj) override {
        if (iidEqual(_iid, kFUnknownIID) || iidEqual(_iid, kIPluginFactoryIID) ||
            iidEqual(_iid, kIPluginFactory2IID) || iidEqual(_iid, kIPluginFactory3IID)) {
            addRef();
            *obj = this;
            return kResultOk;
        }
        *obj = nullptr;
        return kNoInterface;
    }
    uint32 addRef() override { return ++refCount_; }
    uint32 release() override { return --refCount_; }

    virtual tresult getFactoryInfo(PFactoryInfo* info) {
        if (!info) return kInvalidArgument;
        std::memset(info, 0, sizeof(*info));
        std::strncpy(info->vendor, "Vintage Studio DSP", 63);
        std::strncpy(info->url, "https://beatskillz.com", 255);
        std::strncpy(info->email, "dsp@vintagestudio.audio", 127);
        info->flags = 0x10; // kUnicode
        return kResultOk;
    }
    virtual int32 countClasses() { return 1; }
    virtual tresult getClassInfo(int32 index, PClassInfo* info) {
        if (index != 0 || !info) return kInvalidArgument;
        std::memset(info, 0, sizeof(*info));
        std::memcpy(info->cid, kVSRMasterProcCID, 16);
        info->cardinality = 0x7FFFFFFF;
        std::strncpy(info->category, "Audio Module Class", 31);
        std::strncpy(info->name, "VSR-MASTER Console", 63);
        return kResultOk;
    }
    virtual tresult createInstance(FIDString cid, FIDString _iid, void** obj) {
        if (!cid || !_iid || !obj) return kInvalidArgument;
        if (iidEqual(cid, kVSRMasterProcCID)) {
            auto* effect = new VSRMasterVST3Effect();
            tresult res = effect->queryInterface(_iid, obj);
            effect->release();
            return res;
        }
        *obj = nullptr;
        return kNoInterface;
    }
    virtual tresult getClassInfo2(int32 index, PClassInfo2* info) {
        if (index != 0 || !info) return kInvalidArgument;
        std::memset(info, 0, sizeof(*info));
        std::memcpy(info->cid, kVSRMasterProcCID, 16);
        info->cardinality = 0x7FFFFFFF;
        std::strncpy(info->category, "Audio Module Class", 31);
        std::strncpy(info->name, "VSR-MASTER Console", 63);
        info->classFlags = 1; // kDistributable
        std::strncpy(info->subCategories, "Fx|Mastering|Dynamics|EQ|Spatial", 127);
        std::strncpy(info->vendor, "Vintage Studio DSP", 63);
        std::strncpy(info->version, "1.2.0", 63);
        std::strncpy(info->sdkVersion, "VST 3.7.8", 63);
        return kResultOk;
    }
    virtual tresult getClassInfoUnicode(int32 index, PClassInfoW* info) {
        if (index != 0 || !info) return kInvalidArgument;
        std::memset(info, 0, sizeof(*info));
        std::memcpy(info->cid, kVSRMasterProcCID, 16);
        info->cardinality = 0x7FFFFFFF;
        std::strncpy(info->category, "Audio Module Class", 31);
        str8ToStr16(info->name, "VSR-MASTER Console", 64);
        info->classFlags = 1;
        std::strncpy(info->subCategories, "Fx|Mastering|Dynamics|EQ|Spatial", 127);
        str8ToStr16(info->vendor, "Vintage Studio DSP", 64);
        str8ToStr16(info->version, "1.2.0", 64);
        str8ToStr16(info->sdkVersion, "VST 3.7.8", 64);
        return kResultOk;
    }
    virtual tresult setHostContext(FUnknown*) { return kResultOk; }

private:
    std::atomic<uint32> refCount_{1};
};

} // namespace vst3

extern "C" __attribute__((visibility("default")))
void* GetPluginFactory() {
    static vst3::VSRMasterPluginFactory sFactory;
    sFactory.addRef();
    return &sFactory;
}

extern "C" __attribute__((visibility("default")))
bool bundleEntry(CFBundleRef) { return true; }

extern "C" __attribute__((visibility("default")))
bool bundleExit() { return true; }
