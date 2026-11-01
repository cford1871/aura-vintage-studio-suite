#pragma once
#include <cmath>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <atomic>
#include <array>

namespace vsr {

enum ParamID : int {
    kParamInputGain = 0,     // -12 to +24 dB (RX950 Gain / AK9X InTrim)
    kParamDrive,             // 0 to 100 % (AK9X / S12X Neural/Analog Drive)
    kParamOverdrive,         // 0 or 1 (S12X Overdrive circuit)
    kParamMachineType,       // 0..5: S950, SP1200, S900, MPC60, MuLaw, Paula (SampleX V3 Types A-F)
    kParamBandwidth,         // 1900 to 48000 Hz (RX950 Bandwidth / SampleX Sample Ratio)
    kParamDistruktShift,     // -12 to +12 semitones aliasing shift (AK9X/S12X Distrukt / SampleX Shift)
    kParamBitDepth,          // 4.0 to 16.0 bits (SampleX Bitrate / RX950-1200 12-bit)
    kParamMuLaw,             // 0 or 1 (SampleX MpoweR companding)
    kParamDiskNoise,         // 0 to 100 % (SampleX Disk power / vintage DAC floor)
    kParamLpfCutoff,         // 150 to 20000 Hz (RX950 Filter / SampleX LPF)
    kParamLpfResonance,      // 0 to 95 % (SampleX LPF Res / RX1200 SSM2044)
    kParamLpfMode,           // 0: S950 36dB Butterworth, 1: SP1200 SSM2044 Ladder, 2: RX1200 Dynamic Env LPF
    kParamLpfPower,          // 0 or 1 (SampleX LPF Power)
    kParamHpfCutoff,         // 20 to 2000 Hz (SampleX HPF / RX950 Bessel HPF)
    kParamHpfResonance,      // 0 to 85 % (SampleX HPF Res)
    kParamHpfPower,          // 0 or 1 (SampleX HPF Power)
    kParamBrilliance,        // -12 to +12 dB (RX950 / RX1200 Butterworth HighShelf<1>)
    kParamCompThreshold,     // -40 to 0 dB (MAX1 Threshold)
    kParamCompAttack,        // 0.5 to 100 ms (MAX1 Attack)
    kParamCompRelease,       // 10 to 1000 ms (MAX1 Release)
    kParamCompRatio,         // 1.0 to 20.0 (MAX1 Ratio)
    kParamCompAutoRelease,   // 0 or 1 (MAX1 AutoRelease)
    kParamCompFeedback,      // 0 or 1 (MAX1 Feedback Mode)
    kParamFatten,            // 0 to 100 % (MAX1 Fatten)
    kParamSoftClip,          // 0 or 1 (MAX1 Soft Clip)
    kParamStereoWidth,       // 0 to 200 % (SampleX WidthKnob)
    kParamMono,              // 0 or 1 (RX950 / AK9X Mono)
    kParamAutoGain,          // 0 or 1 (AK9X / S12X AutoGain)
    kParamDryWetMix,         // 0 to 100 % (AK9X / S12X / SampleX Mix)
    kParamOutputLevel,       // -24 to +12 dB (RX950 Level / OutTrim)
    kNumParams
};

struct ParamMeta {
    int id;
    const char* name;
    const char* shortName;
    const char* unit;
    float minVal;
    float maxVal;
    float defaultVal;
    bool isBoolean;
    bool isStepped;
};

static const ParamMeta kParamTable[kNumParams] = {
    { kParamInputGain,       "Input Gain",       "IN GAIN",   "dB",  -12.0f,   24.0f,    0.0f,  false, false },
    { kParamDrive,           "Analog Drive",     "DRIVE",     "%",     0.0f,  100.0f,   25.0f,  false, false },
    { kParamOverdrive,       "SP Overdrive",     "OVDRIVE",   "",      0.0f,    1.0f,    0.0f,  true,  true  },
    { kParamMachineType,     "Sampler Model",    "MODEL",     "",      0.0f,    5.0f,    0.0f,  false, true  },
    { kParamBandwidth,       "Audio Bandwidth",  "BANDWIDTH", "Hz", 1900.0f, 48000.0f, 26040.0f,false, false },
    { kParamDistruktShift,   "Distrukt Alias",   "DISTRUKT",  "st",  -12.0f,   12.0f,    0.0f,  false, false },
    { kParamBitDepth,        "Bit Depth",        "BITS",      "bit",   4.0f,   16.0f,   12.0f,  false, false },
    { kParamMuLaw,           "MpoweR Mu-Law",    "MU-LAW",    "",      0.0f,    1.0f,    1.0f,  true,  true  },
    { kParamDiskNoise,       "Disk & DAC Floor", "DISK",      "%",     0.0f,  100.0f,    8.0f,  false, false },
    { kParamLpfCutoff,       "LPF Cutoff",       "LPF FREQ",  "Hz",  150.0f, 20000.0f, 12500.0f,false, false },
    { kParamLpfResonance,    "LPF Resonance",    "LPF RES",   "%",     0.0f,   95.0f,   18.0f,  false, false },
    { kParamLpfMode,         "Filter Topology",  "LPF MODE",  "",      0.0f,    2.0f,    0.0f,  false, true  },
    { kParamLpfPower,        "LPF Enable",       "LPF ON",    "",      0.0f,    1.0f,    1.0f,  true,  true  },
    { kParamHpfCutoff,       "HPF Cutoff",       "HPF FREQ",  "Hz",   20.0f,  2000.0f,   35.0f,  false, false },
    { kParamHpfResonance,    "HPF Resonance",    "HPF RES",   "%",     0.0f,   85.0f,   10.0f,  false, false },
    { kParamHpfPower,        "HPF Enable",       "HPF ON",    "",      0.0f,    1.0f,    1.0f,  true,  true  },
    { kParamBrilliance,      "DAC Brilliance",   "BRILLIANCE","dB",  -12.0f,   12.0f,    2.0f,  false, false },
    { kParamCompThreshold,   "MAX1 Threshold",   "THRESH",    "dB",  -40.0f,    0.0f,  -14.0f,  false, false },
    { kParamCompAttack,      "MAX1 Attack",      "ATTACK",    "ms",    0.5f,  100.0f,   12.0f,  false, false },
    { kParamCompRelease,     "MAX1 Release",     "RELEASE",   "ms",   10.0f, 1000.0f,  140.0f,  false, false },
    { kParamCompRatio,       "MAX1 Ratio",       "RATIO",     ":1",    1.0f,   20.0f,    3.5f,  false, false },
    { kParamCompAutoRelease, "Auto Release",     "AUTO REL",  "",      0.0f,    1.0f,    1.0f,  true,  true  },
    { kParamCompFeedback,    "Feedback Comp",    "FEEDBACK",  "",      0.0f,    1.0f,    1.0f,  true,  true  },
    { kParamFatten,          "MAX1 Fatten",      "FATTEN",    "%",     0.0f,  100.0f,   30.0f,  false, false },
    { kParamSoftClip,        "Soft Clipper",     "SOFT CLIP", "",      0.0f,    1.0f,    1.0f,  true,  true  },
    { kParamStereoWidth,     "Stereo Width",     "WIDTH",     "%",     0.0f,  200.0f,  100.0f,  false, false },
    { kParamMono,            "Mono Collapse",    "MONO",      "",      0.0f,    1.0f,    0.0f,  true,  true  },
    { kParamAutoGain,        "Auto Gain",        "AUTO GAIN", "",      0.0f,    1.0f,    1.0f,  true,  true  },
    { kParamDryWetMix,       "Dry / Wet Mix",    "MIX",       "%",     0.0f,  100.0f,  100.0f,  false, false },
    { kParamOutputLevel,     "Output Level",     "OUTPUT",    "dB",  -24.0f,   12.0f,    0.0f,  false, false }
};

struct PresetInfo {
    const char* name;
    float values[kNumParams];
};

static const PresetInfo kFactoryPresets[] = {
    { "01. Classic S950 + SP1200 Hybrid",
      { 2.0f, 28.0f, 0.0f, 0.0f, 26040.0f, 0.0f, 12.0f, 1.0f, 8.0f, 12500.0f, 18.0f, 0.0f, 1.0f, 35.0f, 10.0f, 1.0f, 2.5f, -14.0f, 12.0f, 140.0f, 3.5f, 1.0f, 1.0f, 30.0f, 1.0f, 100.0f, 0.0f, 1.0f, 100.0f, 0.0f } },
    { "02. Boom-Bap SP-1200 Grit Ring",
      { 6.5f, 52.0f, 1.0f, 1.0f, 26040.0f, -5.0f, 12.0f, 1.0f, 18.0f, 7800.0f, 48.0f, 1.0f, 1.0f, 55.0f, 28.0f, 1.0f, 3.5f, -18.0f, 8.0f, 95.0f, 5.0f, 1.0f, 1.0f, 45.0f, 1.0f, 0.0f, 1.0f, 1.0f, 100.0f, 0.0f } },
    { "03. Inphonik RX950 Warm Filter Sweep",
      { 4.5f, 22.0f, 0.0f, 0.0f, 19200.0f, 0.0f, 12.0f, 0.0f, 5.0f, 4200.0f, 10.0f, 0.0f, 1.0f, 30.0f, 5.0f, 1.0f, 4.5f, -12.0f, 18.0f, 180.0f, 2.5f, 1.0f, 0.0f, 25.0f, 1.0f, 100.0f, 0.0f, 1.0f, 100.0f, 0.0f } },
    { "04. SampleX V3 MpoweR 8-Bit Crunch",
      { 5.0f, 60.0f, 1.0f, 4.0f, 14500.0f, 3.5f, 8.0f, 1.0f, 24.0f, 6500.0f, 55.0f, 1.0f, 1.0f, 80.0f, 35.0f, 1.0f, 1.5f, -16.0f, 5.0f, 110.0f, 4.5f, 1.0f, 1.0f, 35.0f, 1.0f, 125.0f, 0.0f, 1.0f, 90.0f, 0.0f } },
    { "05. MAX1 Drum Bus Slam & Fatten",
      { 3.0f, 35.0f, 0.0f, 3.0f, 40000.0f, 0.0f, 12.0f, 1.0f, 4.0f, 16500.0f, 22.0f, 2.0f, 1.0f, 42.0f, 20.0f, 1.0f, 3.0f, -22.0f, 6.0f, 80.0f, 8.0f, 1.0f, 1.0f, 68.0f, 1.0f, 115.0f, 0.0f, 1.0f, 85.0f, 0.5f } },
    { "06. RX1200 Dynamic SSM2044 Pluck",
      { 4.0f, 40.0f, 0.0f, 1.0f, 26040.0f, -2.0f, 12.0f, 1.0f, 12.0f, 2400.0f, 65.0f, 2.0f, 1.0f, 45.0f, 15.0f, 1.0f, 4.0f, -15.0f, 10.0f, 120.0f, 4.0f, 1.0f, 1.0f, 40.0f, 1.0f, 100.0f, 0.0f, 1.0f, 100.0f, 0.0f } },
    { "07. Amiga Paula 28kHz Lo-Fi Wide",
      { 3.5f, 45.0f, 0.0f, 5.0f, 28000.0f, -4.0f, 8.5f, 0.0f, 15.0f, 5200.0f, 30.0f, 0.0f, 1.0f, 65.0f, 18.0f, 1.0f, 0.0f, -14.0f, 15.0f, 160.0f, 3.0f, 1.0f, 0.0f, 28.0f, 1.0f, 160.0f, 0.0f, 1.0f, 100.0f, 0.0f } },
    { "08. Clean Studio DAC Brilliance",
      { 0.0f, 12.0f, 0.0f, 3.0f, 44100.0f, 0.0f, 15.0f, 0.0f, 2.0f, 18500.0f, 8.0f, 0.0f, 1.0f, 25.0f, 5.0f, 1.0f, 5.5f, -10.0f, 20.0f, 200.0f, 2.0f, 1.0f, 0.0f, 18.0f, 1.0f, 110.0f, 0.0f, 1.0f, 100.0f, 0.0f } }
};
static constexpr int kNumPresets = sizeof(kFactoryPresets) / sizeof(kFactoryPresets[0]);

struct Biquad {
    double b0 = 1.0, b1 = 0.0, b2 = 0.0, a1 = 0.0, a2 = 0.0;
    double z1[2] = {0.0, 0.0};
    double z2[2] = {0.0, 0.0};

