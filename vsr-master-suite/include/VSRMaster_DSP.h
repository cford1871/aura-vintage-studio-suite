#pragma once
#include <cmath>
#include <cstring>
#include <algorithm>
#include <atomic>
#include <array>

namespace vsrm {

enum ParamID : int {
    // Module I: PAA Audio Tape56K (Lyrec TR-533 + DSP56K Fixed-Point) & Ozone 12 Unlimiter
    kInGain = 0,          // -12..+12 dB (Tape56K inGain)
    kTape56KDrive,        // 0..100 %   (Tape56K Lyrec TR-533 tape saturation)
    kTapeBiasBump,        // 0..100 %   (Ozone 12 Vintage Tape Bias & Head Bump)
    kUnlimiterAmount,     // 0..100 %   (Ozone 12 Unlimiter upward transient restoration)
    kTapeMode,            // 0 = Lyrec TR-533 + DSP56K, 1 = Studer A810, 2 = S950/SP1200
    kTape56KLink,         // 0/1        (Tape56K inverse I/O constant-gain link)
    kDsp56KFixedPt,       // 0/1        (Motorola DSP56000 24-bit fixed-point saturation)

    // Module II: Oeksound Soothe3 Dynamic Resonance Suppressor
    kSootheDepth,         // 0..100 %   (Soothe3 Depth)
    kSootheDetail,        // 0..100 %   (Soothe3 Sharpness / Detail Q)
    kSootheMaxCut,        // -18..0 dB  (Soothe3 Max Cut ceiling)
    kSootheTilt,          // -100..+100%(Soothe3 Detail Tilt Low/High)
    kSootheHardMode,      // 0/1        (Soothe3 Soft=0 / Hard=1 mode)
    kSootheDelta,         // 0/1        (Soothe3 Delta — listen to removed resonances)

    // Module III: iZotope Ozone 12 Bass Control, Clarity & Stabilizer M/S EQ
    kBassPunch,           // 0..100 %   (Ozone 12 Bass Control Punch)
    kBassSustain,         // 0..100 %   (Ozone 12 Bass Control SustainPowerAmount)
    kStabilizerSculpt,    // 0..100 %   (Ozone 12 Stabilizer adaptive tonal balance)
    kClarityAir,          // 0..100 %   (Ozone 12 Clarity psychoacoustic HF unmasking)
    kLowEndFocus,         // 0/1        (Ozone 12 Low End Focus Punchy contrast)
    kStemVocalDrumBoost,  // 0/1        (Ozone 12 Stem EQ Vocal + Drum focus)

    // Module IV: Ozone 12 Impact, Stereo Imager & MAX1 Bus Glue
    kCompThreshold,       // -30..0 dB  (MAX1 / Vintage Comp Threshold)
    kCompRatio,           // 1.2..10.0  (MAX1 / Vintage Comp Ratio)
    kImpactPunch,         // 0..100 %   (Ozone 12 Impact 4-band micro-dynamics)
    kStereoWidth,         // 0..200 %   (Ozone 12 Imager Stereo Width)
    kBassMonoFreq,        // 20..250 Hz (Bass Mono Maker crossover)
    kStereoizeOn,         // 0/1        (Ozone 12 Phase-coherent Stereoize II)
    kRecoverSidesOn,      // 0/1        (Ozone 12 Imager Recover Sides)

    // Module V: Ozone 12 IRC IV Maximizer & True-Peak Finalizer
    kMaximizerGain,       // 0..+14 dB  (Ozone 12 Maximizer Gain)
    kCeiling,             // -3.0..0 dB (Ozone 12 Margin / Ceiling dBTP)
    kUpwardBoost,         // 0..100 %   (Ozone 12 LowLevelBoost upward compression)
    kMix,                 // 0..100 %   (Global Dry/Wet Mix)
    kSoftClipOn,          // 0/1        (Ozone 12 Maximizer Soft Clip)
    kTruePeakOn,          // 0/1        (Prevent Intersample Clipping)

