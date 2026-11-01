# VSR-9512X // Vintage Sampler & Studio Mastering Effects Processor

**VSR-9512X** is a universal macOS (`arm64` / `arm64e` / `x86_64`) **VST3**, **Audio Unit (`aufx`)**, and **Standalone CoreAudio** studio effects processor combining the reverse-engineered DSP architectures of six classic sampler and dynamics plugins:

1. **BeatSkillz AK9X** — Akai S950 input drive, harmonic saturation, and pitch-aliased `Distrukt` clock shift
2. **BeatSkillz S12X** — E-mu SP-1200 input preamp overdrive (`SP OVERDRIVE`) and 12-bit ZOH decimation
3. **BeatSkillz SampleX V3** — 6 vintage sampler circuit models (`A: S950`, `B: SP1200`, `C: S900`, `D: MPC60`, `E: μ-LAW`, `F: PAULA`), continuous variable sample-rate clock (`1.9 kHz – 48 kHz`), continuous bit-depth quantizer (`4.0 – 16.0 bit`), `MpoweR` μ-Law compander, floppy disk noise floor (`DISK FLR`), and resonant high-pass/low-pass filters
4. **BeatSkillz MAX1** — VCA/Opto auto-compressor with selectable Feedforward/Feedback detector (`FEEDBACK`), Program-Dependent Auto-Release (`AUTO REL`), harmonic `FATTEN` circuit, Mid/Side Stereo `WIDTH` expander (`0% – 200%`), output `SOFT CLIP`, and live Gain-Reduction history graph
5. **Inphonik RX950** — Authentic 6th-order 36 dB/octave Butterworth low-pass filter (`S950 36dB` MF6), 2nd-order Bessel DC high-pass filter, and DAC high-shelf `BRILLIANCE` EQ
6. **Inphonik RX1200** — 26.041 kHz zero-order-hold resampler and SSM2044 4-pole resonant low-pass filter in both static (`SSM2044`) and envelope-followed (`DYN ENV`) modes

---

## Project Layout

```text
vintage-studio-rack/
├── README.md                  # Documentation & build/install guide
├── build.sh                   # One-command universal macOS build script
├── include/
│   ├── VSR9512X_DSP.h         # Complete C++17 real-time DSP engine, 30 parameters, 8 presets, 5 skin index states
│   └── VSR9512X_UI.h          # Custom CoreGraphics/Cocoa studio hardware UI with 5 skins & 60 FPS OLED scope
├── src/
│   ├── VST3Plugin.mm          # Steinberg VST3 3.7 single-component effects processor
│   ├── AUPlugin.mm            # Apple Audio Unit v2 (aufx / 'V95X' / 'BSKZ') effects processor
│   └── StandaloneApp.mm       # Standalone macOS CoreAudio host & PNG preview exporter
├── tests/
│   └── verify_plugins.mm      # Automated verification suite for VST3 & Audio Unit bundles
└── build/
    ├── VSR-9512X.vst3         # Universal VST3 bundle
    ├── VSR-9512X.component    # Universal Audio Unit (aufx) bundle
    └── VSR-9512X.app          # Universal Standalone macOS app
```

---

## 5 Built-in Hardware Skins

Click the **`SKIN: <NAME>`** button in the top bar (or press **`S`** on your keyboard) to cycle through 5 hardware studio faceplates:

1. **`OBSIDIAN RACK`** — Dark brushed anodized aluminum with Amber & Cyan OLED display
2. **`S950 CREAM`** — Vintage Akai S950 cream/beige rack faceplate with backlit yellow-green LCD and red/cobalt buttons
3. **`SP-1200 SLATE`** — E-mu SP-1200 charcoal blue-grey chassis with Amber VFD scope and sky-blue controls
4. **`SAMPLEX GOLD`** — Black & 24k champagne gold mastering edition
5. **`CYBER NEON`** — Synthwave deep purple chassis with hot magenta & neon cyan OLED display

---

## Building from Source

Run `./build.sh` from `/Users/charlesford/.gemini/antigravity/scratch/vintage-studio-rack`:

```bash
./build.sh
```

### Installing to macOS System Plug-In Folders

```bash
cp -R build/VSR-9512X.vst3 ~/Library/Audio/Plug-Ins/VST3/
cp -R build/VSR-9512X.component /Library/Audio/Plug-Ins/Components/
```

### Validating the Audio Unit

```bash
auval -v aufx V95X BSKZ
```