    void reset() {
        z1[0] = z1[1] = z2[0] = z2[1] = 0.0;
    }

    inline float process(float x, int ch) {
        double in = static_cast<double>(x);
        double out = b0 * in + z1[ch];
        z1[ch] = b1 * in - a1 * out + z2[ch];
        z2[ch] = b2 * in - a2 * out;
        return static_cast<float>(out);
    }

    void setLowPass(double sr, double freq, double Q) {
        freq = std::clamp(freq, 20.0, sr * 0.48);
        Q = std::max(0.1, Q);
        double w0 = 2.0 * M_PI * freq / sr;
        double cosw = std::cos(w0);
        double alpha = std::sin(w0) / (2.0 * Q);
        double a0 = 1.0 + alpha;
        b0 = ((1.0 - cosw) * 0.5) / a0;
        b1 = (1.0 - cosw) / a0;
        b2 = ((1.0 - cosw) * 0.5) / a0;
        a1 = (-2.0 * cosw) / a0;
        a2 = (1.0 - alpha) / a0;
    }

    void setHighPass(double sr, double freq, double Q) {
        freq = std::clamp(freq, 15.0, sr * 0.45);
        Q = std::max(0.1, Q);
        double w0 = 2.0 * M_PI * freq / sr;
        double cosw = std::cos(w0);
        double alpha = std::sin(w0) / (2.0 * Q);
        double a0 = 1.0 + alpha;
        b0 = ((1.0 + cosw) * 0.5) / a0;
        b1 = -(1.0 + cosw) / a0;
        b2 = ((1.0 + cosw) * 0.5) / a0;
        a1 = (-2.0 * cosw) / a0;
        a2 = (1.0 - alpha) / a0;
    }