    kNumParams
};

struct ParamMeta {
    const char* name;
    const char* unit;
    float minVal;
    float maxVal;
    float defVal;
    bool isToggle;
};

static const ParamMeta kParamTable[kNumParams] = {
    {"IN GAIN",       "dB",  -12.0f,  12.0f,   1.5f, false},
    {"TAPE56K DRV",   "%",     0.0f, 100.0f,  38.0f, false},
    {"HEAD BUMP",     "%",     0.0f, 100.0f,  32.0f, false},
    {"UNLIMITER",     "%",     0.0f, 100.0f,  26.0f, false},
    {"TAPE MODEL",    "",      0.0f,   2.0f,   0.0f, false},
    {"GAIN LINK",     "",      0.0f,   1.0f,   1.0f, true},
    {"DSP56K 24B",    "",      0.0f,   1.0f,   1.0f, true},

    {"SOOTHE DEPTH",  "%",     0.0f, 100.0f,  46.0f, false},
    {"DETAIL / Q",    "%",     0.0f, 100.0f,  64.0f, false},
    {"MAX CUT",       "dB",  -18.0f,   0.0f,  -6.5f, false},
    {"SPECT TILT",    "%",  -100.0f, 100.0f,  18.0f, false},
    {"HARD MODE",     "",      0.0f,   1.0f,   0.0f, true},
    {"DELTA LISTEN",  "",      0.0f,   1.0f,   0.0f, true},

    {"BASS PUNCH",    "%",     0.0f, 100.0f,  54.0f, false},
    {"SUB SUSTAIN",   "%",     0.0f, 100.0f,  68.0f, false},
    {"STABILIZER",    "%",     0.0f, 100.0f,  42.0f, false},
    {"CLARITY AIR",   "%",     0.0f, 100.0f,  48.0f, false},
    {"LOW FOCUS",     "",      0.0f,   1.0f,   1.0f, true},
    {"STEM BOOST",    "",      0.0f,   1.0f,   1.0f, true},

    {"COMP THRESH",   "dB",  -30.0f,   0.0f, -14.5f, false},
    {"COMP RATIO",    ":1",    1.2f,  10.0f,   2.5f, false},
    {"IMPACT PUNCH",  "%",     0.0f, 100.0f,  36.0f, false},
    {"STEREO WIDTH",  "%",     0.0f, 200.0f, 122.0f, false},
    {"BASS MONO",     "Hz",   20.0f, 250.0f, 110.0f, false},
    {"STEREOIZE II",  "",      0.0f,   1.0f,   1.0f, true},
    {"RECOVER SIDES", "",      0.0f,   1.0f,   1.0f, true},

    {"MAXIMIZER",     "dB",    0.0f,  14.0f,   4.6f, false},
    {"CEILING",       "dBTP", -3.0f,   0.0f,  -0.1f, false},
    {"UPWARD COMP",   "%",     0.0f, 100.0f,  28.0f, false},
    {"DRY / WET",     "%",     0.0f, 100.0f, 100.0f, false},
    {"SOFT CLIP",     "",      0.0f,   1.0f,   1.0f, true},
    {"ISP TRUE PEAK", "",      0.0f,   1.0f,   1.0f, true}
};

struct Biquad {
    double b0 = 1.0, b1 = 0.0, b2 = 0.0, a1 = 0.0, a2 = 0.0;
    double z1L = 0.0, z2L = 0.0, z1R = 0.0, z2R = 0.0;

    void reset() { z1L = z2L = z1R = z2R = 0.0; }

    void setPeaking(double fs, double fc, double Q, double gainDB) {
        fc = std::max(20.0, std::min(fs * 0.45, fc));
        double A = std::pow(10.0, gainDB / 40.0);
        double w0 = 2.0 * M_PI * fc / fs;
        double alpha = std::sin(w0) / (2.0 * std::max(0.2, Q));
        double cosw = std::cos(w0);
        double a0 = 1.0 + alpha / A;
        b0 = (1.0 + alpha * A) / a0;
        b1 = (-2.0 * cosw) / a0;
        b2 = (1.0 - alpha * A) / a0;
        a1 = b1;
        a2 = (1.0 - alpha / A) / a0;
    }

    void setHighShelf(double fs, double fc, double gainDB) {
        fc = std::max(1000.0, std::min(fs * 0.46, fc));
        double A = std::pow(10.0, gainDB / 40.0);
        double w0 = 2.0 * M_PI * fc / fs;
        double cosw = std::cos(w0);
        double sinw = std::sin(w0);
        double alpha = sinw / 2.0 * std::sqrt(2.0);
        double sqA = 2.0 * std::sqrt(A) * alpha;
        double a0 = (A + 1.0) - (A - 1.0) * cosw + sqA;
        b0 = (A * ((A + 1.0) + (A - 1.0) * cosw + sqA)) / a0;
        b1 = (-2.0 * A * ((A - 1.0) + (A + 1.0) * cosw)) / a0;
        b2 = (A * ((A + 1.0) + (A - 1.0) * cosw - sqA)) / a0;
        a1 = (2.0 * ((A - 1.0) - (A + 1.0) * cosw)) / a0;
        a2 = ((A + 1.0) - (A - 1.0) * cosw - sqA) / a0;
    }