    void setHighShelf(double sr, double freq, double gainDb) {
        freq = std::clamp(freq, 200.0, sr * 0.46);
        double A = std::pow(10.0, gainDb / 40.0);
        double w0 = 2.0 * M_PI * freq / sr;
        double cosw = std::cos(w0);
        double sinw = std::sin(w0);
        double alpha = sinw * 0.5 * std::sqrt(2.0);
        double sqrtA2alpha = 2.0 * std::sqrt(A) * alpha;
        double a0 = (A + 1.0) - (A - 1.0) * cosw + sqrtA2alpha;
        b0 = (A * ((A + 1.0) + (A - 1.0) * cosw + sqrtA2alpha)) / a0;
        b1 = (-2.0 * A * ((A - 1.0) + (A + 1.0) * cosw)) / a0;
        b2 = (A * ((A + 1.0) + (A - 1.0) * cosw - sqrtA2alpha)) / a0;
        a1 = (2.0 * ((A - 1.0) - (A + 1.0) * cosw)) / a0;
        a2 = ((A + 1.0) - (A - 1.0) * cosw - sqrtA2alpha) / a0;
    }

    void setLowShelf(double sr, double freq, double gainDb) {
        freq = std::clamp(freq, 30.0, sr * 0.45);
        double A = std::pow(10.0, gainDb / 40.0);
        double w0 = 2.0 * M_PI * freq / sr;
        double cosw = std::cos(w0);
        double sinw = std::sin(w0);
        double alpha = sinw * 0.5 * std::sqrt(2.0);
        double sqrtA2alpha = 2.0 * std::sqrt(A) * alpha;
        double a0 = (A + 1.0) + (A - 1.0) * cosw + sqrtA2alpha;
        b0 = (A * ((A + 1.0) - (A - 1.0) * cosw + sqrtA2alpha)) / a0;
        b1 = (2.0 * A * ((A - 1.0) - (A + 1.0) * cosw)) / a0;
        b2 = (A * ((A + 1.0) - (A - 1.0) * cosw - sqrtA2alpha)) / a0;
        a1 = (-2.0 * ((A - 1.0) + (A + 1.0) * cosw)) / a0;
        a2 = ((A + 1.0) + (A - 1.0) * cosw - sqrtA2alpha) / a0;
    }
};

// SSM2044 4-Pole Nonlinear Resonant Ladder Filter (SP-1200 / RX1200 / SampleX V3)
struct SSM2044LadderFilter {
    double stage[2][4] = {{0.0}};
    void reset() { std::memset(stage, 0, sizeof(stage)); }

    inline float process(float in, int ch, double sr, double cutoffHz, double resonance01) {
        cutoffHz = std::clamp(cutoffHz, 80.0, sr * 0.45);
        double fc = cutoffHz / sr;
        double f = std::clamp(2.0 * std::sin(M_PI * fc), 0.001, 0.92);
        double k = 3.85 * std::clamp(resonance01, 0.0, 0.96);
        double x = static_cast<double>(in) - k * std::tanh(stage[ch][3]);
        for (int i = 0; i < 4; ++i) {
            double prev = (i == 0) ? std::tanh(x) : stage[ch][i - 1];
            stage[ch][i] += f * (prev - stage[ch][i]);
        }
        return static_cast<float>(stage[ch][3]);
    }
};

// Built-in 8-Pad RX1200 Sample/Drum Trigger Voice
struct PadVoice {
    bool active = false;
    int type = 0; // 0: Kick, 1: Snare, 2: Closed Hat, 3: Open Hat, 4: Rim/Clap, 5: Tom, 6: Chord Hit, 7: Sub Drop
    double phase = 0.0;
    double env = 0.0;
    double timeSec = 0.0;
    float pitchSemitones = 0.0f;
    float decayScale = 1.0f;
    float level = 0.85f;
    float pan = 0.0f;
};

class VSR9512XEngine {
public:
    VSR9512XEngine() {
        for (int i = 0; i < kNumParams; ++i) {
            params_[i].store(kParamTable[i].defaultVal, std::memory_order_relaxed);
        }
        std::memset(scopeRing_, 0, sizeof(scopeRing_));
        std::memset(grHistory_, 0, sizeof(grHistory_));
        initPads();
        updateFilters();
    }

    void prepare(double sampleRate) {
        if (sampleRate > 8000.0) sampleRate_ = sampleRate;
        for (auto& b : s950Butterworth_) b.reset();
        hpfBiquad_.reset();
        brillianceShelf_.reset();
        fattenShelf_.reset();
        ssmLadder_.reset();
        compEnv_ = 0.0f;
        dynFilterEnv_ = 0.0f;
        prevOut_[0] = prevOut_[1] = 0.0f;
        resampPhase_[0] = resampPhase_[1] = 0.0;
        heldSample_[0] = heldSample_[1] = 0.0f;
        updateFilters();
    }

    void setParam(int id, float val) {
        if (id < 0 || id >= kNumParams) return;
        val = std::clamp(val, kParamTable[id].minVal, kParamTable[id].maxVal);
        params_[id].store(val, std::memory_order_relaxed);
    }

    float getParam(int id) const {
        if (id < 0 || id >= kNumParams) return 0.0f;
        return params_[id].load(std::memory_order_relaxed);
    }

    void setParamNormalized(int id, float norm) {
        if (id < 0 || id >= kNumParams) return;
        norm = std::clamp(norm, 0.0f, 1.0f);
        const auto& m = kParamTable[id];
        float v = m.minVal + norm * (m.maxVal - m.minVal);
        if (m.isStepped) v = std::round(v);
        setParam(id, v);
    }

    float getParamNormalized(int id) const {
        if (id < 0 || id >= kNumParams) return 0.0f;
        const auto& m = kParamTable[id];
        if (m.maxVal <= m.minVal) return 0.0f;
        return std::clamp((getParam(id) - m.minVal) / (m.maxVal - m.minVal), 0.0f, 1.0f);
    }

    void loadPreset(int presetIdx) {
        if (presetIdx < 0 || presetIdx >= kNumPresets) return;
        currentPreset_.store(presetIdx, std::memory_order_relaxed);
        for (int i = 0; i < kNumParams; ++i) {
            setParam(i, kFactoryPresets[presetIdx].values[i]);
        }
    }

    int getCurrentPreset() const { return currentPreset_.load(std::memory_order_relaxed); }

    void triggerPad(int padIdx) {
        if (padIdx < 0 || padIdx >= 8) return;
        pads_[padIdx].active = true;
        pads_[padIdx].phase = 0.0;
        pads_[padIdx].timeSec = 0.0;
        pads_[padIdx].env = 1.0;
    }

    void setDemoLoopPlaying(bool play) { demoLoopPlaying_.store(play, std::memory_order_relaxed); }
    bool isDemoLoopPlaying() const { return demoLoopPlaying_.load(std::memory_order_relaxed); }

    void setSkin(int idx) { skinIndex_.store((idx % 5 + 5) % 5, std::memory_order_relaxed); }
    int getSkin() const { return skinIndex_.load(std::memory_order_relaxed); }

    // Telemetry getters for UI meters & oscilloscope
    float getInputPeakL() const { return inPeakL_.load(std::memory_order_relaxed); }
    float getInputPeakR() const { return inPeakR_.load(std::memory_order_relaxed); }
    float getOutputPeakL() const { return outPeakL_.load(std::memory_order_relaxed); }
    float getOutputPeakR() const { return outPeakR_.load(std::memory_order_relaxed); }
    float getGainReductionDb() const { return grDb_.load(std::memory_order_relaxed); }

    void copyScopeData(float* dstScope256, float* dstGr128) const {
        uint32_t w = scopeWriteIdx_.load(std::memory_order_relaxed);
        for (int i = 0; i < 256; ++i) {
            dstScope256[i] = scopeRing_[(w + i) & 255];
        }
        uint32_t gw = grWriteIdx_.load(std::memory_order_relaxed);
        for (int i = 0; i < 128; ++i) {
            dstGr128[i] = grHistory_[(gw + i) & 127];
        }
    }