    void setBandpass(double fs, double fc, double Q) {
        fc = std::max(40.0, std::min(fs * 0.45, fc));
        double w0 = 2.0 * M_PI * fc / fs;
        double alpha = std::sin(w0) / (2.0 * std::max(0.3, Q));
        double a0 = 1.0 + alpha;
        b0 = alpha / a0;
        b1 = 0.0;
        b2 = -alpha / a0;
        a1 = (-2.0 * std::cos(w0)) / a0;
        a2 = (1.0 - alpha) / a0;
    }

    inline void processStereo(float& l, float& r) {
        double inL = l, inR = r;
        double outL = b0 * inL + z1L;
        z1L = b1 * inL - a1 * outL + z2L;
        z2L = b2 * inL - a2 * outL;

        double outR = b0 * inR + z1R;
        z1R = b1 * inR - a1 * outR + z2R;
        z2R = b2 * inR - a2 * outR;
        l = static_cast<float>(outL);
        r = static_cast<float>(outR);
    }
};

struct PresetBank {
    const char* name;
    const char* shortLabel;
    float vals[kNumParams];
};

static const PresetBank kPresets[8] = {
    {"01. Ozone12 + Soothe3 + Tape56K Master", "O12+STH3+56K",
     {1.5f, 38.0f, 32.0f, 26.0f, 0.0f, 1.0f, 1.0f,  46.0f, 64.0f, -6.5f, 18.0f, 0.0f, 0.0f,
      54.0f, 68.0f, 42.0f, 48.0f, 1.0f, 1.0f,      -14.5f, 2.5f, 36.0f, 122.0f, 110.0f, 1.0f, 1.0f,
      4.6f, -0.1f, 28.0f, 100.0f, 1.0f, 1.0f}},
    {"02. Tape56K Lyrec TR-533 Warm Glue", "TAPE56K GLUE",
     {3.0f, 68.0f, 58.0f, 15.0f, 0.0f, 1.0f, 1.0f,  28.0f, 50.0f, -4.0f, -10.0f, 0.0f, 0.0f,
      62.0f, 75.0f, 30.0f, 32.0f, 1.0f, 0.0f,      -16.0f, 3.0f, 42.0f, 112.0f, 120.0f, 1.0f, 1.0f,
      3.8f, -0.2f, 22.0f, 100.0f, 1.0f, 1.0f}},
    {"03. Soothe3 Harshness & Mud Tamer", "SOOTHE3 TAME",
     {0.5f, 20.0f, 18.0f, 12.0f, 1.0f, 1.0f, 0.0f,  76.0f, 82.0f, -11.0f, 35.0f, 1.0f, 0.0f,
      40.0f, 52.0f, 64.0f, 58.0f, 1.0f, 1.0f,      -12.0f, 2.0f, 25.0f, 116.0f, 95.0f, 0.0f, 1.0f,
      3.2f, -0.1f, 18.0f, 100.0f, 1.0f, 1.0f}},
    {"04. Ozone12 Hip-Hop 808 Bass Control", "808 BASS CTRL",
     {2.2f, 45.0f, 64.0f, 34.0f, 0.0f, 1.0f, 1.0f,  42.0f, 60.0f, -5.5f, 10.0f, 0.0f, 0.0f,
      84.0f, 88.0f, 48.0f, 52.0f, 1.0f, 1.0f,      -16.5f, 3.5f, 64.0f, 128.0f, 135.0f, 1.0f, 1.0f,
      5.8f, -0.1f, 35.0f, 100.0f, 1.0f, 1.0f}},
    {"05. Unlimiter + Impact Punch Restorer", "UNLIMIT PUNCH",
     {1.0f, 28.0f, 30.0f, 78.0f, 1.0f, 1.0f, 0.0f,  35.0f, 55.0f, -4.5f, 0.0f, 0.0f, 0.0f,
      72.0f, 54.0f, 38.0f, 44.0f, 1.0f, 1.0f,      -11.0f, 1.8f, 82.0f, 118.0f, 100.0f, 1.0f, 1.0f,
      3.4f, -0.2f, 48.0f, 100.0f, 0.0f, 1.0f}},
    {"06. Clarity & Imager Wide Pop Air", "CLARITY WIDE",
     {0.8f, 24.0f, 22.0f, 20.0f, 1.0f, 1.0f, 0.0f,  52.0f, 70.0f, -6.0f, 42.0f, 0.0f, 0.0f,
      48.0f, 60.0f, 72.0f, 85.0f, 1.0f, 1.0f,      -13.5f, 2.2f, 45.0f, 152.0f, 90.0f, 1.0f, 1.0f,
      5.0f, -0.1f, 32.0f, 100.0f, 1.0f, 1.0f}},
    {"07. Vintage S950 + DSP56K Lo-Fi Bus", "S950+DSP56K",
     {3.5f, 74.0f, 70.0f, 40.0f, 2.0f, 1.0f, 1.0f,  38.0f, 48.0f, -5.0f, -25.0f, 0.0f, 0.0f,
      78.0f, 80.0f, 35.0f, 28.0f, 1.0f, 0.0f,      -18.0f, 4.0f, 58.0f, 95.0f, 150.0f, 0.0f, 0.0f,
      5.2f, -0.1f, 40.0f, 100.0f, 1.0f, 1.0f}},
    {"08. IRC IV Streaming Master (-9 LUFS)", "IRC IV MASTER",
     {1.2f, 30.0f, 25.0f, 22.0f, 0.0f, 1.0f, 1.0f,  50.0f, 68.0f, -7.0f, 15.0f, 0.0f, 0.0f,
      58.0f, 64.0f, 55.0f, 60.0f, 1.0f, 1.0f,      -15.0f, 2.4f, 40.0f, 126.0f, 105.0f, 1.0f, 1.0f,
      6.4f, -0.1f, 30.0f, 100.0f, 1.0f, 1.0f}}
};

static constexpr int kNumPresets = 8;
static const PresetBank (&kFactoryPresets)[kNumPresets] = kPresets;

class VSRMasterEngine {
public:
    static constexpr int kScopeSize = 256;
    static constexpr int kSootheBands = 6;