    // μ-Law companding (SampleX MpoweR / Vintage 12-bit & 8-bit DACs)
    static inline float muLawEncode(float x, float mu = 255.0f) {
        x = std::clamp(x, -1.0f, 1.0f);
        float s = (x >= 0.0f) ? 1.0f : -1.0f;
        return s * std::log(1.0f + mu * std::abs(x)) / std::log(1.0f + mu);
    }

    static inline float muLawDecode(float y, float mu = 255.0f) {
        y = std::clamp(y, -1.0f, 1.0f);
        float s = (y >= 0.0f) ? 1.0f : -1.0f;
        return s * (1.0f / mu) * (std::pow(1.0f + mu, std::abs(y)) - 1.0f);
    }

    void processBlock(const float* inL, const float* inR, float* outL, float* outR, int numSamples) {
        updateFilters();

        const float inGainLinear = std::pow(10.0f, getParam(kParamInputGain) / 20.0f);
        const float drive01 = getParam(kParamDrive) * 0.01f;
        const bool overdriveOn = getParam(kParamOverdrive) >= 0.5f;
        const int machine = static_cast<int>( std::round(getParam(kParamMachineType)) );
        const float bandwidthHz = getParam(kParamBandwidth);
        const float shiftSt = getParam(kParamDistruktShift);
        const float bits = getParam(kParamBitDepth);
        const bool muLawOn = getParam(kParamMuLaw) >= 0.5f;
        const float diskNoise01 = getParam(kParamDiskNoise) * 0.01f;
        const float lpfCutoff = getParam(kParamLpfCutoff);
        const float lpfRes01 = getParam(kParamLpfResonance) * 0.01f;
        const int lpfMode = static_cast<int>( std::round(getParam(kParamLpfMode)) );
        const bool lpfOn = getParam(kParamLpfPower) >= 0.5f;
        const bool hpfOn = getParam(kParamHpfPower) >= 0.5f;
        const float threshDb = getParam(kParamCompThreshold);
        const float attackMs = std::max(0.5f, getParam(kParamCompAttack));
        const float releaseMs = std::max(10.0f, getParam(kParamCompRelease));
        const float ratio = std::max(1.0f, getParam(kParamCompRatio));
        const bool autoRel = getParam(kParamCompAutoRelease) >= 0.5f;
        const bool feedbackComp = getParam(kParamCompFeedback) >= 0.5f;
        const float fatten01 = getParam(kParamFatten) * 0.01f;
        const bool softClipOn = getParam(kParamSoftClip) >= 0.5f;
        const float width = getParam(kParamStereoWidth) * 0.01f;
        const bool monoOn = getParam(kParamMono) >= 0.5f;
        const bool autoGainOn = getParam(kParamAutoGain) >= 0.5f;
        const float wetMix = getParam(kParamDryWetMix) * 0.01f;
        const float outLevelLinear = std::pow(10.0f, getParam(kParamOutputLevel) / 20.0f);

        // Machine-specific clock and bit bias (SampleX V3 Types A..F)
        double effectiveClock = static_cast<double>(bandwidthHz);
        float effectiveBits = bits;
        if (machine == 1) { // SP-1200
            effectiveClock = std::min(effectiveClock, 26040.0);
        } else if (machine == 2) { // S900
            effectiveClock = std::min(effectiveClock, 32000.0);
        } else if (machine == 4) { // Mu-Law 8-Bit
            effectiveBits = std::min(effectiveBits, 9.0f);
        } else if (machine == 5) { // Amiga Paula
            effectiveClock = std::min(effectiveClock, 28867.0);
            effectiveBits = std::min(effectiveBits, 8.5f);
        }

        // Distrukt pitch-shift aliasing factor
        double shiftFactor = std::pow(2.0, static_cast<double>(shiftSt) / 12.0);
        double clockRatio = std::clamp((effectiveClock * shiftFactor) / sampleRate_, 0.02, 1.0);
        float quantSteps = std::pow(2.0f, std::clamp(effectiveBits, 4.0f, 16.0f));

        // Compressor coefficients
        float attackCoef = std::exp(-1.0f / (0.001f * attackMs * static_cast<float>(sampleRate_)));
        float baseReleaseCoef = std::exp(-1.0f / (0.001f * releaseMs * static_cast<float>(sampleRate_)));

        // Auto-gain compensation estimate
        float autoGainComp = 1.0f;
        if (autoGainOn) {
            float driveDbComp = -drive01 * 4.5f - (overdriveOn ? 4.0f : 0.0f) - (getParam(kParamInputGain) * 0.45f);
            float makeupDb = (threshDb < -4.0f && ratio > 1.1f) ? (-threshDb * (1.0f - 1.0f / ratio) * 0.45f) : 0.0f;
            autoGainComp = std::pow(10.0f, (driveDbComp + makeupDb) / 20.0f);
        }

        float blockInPeakL = 0.0f, blockInPeakR = 0.0f;
        float blockOutPeakL = 0.0f, blockOutPeakR = 0.0f;
        float lastGrDb = 0.0f;

        for (int i = 0; i < numSamples; ++i) {
            float dryL = inL ? inL[i] : 0.0f;
            float dryR = inR ? inR[i] : dryL;

            // Add built-in demo groove / 8-pad RX1200 sampler voices
            float padL = 0.0f, padR = 0.0f;
            renderPadsAndDemo(padL, padR);
            dryL += padL;
            dryR += padR;

            blockInPeakL = std::max(blockInPeakL, std::abs(dryL));
            blockInPeakR = std::max(blockInPeakR, std::abs(dryR));

            float s[2] = { dryL * inGainLinear, dryR * inGainLinear };

            // Stage 1: Preamp & Neural/Analog Harmonic Saturation (AK9X / S12X / RX950)
            float preGain = 1.0f + drive01 * 5.5f + (overdriveOn ? 3.5f : 0.0f);
            for (int ch = 0; ch < 2; ++ch) {
                float x = s[ch] * preGain;
                // Machine-specific asymmetric transformer/converter saturation
                if (machine == 0 || machine == 2) {
                    // Akai S950 / S900 warm asymmetric JFET + converter clip
                    float asym = 0.08f * drive01 * x * x;
                    s[ch] = std::tanh(x + asym) / std::tanh(std::max(1.0f, preGain * 0.65f));
                } else if (machine == 1) {
                    // SP-1200 hard input op-amp / SSM saturation
                    float hard = std::clamp(x, -1.35f, 1.35f);
                    s[ch] = std::tanh(1.4f * hard - 0.15f * hard * hard * hard);
                } else {
                    s[ch] = std::tanh(x);
                }

                // Subtle Floppy Disk / Vintage 12-bit DAC noise floor (SampleX Disk Power)
                if (diskNoise01 > 0.001f) {
                    rngState_ = rngState_ * 1664525u + 1013904223u;
                    float wn = (static_cast<float>(rngState_ & 0xFFFF) / 32768.0f) - 1.0f;
                    // Gate noise slightly by signal + motor hum
                    float gate = std::min(1.0f, std::abs(s[ch]) * 6.0f + 0.12f);
                    s[ch] += wn * diskNoise01 * 0.0045f * gate;
                }
            }

            // Stage 2: Variable Clock Zero-Order-Hold Resampler & Bit Quantizer (RX950 / RX1200 / SampleX V3)
            for (int ch = 0; ch < 2; ++ch) {
                resampPhase_[ch] += clockRatio;
                if (resampPhase_[ch] >= 1.0) {
                    resampPhase_[ch] -= std::floor(resampPhase_[ch]);
                    float q = s[ch];
                    if (muLawOn || machine == 4) {
                        q = muLawEncode(q, 255.0f);
                        q = std::round(q * (quantSteps * 0.5f)) / (quantSteps * 0.5f);
                        q = muLawDecode(q, 255.0f);
                    } else {
                        q = std::round(q * (quantSteps * 0.5f)) / (quantSteps * 0.5f);
                    }
                    heldSample_[ch] = std::clamp(q, -1.0f, 1.0f);
                }
                s[ch] = heldSample_[ch];
            }

            // Track envelope for RX1200 Dynamic Filter mode
            float absPeak = 0.5f * (std::abs(s[0]) + std::abs(s[1]));
            if (absPeak > dynFilterEnv_) {
                dynFilterEnv_ = 0.992f * dynFilterEnv_ + 0.008f * absPeak;
            } else {
                dynFilterEnv_ = 0.9994f * dynFilterEnv_ + 0.0006f * absPeak;
            }

            // Stage 3: Dual Vintage Filter Bank (RX950 6th-order Butterworth / RX1200 SSM2044 Ladder + HPF + Brilliance)
            if (hpfOn) {
                s[0] = hpfBiquad_.process(s[0], 0);
                s[1] = hpfBiquad_.process(s[1], 1);
            }

            if (lpfOn) {
                if (lpfMode == 0 && lpfRes01 < 0.25f) {
                    // RX950 Steep 6th-order Butterworth (3 cascaded biquads = 36 dB/oct)
                    for (int st = 0; st < 3; ++st) {
                        s[0] = s950Butterworth_[st].process(s[0], 0);
                        s[1] = s950Butterworth_[st].process(s[1], 1);
                    }
                } else {
                    // SP-1200 SSM2044 4-Pole Resonant Ladder (Mode 1: Static, Mode 2: RX1200 Dynamic Env)
                    double modCutoff = static_cast<double>(lpfCutoff);
                    if (lpfMode == 2) {
                        modCutoff = std::clamp(modCutoff * (1.0 + 3.5 * dynFilterEnv_), 150.0, sampleRate_ * 0.45);
                    }
                    s[0] = ssmLadder_.process(s[0], 0, sampleRate_, modCutoff, lpfRes01);
                    s[1] = ssmLadder_.process(s[1], 1, sampleRate_, modCutoff, lpfRes01);
                }
            }

            // RX950 / RX1200 1st-Order Butterworth Brilliance High-Shelf
            s[0] = brillianceShelf_.process(s[0], 0);
            s[1] = brillianceShelf_.process(s[1], 1);

            // Stage 4: MAX1 Intelligent Compressor, Fattener & Soft Clipper
            float detL = feedbackComp ? prevOut_[0] : s[0];
            float detR = feedbackComp ? prevOut_[1] : s[1];
            float detPeak = std::max(std::abs(detL), std::abs(detR));
            if (detPeak > compEnv_) {
                compEnv_ = attackCoef * compEnv_ + (1.0f - attackCoef) * detPeak;
            } else {
                // Auto-release speeds up recovery after fast transients
                float relCoef = autoRel
                    ? std::clamp(baseReleaseCoef * (1.0f - 0.15f * std::min(1.0f, compEnv_)), 0.90f, 0.99995f)
                    : baseReleaseCoef;
                compEnv_ = relCoef * compEnv_ + (1.0f - relCoef) * detPeak;
            }

            float envDb = 20.0f * std::log10(std::max(compEnv_, 1e-6f));
            float gainReductionDb = 0.0f;
            float kneeWidth = 6.0f;
            if (envDb > (threshDb - kneeWidth * 0.5f)) {
                if (envDb < (threshDb + kneeWidth * 0.5f)) {
                    float d = envDb - threshDb + kneeWidth * 0.5f;
                    gainReductionDb = ((1.0f / ratio - 1.0f) * d * d) / (2.0f * kneeWidth);
                } else {
                    gainReductionDb = (threshDb - envDb) * (1.0f - 1.0f / ratio);
                }
            }
            lastGrDb = -gainReductionDb;
            float compGain = std::pow(10.0f, gainReductionDb / 20.0f);
            s[0] *= compGain;
            s[1] *= compGain;

            // MAX1 Fatten (Low-mid warmth + 2nd harmonic punch)
            if (fatten01 > 0.001f) {
                s[0] = fattenShelf_.process(s[0], 0);
                s[1] = fattenShelf_.process(s[1], 1);
                s[0] += 0.12f * fatten01 * (s[0] * s[0] * (s[0] >= 0.0f ? 1.0f : -0.6f));
                s[1] += 0.12f * fatten01 * (s[1] * s[1] * (s[1] >= 0.0f ? 1.0f : -0.6f));
            }

            prevOut_[0] = s[0];
            prevOut_[1] = s[1];

            // Stereo Width (SampleX WidthKnob) & Mono Switch (RX950 / AK9X)
            float mid = 0.5f * (s[0] + s[1]);
            float side = 0.5f * (s[0] - s[1]) * (monoOn ? 0.0f : width);
            s[0] = mid + side;
            s[1] = mid - side;

            // Apply Auto-Gain & Output Level
            s[0] *= autoGainComp * outLevelLinear;
            s[1] *= autoGainComp * outLevelLinear;

            // MAX1 Soft Clipper
            if (softClipOn) {
                s[0] = std::tanh(s[0] * 1.1f) / std::tanh(1.1f);
                s[1] = std::tanh(s[1] * 1.1f) / std::tanh(1.1f);
            }

            // Dry / Wet Mix
            float finalL = dryL * (1.0f - wetMix) + s[0] * wetMix;
            float finalR = dryR * (1.0f - wetMix) + s[1] * wetMix;

            if (outL) outL[i] = finalL;
            if (outR) outR[i] = finalR;

            blockOutPeakL = std::max(blockOutPeakL, std::abs(finalL));
            blockOutPeakR = std::max(blockOutPeakR, std::abs(finalR));

            // Push to oscilloscope ring buffer every 8 samples
            if ((sampleCounter_++ & 7) == 0) {
                uint32_t idx = scopeWriteIdx_.load(std::memory_order_relaxed);
                scopeRing_[idx & 255] = 0.5f * (finalL + finalR);
                scopeWriteIdx_.store(idx + 1, std::memory_order_relaxed);
            }
        }

        // Smooth meter decay
        inPeakL_.store(std::max(blockInPeakL, inPeakL_.load(std::memory_order_relaxed) * 0.86f), std::memory_order_relaxed);
        inPeakR_.store(std::max(blockInPeakR, inPeakR_.load(std::memory_order_relaxed) * 0.86f), std::memory_order_relaxed);
        outPeakL_.store(std::max(blockOutPeakL, outPeakL_.load(std::memory_order_relaxed) * 0.86f), std::memory_order_relaxed);
        outPeakR_.store(std::max(blockOutPeakR, outPeakR_.load(std::memory_order_relaxed) * 0.86f), std::memory_order_relaxed);
        grDb_.store(lastGrDb, std::memory_order_relaxed);

        uint32_t gIdx = grWriteIdx_.load(std::memory_order_relaxed);
        grHistory_[gIdx & 127] = lastGrDb;
        grWriteIdx_.store(gIdx + 1, std::memory_order_relaxed);
    }

private:
    void initPads() {
        const float pans[8] = { 0.0f, 0.0f, -0.25f, 0.25f, -0.15f, 0.20f, -0.30f, 0.10f };
        for (int i = 0; i < 8; ++i) {
            pads_[i].type = i;
            pads_[i].pan = pans[i];
        }
    }