    VSRMasterEngine() {
        for (int i = 0; i < kNumParams; ++i) {
            params_[i].store(kParamTable[i].defVal, std::memory_order_relaxed);
        }
        sootheAttenDB_.fill(0.0f);
        grHistory_.fill(0.0f);
        vecX_.fill(0.0f);
        vecY_.fill(0.0f);
        fftMag_.fill(0.25f);
        prepare(48000.0);
    }

    void prepare(double sampleRate) {
        sampleRate_ = (sampleRate > 8000.0) ? sampleRate : 48000.0;
        headBumpFilter_.reset();
        bassPunchFilter_.reset();
        clarityShelf_.reset();
        stemPresence_.reset();
        for (int b = 0; b < kSootheBands; ++b) {
            sootheDetect_[b].reset();
            sootheNotch_[b].reset();
            sootheEnv_[b] = 0.0;
        }
        compEnv_ = 0.0;
        fastTransEnv_ = 0.0;
        slowTransEnv_ = 0.0;
        bassLowL_ = bassLowR_ = 0.0;
        stereoDelayIdx_ = 0;
        stereoDelayL_.fill(0.0f);
        stereoDelayR_.fill(0.0f);
        updateFilters();
    }

    void setParameterNormalized(int id, float norm) {
        if (id < 0 || id >= kNumParams) return;
        norm = std::max(0.0f, std::min(1.0f, norm));
        const auto& m = kParamTable[id];
        float val = m.minVal + norm * (m.maxVal - m.minVal);
        if (m.isToggle) val = (norm >= 0.5f) ? 1.0f : 0.0f;
        params_[id].store(val, std::memory_order_relaxed);
    }

    float getParameterNormalized(int id) const {
        if (id < 0 || id >= kNumParams) return 0.0f;
        const auto& m = kParamTable[id];
        float v = params_[id].load(std::memory_order_relaxed);
        return (v - m.minVal) / (m.maxVal - m.minVal);
    }

    void setParameterValue(int id, float val) {
        if (id < 0 || id >= kNumParams) return;
        const auto& m = kParamTable[id];
        val = std::max(m.minVal, std::min(m.maxVal, val));
        params_[id].store(val, std::memory_order_relaxed);
    }