    void renderPadsAndDemo(float& outL, float& outR) {
        double dt = 1.0 / sampleRate_;
        // Optional built-in 92 BPM Boom-Bap Demo Loop sequencer
        if (demoLoopPlaying_.load(std::memory_order_relaxed)) {
            demoTimeSec_ += dt;
            double stepDur = (60.0 / 92.0) * 0.25; // 16th note at 92 BPM
            if (demoTimeSec_ >= stepDur) {
                demoTimeSec_ -= stepDur;
                int step = (demoStep_++) & 15;
                if (step == 0 || step == 7 || step == 10) triggerPad(0); // Kick
                if (step == 4 || step == 12) triggerPad(1);              // Snare
                if (step % 2 == 0) triggerPad(2);                        // Closed Hat
                if (step == 14) triggerPad(3);                           // Open Hat
                if (step == 0 || step == 6 || step == 12) triggerPad(6); // Dusty Jazz Chord stab
                if (step == 15) triggerPad(4);                           // Rimshot
            }
        }

        for (int p = 0; p < 8; ++p) {
            auto& v = pads_[p];
            if (!v.active) continue;
            float sig = 0.0f;
            double t = v.timeSec;
            noiseRng_ = noiseRng_ * 1664525u + 1013904223u;
            float n = (static_cast<float>(noiseRng_ & 0xFFFF) / 32768.0f) - 1.0f;

            switch (v.type) {
                case 0: { // SP-1200 Punchy Kick
                    double freq = 52.0 + 135.0 * std::exp(-t * 38.0);
                    v.phase += freq * dt;
                    sig = static_cast<float>(std::sin(2.0 * M_PI * v.phase)) * static_cast<float>(std::exp(-t * 7.5));
                    if (t > 0.45) v.active = false;
                    break;
                }
                case 1: { // Crispy 90s Snare
                    double freq = 185.0 + 90.0 * std::exp(-t * 45.0);
                    v.phase += freq * dt;
                    float body = static_cast<float>(std::sin(2.0 * M_PI * v.phase)) * static_cast<float>(std::exp(-t * 18.0));
                    float wires = n * static_cast<float>(std::exp(-t * 11.5));
                    sig = 0.55f * body + 0.65f * wires;
                    if (t > 0.35) v.active = false;
                    break;
                }
                case 2: { // Closed 12-Bit Hat
                    sig = n * static_cast<float>(std::sin(t * 9500.0)) * static_cast<float>(std::exp(-t * 42.0));
                    if (t > 0.12) v.active = false;
                    break;
                }
                case 3: { // Open Hat
                    sig = n * static_cast<float>(std::sin(t * 8800.0)) * static_cast<float>(std::exp(-t * 7.5));
                    if (t > 0.45) v.active = false;
                    break;
                }
                case 4: { // Rim / Clap
                    double freq = 860.0;
                    v.phase += freq * dt;
                    sig = (0.6f * static_cast<float>(std::sin(2.0 * M_PI * v.phase)) + 0.4f * n) * static_cast<float>(std::exp(-t * 35.0));
                    if (t > 0.18) v.active = false;
                    break;
                }
                case 5: { // Classic Analog Tom
                    double freq = 110.0 + 60.0 * std::exp(-t * 18.0);
                    v.phase += freq * dt;
                    sig = static_cast<float>(std::sin(2.0 * M_PI * v.phase)) * static_cast<float>(std::exp(-t * 9.0));
                    if (t > 0.40) v.active = false;
                    break;
                }
                case 6: { // Rhodes / Minor 9th Sampler Chord Hit
                    const double freqs[4] = { 261.63, 311.13, 392.00, 466.16 }; // Cm7
                    double sum = 0.0;
                    for (int k = 0; k < 4; ++k) {
                        sum += std::sin(2.0 * M_PI * freqs[k] * t + 0.35 * std::sin(4.0 * M_PI * freqs[k] * t));
                    }
                    sig = static_cast<float>(sum * 0.22 * std::exp(-t * 4.2));
                    if (t > 0.65) v.active = false;
                    break;
                }
                case 7: { // MPC Sub Bass Note
                    double freq = 55.0;
                    v.phase += freq * dt;
                    sig = static_cast<float>(std::tanh(1.8 * std::sin(2.0 * M_PI * v.phase)) * std::exp(-t * 3.8));
                    if (t > 0.70) v.active = false;
                    break;
                }
            }
            v.timeSec += dt;
            float lGain = v.level * std::sqrt(0.5f * (1.0f - v.pan));
            float rGain = v.level * std::sqrt(0.5f * (1.0f + v.pan));
            outL += sig * lGain;
            outR += sig * rGain;
        }
    }