    float getParameterValue(int id) const {
        if (id < 0 || id >= kNumParams) return 0.0f;
        return params_[id].load(std::memory_order_relaxed);
    }

    void setParam(int id, float val) { setParameterValue(id, val); }
    float getParam(int id) const { return getParameterValue(id); }
    void setParamNormalized(int id, float norm) { setParameterNormalized(id, norm); }
    float getParamNormalized(int id) const { return getParameterNormalized(id); }

    void loadPreset(int idx) {
        if (idx < 0 || idx >= 8) return;
        currentPreset_.store(idx, std::memory_order_relaxed);
        for (int i = 0; i < kNumParams; ++i) {
            params_[i].store(kPresets[idx].vals[i], std::memory_order_relaxed);
        }
    }

    int getCurrentPreset() const { return currentPreset_.load(std::memory_order_relaxed); }

    void setSkin(int idx) { skinIndex_.store((idx % 5 + 5) % 5, std::memory_order_relaxed); }
    int getSkin() const { return skinIndex_.load(std::memory_order_relaxed); }

    void setDemoLoopPlaying(bool play) { demoLoopPlaying_.store(play, std::memory_order_relaxed); }
    bool isDemoLoopPlaying() const { return demoLoopPlaying_.load(std::memory_order_relaxed); }

    void updateFilters() {
        double bumpPct = getParameterValue(kTapeBiasBump) * 0.01;
        headBumpFilter_.setPeaking(sampleRate_, 72.0, 1.15, bumpPct * 4.5);

        double bassPunchPct = getParameterValue(kBassPunch) * 0.01;
        double bassSustPct  = getParameterValue(kBassSustain) * 0.01;
        bassPunchFilter_.setPeaking(sampleRate_, 58.0, 0.95, (bassPunchPct * 3.5 + bassSustPct * 2.5) - 1.5);

        double stabPct = getParameterValue(kStabilizerSculpt) * 0.01;
        double stemOn  = getParameterValue(kStemVocalDrumBoost) >= 0.5f ? 1.0 : 0.0;
        stemPresence_.setPeaking(sampleRate_, 2850.0, 1.1, stabPct * 2.4 + stemOn * 1.4);

        double clarPct = getParameterValue(kClarityAir) * 0.01;
        clarityShelf_.setHighShelf(sampleRate_, 10500.0, clarPct * 5.2);

        static const double kResFreqs[kSootheBands] = {240.0, 580.0, 1450.0, 3100.0, 5600.0, 9800.0};
        double detailQ = 1.6 + (getParameterValue(kSootheDetail) * 0.01) * 5.4;
        for (int b = 0; b < kSootheBands; ++b) {
            sootheDetect_[b].setBandpass(sampleRate_, kResFreqs[b], detailQ);
        }
    }