    void updateFilters() {
        double lpfCut = static_cast<double>(getParam(kParamLpfCutoff));
        double lpfRes = static_cast<double>(getParam(kParamLpfResonance)) * 0.01;
        // 6th-order Butterworth Q values: 0.517638, 0.707107, 1.931852 (plus optional gentle resonance boost)
        const double qBase[3] = { 0.51763809, 0.70710678, 1.93185165 + lpfRes * 2.5 };
        for (int i = 0; i < 3; ++i) {
            s950Butterworth_[i].setLowPass(sampleRate_, lpfCut, qBase[i]);
        }

        double hpfCut = static_cast<double>(getParam(kParamHpfCutoff));
        double hpfRes = static_cast<double>(getParam(kParamHpfResonance)) * 0.01;
        hpfBiquad_.setHighPass(sampleRate_, hpfCut, 0.7071 + hpfRes * 3.2);

        double brillDb = static_cast<double>(getParam(kParamBrilliance));
        brillianceShelf_.setHighShelf(sampleRate_, 4800.0, brillDb);

        double fattenDb = static_cast<double>(getParam(kParamFatten)) * 0.085; // up to +8.5 dB warm low-mid boost
        fattenShelf_.setLowShelf(sampleRate_, 165.0, fattenDb);
    }

    double sampleRate_ = 48000.0;
    std::array<std::atomic<float>, kNumParams> params_;
    std::atomic<int> currentPreset_{0};
    std::atomic<int> skinIndex_{0};

    Biquad s950Butterworth_[3];
    Biquad hpfBiquad_;
    Biquad brillianceShelf_;
    Biquad fattenShelf_;
    SSM2044LadderFilter ssmLadder_;

    double resampPhase_[2] = {0.0, 0.0};
    float heldSample_[2] = {0.0f, 0.0f};
    float compEnv_ = 0.0f;
    float dynFilterEnv_ = 0.0f;
    float prevOut_[2] = {0.0f, 0.0f};
    uint32_t rngState_ = 0x12345678u;
    uint32_t noiseRng_ = 0x87654321u;

    PadVoice pads_[8];
    std::atomic<bool> demoLoopPlaying_{false};
    double demoTimeSec_ = 0.0;
    int demoStep_ = 0;

    std::atomic<float> inPeakL_{0.0f}, inPeakR_{0.0f};
    std::atomic<float> outPeakL_{0.0f}, outPeakR_{0.0f};
    std::atomic<float> grDb_{0.0f};

    float scopeRing_[256];
    std::atomic<uint32_t> scopeWriteIdx_{0};
    float grHistory_[128];
    std::atomic<uint32_t> grWriteIdx_{0};
    uint32_t sampleCounter_ = 0;
};

} // namespace vsr