    void processBlock(const float* inL, const float* inR, float* outL, float* outR, int numFrames) {
        updateFilters();

        float inGainDB = getParameterValue(kInGain);
        float inGainLin = std::pow(10.0f, inGainDB / 20.0f);
        bool linkGain = getParameterValue(kTape56KLink) >= 0.5f;
        float invOutLin = linkGain ? std::pow(10.0f, -inGainDB * 0.65f / 20.0f) : 1.0f;

        float tapeDrv = getParameterValue(kTape56KDrive) * 0.01f;
        float unlimAmt = getParameterValue(kUnlimiterAmount) * 0.01f;
        int tapeMode = static_cast<int>(std::round(getParameterValue(kTapeMode)));
        bool dsp56k = getParameterValue(kDsp56KFixedPt) >= 0.5f;

        float sootheDepth = getParameterValue(kSootheDepth) * 0.01f;
        float sootheMaxCut = getParameterValue(kSootheMaxCut);
        float sootheTilt = getParameterValue(kSootheTilt) * 0.01f;
        bool sootheHard = getParameterValue(kSootheHardMode) >= 0.5f;
        bool sootheDelta = getParameterValue(kSootheDelta) >= 0.5f;
        double detailQ = 1.6 + (getParameterValue(kSootheDetail) * 0.01) * 5.4;

        float threshDB = getParameterValue(kCompThreshold);
        float ratio = getParameterValue(kCompRatio);
        float impactAmt = getParameterValue(kImpactPunch) * 0.01f;
        float width = getParameterValue(kStereoWidth) * 0.01f;
        float bassMonoHz = getParameterValue(kBassMonoFreq);
        bool stereoize = getParameterValue(kStereoizeOn) >= 0.5f;
        bool recSides = getParameterValue(kRecoverSidesOn) >= 0.5f;

        float maxGainLin = std::pow(10.0f, getParameterValue(kMaximizerGain) / 20.0f);
        float ceilLin = std::pow(10.0f, getParameterValue(kCeiling) / 20.0f);
        float upBoost = getParameterValue(kUpwardBoost) * 0.01f;
        float wetMix = getParameterValue(kMix) * 0.01f;
        bool softClip = getParameterValue(kSoftClipOn) >= 0.5f;

        double monoAlpha = std::exp(-2.0 * M_PI * bassMonoHz / sampleRate_);
        static const double kResFreqs[kSootheBands] = {240.0, 580.0, 1450.0, 3100.0, 5600.0, 9800.0};

        float peakIn = 0.0f, peakOut = 0.0f;
        float maxGR = 0.0f;
        double corrNum = 0.0, corrDenL = 0.0, corrDenR = 0.0;

        for (int i = 0; i < numFrames; ++i) {
            float dryL = inL ? inL[i] : 0.0f;
            float dryR = inR ? inR[i] : dryL;

            // Built-in studio mastering loop generator if host input is silent and demo is active
            if (demoLoopPlaying_.load(std::memory_order_relaxed) && std::fabs(dryL) < 1e-5f && std::fabs(dryR) < 1e-5f) {
                double t = demoPhase_ / sampleRate_;
                double beat = std::fmod(t * 2.1, 4.0);
                double kickEnv = std::exp(-std::fmod(beat, 1.0) * 11.0);
                double kickFreq = 48.0 + 95.0 * std::exp(-std::fmod(beat, 1.0) * 38.0);
                double sub808 = std::sin(2.0 * M_PI * kickFreq * t) * kickEnv * 0.72;
                double snareEnv = (std::fmod(beat, 2.0) >= 1.0) ? std::exp(-(std::fmod(beat, 2.0) - 1.0) * 18.0) : 0.0;
                double chordL = 0.22 * std::sin(2.0 * M_PI * 220.0 * t) + 0.16 * std::sin(2.0 * M_PI * 330.0 * t + 0.4)
                              + 0.12 * std::sin(2.0 * M_PI * 3100.0 * t) * snareEnv;
                double chordR = 0.22 * std::sin(2.0 * M_PI * 220.5 * t) + 0.16 * std::sin(2.0 * M_PI * 329.5 * t - 0.4)
                              + 0.12 * std::cos(2.0 * M_PI * 3100.0 * t) * snareEnv;
                dryL = static_cast<float>(sub808 + chordL);
                dryR = static_cast<float>(sub808 + chordR);
                demoPhase_ += 1.0;
            }

            peakIn = std::max(peakIn, std::max(std::fabs(dryL), std::fabs(dryR)));
            float sL = dryL * inGainLin;
            float sR = dryR * inGainLin;

            // --- MODULE I: PAA Audio Tape56K (Lyrec TR-533 + DSP56K Fixed-Point) & Ozone 12 Unlimiter ---
            headBumpFilter_.processStereo(sL, sR);
            if (tapeDrv > 0.001f) {
                float drv = 1.0f + tapeDrv * (tapeMode == 0 ? 3.2f : (tapeMode == 1 ? 2.4f : 4.2f));
                // Asymmetric Lyrec TR-533 even-harmonic hysteresis
                float asym = 0.08f * tapeDrv;
                sL = std::tanh(sL * drv + asym) - std::tanh(asym);
                sR = std::tanh(sR * drv + asym) - std::tanh(asym);
            }
            if (dsp56k) {
                // Motorola DSP56000 24-bit fixed-point accumulator quantization & soft saturation
                const float qScale = (tapeMode == 2) ? 2048.0f : 32768.0f;
                sL = std::round(sL * qScale) / qScale;
                sR = std::round(sR * qScale) / qScale;
            }
            // Ozone 12 Unlimiter + Impact transient restoration
            double absIn = 0.5 * (std::fabs(sL) + std::fabs(sR));
            fastTransEnv_ = 0.92 * fastTransEnv_ + 0.08 * absIn;
            slowTransEnv_ = 0.996 * slowTransEnv_ + 0.004 * absIn;
            double transDiff = std::max(0.0, fastTransEnv_ - slowTransEnv_);
            float transBoost = 1.0f + static_cast<float>(transDiff * (unlimAmt * 2.2 + impactAmt * 1.8));
            sL *= transBoost * invOutLin;
            sR *= transBoost * invOutLin;

            // --- MODULE II: Oeksound Soothe3 Dynamic Resonance Suppressor ---
            float preSootheL = sL, preSootheR = sR;
            if (sootheDepth > 0.001f) {
                for (int b = 0; b < kSootheBands; ++b) {
                    float bpL = sL, bpR = sR;
                    sootheDetect_[b].processStereo(bpL, bpR);
                    double bandEnv = 0.5 * (std::fabs(bpL) + std::fabs(bpR));
                    sootheEnv_[b] = 0.985 * sootheEnv_[b] + 0.015 * bandEnv;
                    double tiltWeight = 1.0 + sootheTilt * ((b - 2.5) / 2.5) * 0.45;
                    double rawCutDB = -sootheEnv_[b] * sootheDepth * (sootheHard ? 42.0 : 26.0) * std::max(0.25, tiltWeight);
                    double clampedCutDB = std::max(static_cast<double>(sootheMaxCut), std::min(0.0, rawCutDB));
                    sootheAttenDB_[b] = static_cast<float>(clampedCutDB);
                    if ((i & 31) == 0) {
                        sootheNotch_[b].setPeaking(sampleRate_, kResFreqs[b], detailQ, clampedCutDB);
                    }
                    sootheNotch_[b].processStereo(sL, sR);
                }
                if (sootheDelta) {
                    sL = preSootheL - sL;
                    sR = preSootheR - sR;
                }
            } else {
                sootheAttenDB_.fill(0.0f);
            }

            // --- MODULE III: Ozone 12 Bass Control, Stabilizer & Clarity Air ---
            bassPunchFilter_.processStereo(sL, sR);
            stemPresence_.processStereo(sL, sR);
            clarityShelf_.processStereo(sL, sR);

            // --- MODULE IV: MAX1 Bus Glue Compressor & Ozone 12 Stereo Imager ---
            double det = std::max(std::fabs(sL), std::fabs(sR));
            compEnv_ = (det > compEnv_) ? (0.96 * compEnv_ + 0.04 * det) : (0.9985 * compEnv_ + 0.0015 * det);
            double envDB = 20.0 * std::log10(std::max(1e-6, compEnv_));
            float grDB = 0.0f;
            if (envDB > threshDB) {
                grDB = static_cast<float>((envDB - threshDB) * (1.0 - 1.0 / ratio));
                float grLin = std::pow(10.0f, -grDB / 20.0f);
                sL *= grLin;
                sR *= grLin;
            }
            maxGR = std::max(maxGR, grDB);

            // Elliptical Bass Mono Maker + M/S Width + Stereoize II
            bassLowL_ = monoAlpha * bassLowL_ + (1.0 - monoAlpha) * sL;
            bassLowR_ = monoAlpha * bassLowR_ + (1.0 - monoAlpha) * sR;
            float highL = sL - static_cast<float>(bassLowL_);
            float highR = sR - static_cast<float>(bassLowR_);
            float bassMono = 0.5f * static_cast<float>(bassLowL_ + bassLowR_);

            float mid  = 0.5f * (highL + highR);
            float side = 0.5f * (highL - highR) * width;
            if (recSides && grDB > 0.1f) {
                side *= std::pow(10.0f, (grDB * 0.35f) / 20.0f);
            }
            if (stereoize) {
                int rdIdx = (stereoDelayIdx_ + 512 - 19) & 511;
                float haas = stereoDelayL_[rdIdx] * 0.14f;
                side += haas;
            }
            stereoDelayL_[stereoDelayIdx_] = mid;
            stereoDelayIdx_ = (stereoDelayIdx_ + 1) & 511;

            sL = bassMono + mid + side;
            sR = bassMono + mid - side;

            // --- MODULE V: Ozone 12 IRC IV Maximizer & True-Peak Brickwall Finalizer ---
            if (upBoost > 0.001f) {
                float lowLvl = 1.0f / (1.0f + 3.0f * (std::fabs(sL) + std::fabs(sR)));
                float upGain = 1.0f + upBoost * 0.45f * lowLvl;
                sL *= upGain;
                sR *= upGain;
            }
            sL *= maxGainLin;
            sR *= maxGainLin;

            if (softClip) {
                sL = ceilLin * std::tanh(sL / std::max(0.05f, ceilLin));
                sR = ceilLin * std::tanh(sR / std::max(0.05f, ceilLin));
            }
            sL = std::max(-ceilLin, std::min(ceilLin, sL));
            sR = std::max(-ceilLin, std::min(ceilLin, sR));

            float finalL = dryL * (1.0f - wetMix) + sL * wetMix;
            float finalR = dryR * (1.0f - wetMix) + sR * wetMix;

            if (outL) outL[i] = finalL;
            if (outR) outR[i] = finalR;

            peakOut = std::max(peakOut, std::max(std::fabs(finalL), std::fabs(finalR)));
            corrNum  += finalL * finalR;
            corrDenL += finalL * finalL;
            corrDenR += finalR * finalR;

            if ((i & 7) == 0) {
                int w = scopeWriteIdx_.load(std::memory_order_relaxed);
                vecX_[w] = 0.5f * (finalL - finalR);
                vecY_[w] = 0.5f * (finalL + finalR);
                scopeWriteIdx_.store((w + 1) & (kScopeSize - 1), std::memory_order_relaxed);
            }
        }

        inPeak_.store(peakIn, std::memory_order_relaxed);
        outPeak_.store(peakOut, std::memory_order_relaxed);
        currentGR_.store(maxGR, std::memory_order_relaxed);

        int gw = grWriteIdx_.load(std::memory_order_relaxed);
        grHistory_[gw] = maxGR;
        grWriteIdx_.store((gw + 1) & 127, std::memory_order_relaxed);

        double denom = std::sqrt(corrDenL * corrDenR);
        float corr = (denom > 1e-9) ? static_cast<float>(corrNum / denom) : 0.92f;
        phaseCorr_.store(std::max(-1.0f, std::min(1.0f, corr)), std::memory_order_relaxed);
    }

    float getInputPeak() const { return inPeak_.load(std::memory_order_relaxed); }
    float getOutputPeak() const { return outPeak_.load(std::memory_order_relaxed); }
    float getCurrentGR() const { return currentGR_.load(std::memory_order_relaxed); }
    float getPhaseCorrelation() const { return phaseCorr_.load(std::memory_order_relaxed); }
    float getSootheBandAttenDB(int b) const { return (b >= 0 && b < kSootheBands) ? sootheAttenDB_[b] : 0.0f; }

    const std::array<float, kScopeSize>& getVecX() const { return vecX_; }
    const std::array<float, kScopeSize>& getVecY() const { return vecY_; }
    const std::array<float, 128>& getGRHistory() const { return grHistory_; }
    int getGRWriteIdx() const { return grWriteIdx_.load(std::memory_order_relaxed); }

private:
    double sampleRate_ = 48000.0;
    double demoPhase_ = 0.0;
    std::array<std::atomic<float>, kNumParams> params_;
    std::atomic<int> currentPreset_{0};
    std::atomic<int> skinIndex_{0};
    std::atomic<bool> demoLoopPlaying_{true};

    Biquad headBumpFilter_;
    Biquad bassPunchFilter_;
    Biquad stemPresence_;
    Biquad clarityShelf_;
    std::array<Biquad, kSootheBands> sootheDetect_;
    std::array<Biquad, kSootheBands> sootheNotch_;
    std::array<double, kSootheBands> sootheEnv_{};
    std::array<float, kSootheBands> sootheAttenDB_{};

    double compEnv_ = 0.0;
    double fastTransEnv_ = 0.0;
    double slowTransEnv_ = 0.0;
    double bassLowL_ = 0.0;
    double bassLowR_ = 0.0;
    std::array<float, 512> stereoDelayL_{};
    std::array<float, 512> stereoDelayR_{};
    int stereoDelayIdx_ = 0;

    std::atomic<float> inPeak_{0.0f};
    std::atomic<float> outPeak_{0.0f};
    std::atomic<float> currentGR_{0.0f};
    std::atomic<float> phaseCorr_{0.92f};

    std::array<float, kScopeSize> vecX_{};
    std::array<float, kScopeSize> vecY_{};
    std::atomic<int> scopeWriteIdx_{0};

    std::array<float, 128> grHistory_{};
    std::atomic<int> grWriteIdx_{0};
    std::array<float, 64> fftMag_{};
};

} // namespace vsrm
