#pragma once
#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>
#include <functional>
#include <vector>
#include <string>
#include "VSR9512X_DSP.h"

namespace vsr {

enum ColorRole : int {
    kRolePrimary = 0,
    kRoleSecondary = 1,
    kRoleDrive = 2,
    kRoleDynamics = 3
};

struct KnobControl {
    int paramId;
    NSRect rect;
    NSString* title;
    ColorRole role;
};

struct ToggleControl {
    int paramId;
    NSRect rect;
    NSString* label;
    int radioValue; // -1 for boolean toggle, >=0 for stepped radio button
};

struct ActionButton {
    int actionId; // 0..7: Bank 1..8, 100: Prev Preset, 101: Next Preset, 102: Demo Groove, 103: Cycle Skin, 200..202: Clock detents
    NSRect rect;
    NSString* label;
    NSString* sublabel;
};

struct SkinTheme {
    const char* name;
    bool isLightChassis;
    NSColor* chassisTop;
    NSColor* chassisBot;
    NSColor* rackEar;
    NSColor* headerBg;
    NSColor* headerBorder;
    NSColor* brandTitle;
    NSColor* brandSub;
    NSColor* screenTop;
    NSColor* screenBot;
    NSColor* screenBorder;
    NSColor* screenGrid;
    NSColor* eqStroke;
    NSColor* eqFill;
    NSColor* waveStroke;
    NSColor* grStroke;
    NSColor* bayTop;
    NSColor* bayBot;
    NSColor* bayBorder;
    NSColor* bayHeaderBg;
    NSColor* bayTitleText;
    NSColor* bayBadgeText;
    NSColor* knobTop;
    NSColor* knobBot;
    NSColor* knobBorder;
    NSColor* knobTrack;
    NSColor* labelText;
    NSColor* toggleOnBg;
    NSColor* toggleOnText;
    NSColor* toggleOffBg;
    NSColor* toggleOffText;
    NSColor* accentPrimary;
    NSColor* accentSecondary;
    NSColor* accentDrive;
    NSColor* accentDynamics;
};

static inline NSColor* rgb(double r, double g, double b, double a = 1.0) {
    return [NSColor colorWithCalibratedRed:r green:g blue:b alpha:a];
}

static inline SkinTheme getSkinTheme(int idx) {
    switch ((idx % 5 + 5) % 5) {
        case 1: // AKAI S950 CREAM (1988 Putty/Cream Faceplate + Classic Backlit Green LCD)
            return {
                "S950 CREAM", true,
                rgb(0.86, 0.84, 0.78), rgb(0.77, 0.75, 0.69),
                rgb(0.22, 0.23, 0.25),
                rgb(0.18, 0.19, 0.21), rgb(0.42, 0.41, 0.38),
                rgb(0.95, 0.88, 0.72), rgb(0.75, 0.76, 0.78),
                rgb(0.09, 0.17, 0.08), rgb(0.05, 0.11, 0.05),
                rgb(0.45, 0.68, 0.28, 0.75), rgb(0.28, 0.48, 0.18, 0.28),
                rgb(0.62, 0.94, 0.32, 0.95), rgb(0.55, 0.88, 0.25, 0.18),
                rgb(0.92, 0.98, 0.45, 0.95), rgb(0.45, 0.95, 0.65, 0.95),
                rgb(0.83, 0.81, 0.75), rgb(0.75, 0.73, 0.67),
                rgb(0.52, 0.50, 0.45), rgb(0.20, 0.21, 0.23),
                rgb(0.96, 0.95, 0.90), rgb(0.95, 0.42, 0.32),
                rgb(0.24, 0.25, 0.27), rgb(0.12, 0.13, 0.15),
                rgb(0.45, 0.46, 0.48), rgb(0.55, 0.53, 0.48),
                rgb(0.14, 0.15, 0.17),
                rgb(0.84, 0.24, 0.18), rgb(1.00, 1.00, 1.00),
                rgb(0.22, 0.23, 0.25), rgb(0.90, 0.89, 0.85),
                rgb(0.78, 0.22, 0.16), // Crimson Red
                rgb(0.12, 0.38, 0.68), // S950 Cobalt Blue
                rgb(0.85, 0.30, 0.12), // Warm Rust
                rgb(0.15, 0.52, 0.28)  // Forest Meter Green
            };
        case 2: // E-MU SP-1200 SLATE (1987 Battleship Grey + Royal Blue & Red)
            return {
                "SP-1200 SLATE", false,
                rgb(0.24, 0.27, 0.31), rgb(0.16, 0.18, 0.22),
                rgb(0.10, 0.11, 0.13),
                rgb(0.12, 0.14, 0.17), rgb(0.22, 0.52, 0.92, 0.65),
                rgb(0.98, 0.32, 0.24), rgb(0.68, 0.78, 0.92),
                rgb(0.12, 0.05, 0.02), rgb(0.06, 0.02, 0.01),
                rgb(1.00, 0.48, 0.14, 0.55), rgb(0.55, 0.22, 0.08, 0.28),
                rgb(1.00, 0.55, 0.16, 0.92), rgb(1.00, 0.45, 0.12, 0.16),
                rgb(0.28, 0.72, 1.00, 0.95), rgb(1.00, 0.78, 0.22, 0.92),
                rgb(0.21, 0.24, 0.28), rgb(0.15, 0.17, 0.20),
                rgb(0.32, 0.38, 0.46), rgb(0.10, 0.13, 0.18),
                rgb(0.94, 0.96, 0.99), rgb(0.35, 0.72, 1.00),
                rgb(0.18, 0.20, 0.24), rgb(0.08, 0.09, 0.11),
                rgb(0.42, 0.46, 0.52), rgb(0.08, 0.09, 0.11),
                rgb(0.88, 0.91, 0.95),
                rgb(0.20, 0.56, 0.96), rgb(1.00, 1.00, 1.00),
                rgb(0.11, 0.13, 0.16), rgb(0.78, 0.82, 0.88),
                rgb(1.00, 0.62, 0.18), // Warm Amber
                rgb(0.28, 0.68, 1.00), // SP Royal Blue
                rgb(0.98, 0.30, 0.24), // SP Red
                rgb(0.38, 0.92, 0.62)  // Mint Dynamics
            };
        case 3: // SAMPLEX GOLD (Boutique Black & 24K Champagne Gold)
            return {
                "SAMPLEX GOLD", false,
                rgb(0.07, 0.06, 0.05), rgb(0.03, 0.03, 0.02),
                rgb(0.02, 0.02, 0.02),
                rgb(0.09, 0.08, 0.06), rgb(0.85, 0.68, 0.28, 0.65),
                rgb(0.96, 0.80, 0.38), rgb(0.72, 0.64, 0.46),
                rgb(0.08, 0.06, 0.02), rgb(0.03, 0.02, 0.01),
                rgb(0.92, 0.74, 0.30, 0.50), rgb(0.45, 0.35, 0.14, 0.25),
                rgb(0.96, 0.78, 0.32, 0.92), rgb(0.92, 0.72, 0.24, 0.15),
                rgb(1.00, 0.92, 0.65, 0.95), rgb(1.00, 0.55, 0.22, 0.92),
                rgb(0.10, 0.09, 0.08), rgb(0.06, 0.05, 0.04),
                rgb(0.62, 0.48, 0.22, 0.75), rgb(0.14, 0.11, 0.07),
                rgb(0.98, 0.86, 0.55), rgb(0.92, 0.72, 0.28),
                rgb(0.22, 0.19, 0.14), rgb(0.08, 0.07, 0.05),
                rgb(0.78, 0.62, 0.28), rgb(0.04, 0.03, 0.02),
                rgb(0.90, 0.84, 0.68),
                rgb(0.90, 0.70, 0.24), rgb(0.05, 0.04, 0.02),
                rgb(0.08, 0.07, 0.05), rgb(0.82, 0.74, 0.55),
                rgb(0.96, 0.78, 0.30), // 24K Gold
                rgb(1.00, 0.88, 0.55), // Champagne
                rgb(1.00, 0.48, 0.20), // Tube Amber
                rgb(0.88, 0.74, 0.36)  // Warm Brass
            };
        case 4: // CYBER NEON 88 (Synthwave / Outrun Indigo, Magenta & Laser Cyan)
            return {
                "CYBER NEON", false,
                rgb(0.09, 0.05, 0.16), rgb(0.04, 0.02, 0.09),
                rgb(0.03, 0.01, 0.06),
                rgb(0.12, 0.06, 0.22), rgb(1.00, 0.18, 0.58, 0.65),
                rgb(0.00, 0.95, 1.00), rgb(0.88, 0.52, 0.96),
                rgb(0.05, 0.02, 0.12), rgb(0.02, 0.01, 0.06),
                rgb(0.00, 0.95, 1.00, 0.55), rgb(0.55, 0.12, 0.68, 0.28),
                rgb(0.00, 0.95, 1.00, 0.95), rgb(0.00, 0.92, 1.00, 0.16),
                rgb(1.00, 0.20, 0.62, 0.95), rgb(0.65, 1.00, 0.25, 0.95),
                rgb(0.13, 0.07, 0.23), rgb(0.08, 0.04, 0.15),
                rgb(0.58, 0.18, 0.78, 0.75), rgb(0.07, 0.03, 0.14),
                rgb(0.96, 0.88, 1.00), rgb(1.00, 0.25, 0.68),
                rgb(0.22, 0.12, 0.36), rgb(0.09, 0.04, 0.16),
                rgb(0.75, 0.28, 0.92), rgb(0.04, 0.02, 0.08),
                rgb(0.92, 0.84, 0.98),
                rgb(1.00, 0.18, 0.58), rgb(1.00, 1.00, 1.00),
                rgb(0.07, 0.03, 0.13), rgb(0.82, 0.70, 0.94),
                rgb(1.00, 0.22, 0.65), // Neon Magenta
                rgb(0.00, 0.95, 1.00), // Laser Cyan
                rgb(1.00, 0.55, 0.15), // Sunset Orange
                rgb(0.55, 1.00, 0.35)  // Cyber Lime
            };
        default: // 0: OBSIDIAN RACK (Modern Flagship Studio Dark)
            return {
                "OBSIDIAN RACK", false,
                rgb(0.11, 0.12, 0.14), rgb(0.06, 0.07, 0.08),
                rgb(0.05, 0.05, 0.06),
                rgb(0.08, 0.09, 0.11), rgb(0.24, 0.25, 0.28),
                rgb(1.00, 0.68, 0.18), rgb(0.65, 0.67, 0.70),
                rgb(0.03, 0.05, 0.07), rgb(0.01, 0.02, 0.03),
                rgb(0.18, 0.86, 0.95, 0.38), rgb(0.12, 0.25, 0.30, 0.28),
                rgb(0.22, 0.88, 0.98, 0.88), rgb(0.15, 0.78, 0.95, 0.14),
                rgb(1.00, 0.68, 0.18, 0.92), rgb(0.35, 0.95, 0.55, 0.92),
                rgb(0.13, 0.14, 0.17), rgb(0.09, 0.10, 0.12),
                rgb(0.22, 0.23, 0.26), rgb(0.06, 0.07, 0.09),
                rgb(0.92, 0.94, 0.96), rgb(1.00, 0.65, 0.18, 0.88),
                rgb(0.24, 0.26, 0.30), rgb(0.09, 0.10, 0.12),
                rgb(0.36, 0.38, 0.42), rgb(0.05, 0.05, 0.06),
                rgb(0.80, 0.82, 0.85),
                rgb(0.95, 0.58, 0.12, 0.94), rgb(0.05, 0.05, 0.05),
                rgb(0.06, 0.07, 0.09), rgb(0.78, 0.80, 0.83),
                rgb(1.00, 0.65, 0.15), // Amber
                rgb(0.18, 0.86, 0.95), // Cyan
                rgb(1.00, 0.38, 0.28), // Coral
                rgb(0.35, 0.92, 0.55)  // Lime
            };
    }
}

static inline NSColor* colorForRole(const SkinTheme& theme, ColorRole role) {
    switch (role) {
        case kRolePrimary:   return theme.accentPrimary;
        case kRoleSecondary: return theme.accentSecondary;
        case kRoleDrive:     return theme.accentDrive;
        case kRoleDynamics:  return theme.accentDynamics;
    }
    return theme.accentPrimary;
}

} // namespace vsr

#ifndef VSR9512XHardwareView
#define VSR9512XHardwareView VSR9512XHardwareView_Standalone
#endif

@interface VSR9512XHardwareView : NSView {
@public
    vsr::VSR9512XEngine* _engine;
    std::function<void(int, float)> _onParamChanged;
    NSTimer* _uiTimer;
    int _activeKnobIdx;
    CGFloat _dragStartY;
    float _dragStartNorm;
    int _hoverKnobIdx;
    int _flashPadIdx;
    int _flashPadTicks;
    std::vector<vsr::KnobControl> _knobs;
    std::vector<vsr::ToggleControl> _toggles;
    std::vector<vsr::ActionButton> _buttons;
}

- (instancetype)initWithFrame:(NSRect)frame engine:(vsr::VSR9512XEngine*)engine;
- (void)setParamCallback:(std::function<void(int, float)>)cb;
@end

@implementation VSR9512XHardwareView

- (instancetype)initWithFrame:(NSRect)frame engine:(vsr::VSR9512XEngine*)engine {
    self = [super initWithFrame:frame];
    if (self) {
        _engine = engine;
        _activeKnobIdx = -1;
        _hoverKnobIdx = -1;
        _flashPadIdx = -1;
        _flashPadTicks = 0;
        [self buildLayout];
        _uiTimer = [NSTimer scheduledTimerWithTimeInterval:(1.0 / 30.0)
                                                    target:self
                                                  selector:@selector(onTick:)
                                                  userInfo:nil
                                                   repeats:YES];
        [[NSRunLoop currentRunLoop] addTimer:_uiTimer forMode:NSRunLoopCommonModes];
    }
    return self;
}

- (void)dealloc {
    [_uiTimer invalidate];
    _uiTimer = nil;
#if !__has_feature(objc_arc)
    [super dealloc];
#endif
}

- (BOOL)isFlipped { return YES; }
- (BOOL)acceptsFirstResponder { return YES; }

- (void)setParamCallback:(std::function<void(int, float)>)cb {
    _onParamChanged = cb;
}

- (void)onTick:(NSTimer*)timer {
    if (_flashPadTicks > 0) {
        _flashPadTicks--;
        if (_flashPadTicks == 0) _flashPadIdx = -1;
    }
    [self setNeedsDisplay:YES];
}

- (void)buildLayout {
    _knobs.clear();
    _toggles.clear();
    _buttons.clear();

    // Top Preset, Skin & Demo Groove Controls
    _buttons.push_back({ 103, NSMakeRect(292, 18, 118, 26), @"SKIN", @"" });
    _buttons.push_back({ 100, NSMakeRect(416, 18, 26, 26),  @"◀", @"" });
    _buttons.push_back({ 101, NSMakeRect(654, 18, 26, 26),  @"▶", @"" });
    _buttons.push_back({ 102, NSMakeRect(688, 18, 120, 26), @"DEMO GROOVE", @"" });

    // MODULE 1: PREAMP & CIRCUIT MODEL (x: 28..252, y: 232..576)
    NSString* models[6] = { @"A: S950", @"B: SP1200", @"C: S900", @"D: MPC60", @"E: μ-LAW", @"F: PAULA" };
    for (int i = 0; i < 6; ++i) {
        int col = i % 3;
        int row = i / 3;
        _toggles.push_back({ vsr::kParamMachineType, NSMakeRect(38 + col * 68, 266 + row * 28, 64, 23), models[i], i });
    }
    _knobs.push_back({ vsr::kParamInputGain,  NSMakeRect(42,  338, 62, 62), @"IN GAIN",   vsr::kRolePrimary });
    _knobs.push_back({ vsr::kParamDrive,      NSMakeRect(114, 338, 62, 62), @"DRIVE",     vsr::kRoleDrive });
    _knobs.push_back({ vsr::kParamDiskNoise,  NSMakeRect(184, 338, 62, 62), @"DISK FLR",  vsr::kRolePrimary });

    _toggles.push_back({ vsr::kParamOverdrive, NSMakeRect(44,  436, 94, 26), @"SP OVERDRIVE", -1 });
    _toggles.push_back({ vsr::kParamMuLaw,     NSMakeRect(148, 436, 94, 26), @"MpoweR μ-LAW", -1 });

    // MODULE 2: RESAMPLER & DAC CLOCK (x: 262..472, y: 232..576)
    _knobs.push_back({ vsr::kParamBandwidth,     NSMakeRect(274, 280, 62, 62), @"BANDWIDTH", vsr::kRoleSecondary });
    _knobs.push_back({ vsr::kParamDistruktShift, NSMakeRect(340, 280, 62, 62), @"DISTRUKT",  vsr::kRoleDrive });
    _knobs.push_back({ vsr::kParamBitDepth,      NSMakeRect(404, 280, 62, 62), @"BIT DEPTH", vsr::kRoleSecondary });

    _buttons.push_back({ 200, NSMakeRect(274, 386, 60, 34), @"19.2k", @"S950" });
    _buttons.push_back({ 201, NSMakeRect(338, 386, 60, 34), @"26.0k", @"SP1200" });
    _buttons.push_back({ 202, NSMakeRect(402, 386, 60, 34), @"40.0k", @"MPC60" });

    // MODULE 3: DUAL VINTAGE FILTER & BRILLIANCE (x: 482..712, y: 232..576)
    _toggles.push_back({ vsr::kParamLpfMode, NSMakeRect(492, 266, 66, 23), @"S950 36dB", 0 });
    _toggles.push_back({ vsr::kParamLpfMode, NSMakeRect(562, 266, 66, 23), @"SSM2044",   1 });
    _toggles.push_back({ vsr::kParamLpfMode, NSMakeRect(632, 266, 68, 23), @"DYN ENV",   2 });

    _knobs.push_back({ vsr::kParamLpfCutoff,    NSMakeRect(494, 304, 58, 58), @"LPF FREQ", vsr::kRoleSecondary });
    _knobs.push_back({ vsr::kParamLpfResonance, NSMakeRect(566, 304, 58, 58), @"LPF RES",  vsr::kRoleSecondary });
    _knobs.push_back({ vsr::kParamBrilliance,   NSMakeRect(638, 304, 58, 58), @"BRILLNCE", vsr::kRolePrimary });

    _knobs.push_back({ vsr::kParamHpfCutoff,    NSMakeRect(494, 396, 58, 58), @"HPF FREQ", vsr::kRoleDynamics });
    _knobs.push_back({ vsr::kParamHpfResonance, NSMakeRect(566, 396, 58, 58), @"HPF RES",  vsr::kRoleDynamics });

    _toggles.push_back({ vsr::kParamLpfPower, NSMakeRect(636, 398, 62, 24), @"LPF ON", -1 });
    _toggles.push_back({ vsr::kParamHpfPower, NSMakeRect(636, 430, 62, 24), @"HPF ON", -1 });

    // MODULE 4: MAX1 DYNAMICS, FATTEN & MASTERING (x: 722..952, y: 232..576)
    _knobs.push_back({ vsr::kParamCompThreshold, NSMakeRect(732, 268, 52, 52), @"THRESH",  vsr::kRoleDynamics });
    _knobs.push_back({ vsr::kParamCompRatio,     NSMakeRect(786, 268, 52, 52), @"RATIO",   vsr::kRoleDynamics });
    _knobs.push_back({ vsr::kParamCompAttack,    NSMakeRect(840, 268, 52, 52), @"ATTACK",  vsr::kRoleDynamics });
    _knobs.push_back({ vsr::kParamCompRelease,   NSMakeRect(894, 268, 52, 52), @"RELEASE", vsr::kRoleDynamics });

    _knobs.push_back({ vsr::kParamFatten,        NSMakeRect(732, 356, 52, 52), @"FATTEN",  vsr::kRoleDrive });
    _knobs.push_back({ vsr::kParamStereoWidth,   NSMakeRect(786, 356, 52, 52), @"WIDTH",   vsr::kRoleSecondary });
    _knobs.push_back({ vsr::kParamDryWetMix,     NSMakeRect(840, 356, 52, 52), @"MIX",     vsr::kRolePrimary });
    _knobs.push_back({ vsr::kParamOutputLevel,   NSMakeRect(894, 356, 52, 52), @"OUTPUT",  vsr::kRolePrimary });

    _toggles.push_back({ vsr::kParamCompFeedback,    NSMakeRect(732, 438, 68, 24), @"FEEDBACK", -1 });
    _toggles.push_back({ vsr::kParamCompAutoRelease, NSMakeRect(804, 438, 68, 24), @"AUTO REL", -1 });
    _toggles.push_back({ vsr::kParamSoftClip,        NSMakeRect(876, 438, 68, 24), @"SOFT CLIP",-1 });
    _toggles.push_back({ vsr::kParamMono,            NSMakeRect(44,  468, 94, 24), @"MONO OUT", -1 });
    _toggles.push_back({ vsr::kParamAutoGain,        NSMakeRect(148, 468, 94, 24), @"AUTO GAIN",-1 });

    // Bottom 8-Slot Hardware Preset Memory Bank Strip (y: 514..612)
    NSString* bankNames[8] = {
        @"BANK 01", @"BANK 02", @"BANK 03", @"BANK 04",
        @"BANK 05", @"BANK 06", @"BANK 07", @"BANK 08"
    };
    NSString* bankSubs[8] = {
        @"S950+SP12", @"SP12 GRIT", @"RX950 LPF", @"μ-LAW 8B",
        @"MAX1 SLAM", @"SSM2044",   @"PAULA 28K", @"DAC AIR"
    };
    for (int p = 0; p < 8; ++p) {
        _buttons.push_back({ p, NSMakeRect(38 + p * 114, 540, 104, 56), bankNames[p], bankSubs[p] });
    }
}

- (void)formatParamValue:(int)paramId buf:(char*)buf size:(size_t)sz {
    float v = _engine ? _engine->getParam(paramId) : vsr::kParamTable[paramId].defaultVal;
    const auto& m = vsr::kParamTable[paramId];
    if (paramId == vsr::kParamBandwidth) {
        if (v >= 10000.0f) snprintf(buf, sz, "%.1f kHz", v * 0.001f);
        else snprintf(buf, sz, "%.0f Hz", v);
    } else if (paramId == vsr::kParamLpfCutoff) {
        if (v >= 1000.0f) snprintf(buf, sz, "%.2f kHz", v * 0.001f);
        else snprintf(buf, sz, "%.0f Hz", v);
    } else if (paramId == vsr::kParamHpfCutoff) {
        snprintf(buf, sz, "%.0f Hz", v);
    } else if (paramId == vsr::kParamBitDepth) {
        snprintf(buf, sz, "%.1f bit", v);
    } else if (paramId == vsr::kParamDistruktShift) {
        snprintf(buf, sz, "%+.1f st", v);
    } else if (paramId == vsr::kParamCompRatio) {
        snprintf(buf, sz, "%.1f:1", v);
    } else if (strlen(m.unit) > 0) {
        snprintf(buf, sz, "%.1f %s", v, m.unit);
    } else {
        snprintf(buf, sz, "%.1f", v);
    }
}

- (void)drawScrewAt:(NSPoint)pt {
    NSRect r = NSMakeRect(pt.x - 6, pt.y - 6, 12, 12);
    [[NSColor colorWithCalibratedWhite:0.08 alpha:1.0] setFill];
    [[NSBezierPath bezierPathWithOvalInRect:r] fill];
    [[NSColor colorWithCalibratedWhite:0.32 alpha:1.0] setStroke];
    [[NSBezierPath bezierPathWithOvalInRect:r] stroke];
    NSBezierPath* slot = [NSBezierPath bezierPath];
    [slot moveToPoint:NSMakePoint(pt.x - 3.5, pt.y - 3.5)];
    [slot lineToPoint:NSMakePoint(pt.x + 3.5, pt.y + 3.5)];
    [slot setLineWidth:1.4];
    [[NSColor colorWithCalibratedWhite:0.48 alpha:1.0] setStroke];
    [slot stroke];
}

- (void)drawLEDMeterAt:(NSRect)r value:(float)peak label:(NSString*)lbl {
    NSDictionary* lblAttrs = @{
        NSFontAttributeName: [NSFont monospacedSystemFontOfSize:9.0 weight:NSFontWeightBold],
        NSForegroundColorAttributeName: [NSColor colorWithCalibratedWhite:0.75 alpha:1.0]
    };
    [lbl drawAtPoint:NSMakePoint(r.origin.x, r.origin.y - 1) withAttributes:lblAttrs];

    CGFloat barX = r.origin.x + 34;
    CGFloat barW = r.size.width - 34;
    int numSegs = 20;
    CGFloat segW = (barW - (numSegs - 1) * 2.0) / numSegs;
    float db = 20.0f * std::log10(std::max(peak, 1e-4f));
    float norm = std::clamp((db + 48.0f) / 51.0f, 0.0f, 1.0f);
    int litSegs = static_cast<int>(std::round(norm * numSegs));

    for (int i = 0; i < numSegs; ++i) {
        NSRect sr = NSMakeRect(barX + i * (segW + 2.0), r.origin.y + 1, segW, r.size.height - 2);
        bool lit = (i < litSegs);
        NSColor* c = nil;
        if (i >= 18) {
            c = lit ? [NSColor colorWithCalibratedRed:1.0 green:0.22 blue:0.20 alpha:1.0]
                    : [NSColor colorWithCalibratedRed:0.25 green:0.08 blue:0.08 alpha:1.0];
        } else if (i >= 14) {
            c = lit ? [NSColor colorWithCalibratedRed:1.0 green:0.75 blue:0.15 alpha:1.0]
                    : [NSColor colorWithCalibratedRed:0.24 green:0.18 blue:0.06 alpha:1.0];
        } else {
            c = lit ? [NSColor colorWithCalibratedRed:0.22 green:0.92 blue:0.52 alpha:1.0]
                    : [NSColor colorWithCalibratedRed:0.07 green:0.20 blue:0.12 alpha:1.0];
        }
        [c setFill];
        [[NSBezierPath bezierPathWithRoundedRect:sr xRadius:1.5 yRadius:1.5] fill];
    }
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    NSRect bounds = [self bounds];

    int skinIdx = _engine ? _engine->getSkin() : 0;
    vsr::SkinTheme theme = vsr::getSkinTheme(skinIdx);

    // 1. Studio Rack Enclosure Background
    NSGradient* chassisGrad = [[NSGradient alloc] initWithStartingColor:theme.chassisTop
                                                            endingColor:theme.chassisBot];
    [chassisGrad drawInRect:bounds angle:90.0];

    // Rack Ears (left and right 22px strips)
    [theme.rackEar setFill];
    NSRectFill(NSMakeRect(0, 0, 22, bounds.size.height));
    NSRectFill(NSMakeRect(bounds.size.width - 22, 0, 22, bounds.size.height));

    [self drawScrewAt:NSMakePoint(11, 28)];
    [self drawScrewAt:NSMakePoint(11, bounds.size.height - 28)];
    [self drawScrewAt:NSMakePoint(bounds.size.width - 11, 28)];
    [self drawScrewAt:NSMakePoint(bounds.size.width - 11, bounds.size.height - 28)];

    // Top Header Bar
    NSRect headerRect = NSMakeRect(28, 10, bounds.size.width - 56, 44);
    [theme.headerBg setFill];
    [[NSBezierPath bezierPathWithRoundedRect:headerRect xRadius:6 yRadius:6] fill];
    [theme.headerBorder setStroke];
    [[NSBezierPath bezierPathWithRoundedRect:headerRect xRadius:6 yRadius:6] stroke];

    NSDictionary* titleAttrs = @{
        NSFontAttributeName: [NSFont systemFontOfSize:15.5 weight:NSFontWeightHeavy],
        NSForegroundColorAttributeName: theme.brandTitle
    };
    [@"VSR-9512X" drawAtPoint:NSMakePoint(38, 14) withAttributes:titleAttrs];

    NSDictionary* subTitleAttrs = @{
        NSFontAttributeName: [NSFont monospacedSystemFontOfSize:8.5 weight:NSFontWeightSemibold],
        NSForegroundColorAttributeName: theme.brandSub
    };
    [@"S950 • SP-1200 • SAMPLEX V3 • MAX1 FX" drawAtPoint:NSMakePoint(38, 34) withAttributes:subTitleAttrs];

    // Preset Display Box
    NSRect presetBox = NSMakeRect(446, 18, 204, 26);
    [theme.screenBot setFill];
    [[NSBezierPath bezierPathWithRoundedRect:presetBox xRadius:4 yRadius:4] fill];
    [theme.screenBorder setStroke];
    [[NSBezierPath bezierPathWithRoundedRect:presetBox xRadius:4 yRadius:4] stroke];

    int curPreset = _engine ? _engine->getCurrentPreset() : 0;
    NSString* presetName = [NSString stringWithUTF8String:vsr::kFactoryPresets[curPreset].name];
    NSDictionary* presetAttrs = @{
        NSFontAttributeName: [NSFont monospacedSystemFontOfSize:10.0 weight:NSFontWeightBold],
        NSForegroundColorAttributeName: theme.eqStroke
    };
    [presetName drawInRect:NSMakeRect(452, 23, 192, 18) withAttributes:presetAttrs];

    // Top-Right RX950 Style Input/Output LED Peak Meters
    float inPeak = _engine ? std::max(_engine->getInputPeakL(), _engine->getInputPeakR()) : 0.0f;
    float outPeak = _engine ? std::max(_engine->getOutputPeakL(), _engine->getOutputPeakR()) : 0.0f;
    [self drawLEDMeterAt:NSMakeRect(818, 16, 126, 11) value:inPeak label:@"IN"];
    [self drawLEDMeterAt:NSMakeRect(818, 32, 126, 11) value:outPeak label:@"OUT"];

    // 2. Central OLED / Backlit LCD Dual Analyzer & Filter Response Display (y: 62..222)
    NSRect oledRect = NSMakeRect(28, 62, bounds.size.width - 56, 160);
    NSGradient* oledGrad = [[NSGradient alloc] initWithStartingColor:theme.screenTop
                                                         endingColor:theme.screenBot];
    [oledGrad drawInRect:oledRect angle:90.0];
    [theme.screenBorder setStroke];
    [[NSBezierPath bezierPathWithRoundedRect:oledRect xRadius:8 yRadius:8] stroke];

    // Draw Screen Grid Lines
    [theme.screenGrid setStroke];
    for (int gx = 1; gx < 12; ++gx) {
        CGFloat x = oledRect.origin.x + (oledRect.size.width / 12.0) * gx;
        [NSBezierPath strokeLineFromPoint:NSMakePoint(x, oledRect.origin.y + 4)
                                  toPoint:NSMakePoint(x, oledRect.origin.y + oledRect.size.height - 4)];
    }
    for (int gy = 1; gy < 4; ++gy) {
        CGFloat y = oledRect.origin.y + (oledRect.size.height / 4.0) * gy;
        [NSBezierPath strokeLineFromPoint:NSMakePoint(oledRect.origin.x + 4, y)
                                  toPoint:NSMakePoint(oledRect.origin.x + oledRect.size.width - 4, y)];
    }

    if (_engine) {
        float scope[256];
        float grHist[128];
        _engine->copyScopeData(scope, grHist);

        NSRect wavePane = NSMakeRect(36, 70, 560, 144);

        // Filter Response Curve (HPF + LPF + Brilliance Shelf)
        NSBezierPath* eqPath = [NSBezierPath bezierPath];
        NSBezierPath* eqFill = [NSBezierPath bezierPath];
        float hpfCut = _engine->getParam(vsr::kParamHpfCutoff);
        float hpfRes = _engine->getParam(vsr::kParamHpfResonance) * 0.01f;
        bool hpfOn = _engine->getParam(vsr::kParamHpfPower) >= 0.5f;
        float lpfCut = _engine->getParam(vsr::kParamLpfCutoff);
        float lpfRes = _engine->getParam(vsr::kParamLpfResonance) * 0.01f;
        bool lpfOn = _engine->getParam(vsr::kParamLpfPower) >= 0.5f;
        int lpfMode = static_cast<int>(std::round(_engine->getParam(vsr::kParamLpfMode)));
        float brillDb = _engine->getParam(vsr::kParamBrilliance);
        float fattenDb = _engine->getParam(vsr::kParamFatten) * 0.085f;

        [eqFill moveToPoint:NSMakePoint(wavePane.origin.x, wavePane.origin.y + wavePane.size.height)];
        for (int px = 0; px <= 140; ++px) {
            float normX = static_cast<float>(px) / 140.0f;
            float freq = 20.0f * std::pow(1000.0f, normX);
            float db = 0.0f;

            if (hpfOn) {
                float r = hpfCut / std::max(freq, 1.0f);
                db -= 12.0f * std::log2(1.0f + r * r * r * r);
                float resDist = std::abs( std::log2(freq / std::max(hpfCut, 20.0f)) );
                if (resDist < 0.6f) db += hpfRes * 10.0f * (1.0f - resDist / 0.6f);
            }
            if (lpfOn) {
                float r = freq / std::max(lpfCut, 50.0f);
                float orderSlope = (lpfMode == 0) ? 18.0f : 12.0f;
                db -= orderSlope * std::log2(1.0f + std::pow(r, (lpfMode == 0) ? 6.0f : 4.0f));
                float resDist = std::abs( std::log2(freq / std::max(lpfCut, 50.0f)) );
                if (resDist < 0.55f) db += lpfRes * 14.0f * (1.0f - resDist / 0.55f);
            }
            if (freq > 2500.0f) {
                float shelfT = std::clamp(std::log2(freq / 2500.0f) / 2.0f, 0.0f, 1.0f);
                db += brillDb * shelfT;
            }
            if (freq < 300.0f) {
                float fatT = std::clamp(std::log2(300.0f / std::max(freq, 20.0f)) / 2.0f, 0.0f, 1.0f);
                db += fattenDb * fatT;
            }

            CGFloat x = wavePane.origin.x + normX * wavePane.size.width;
            CGFloat y = wavePane.origin.y + wavePane.size.height * 0.52 - std::clamp(db, -28.0f, 18.0f) * 2.2;
            if (px == 0) [eqPath moveToPoint:NSMakePoint(x, y)];
            else [eqPath lineToPoint:NSMakePoint(x, y)];
            [eqFill lineToPoint:NSMakePoint(x, y)];
        }
        [eqFill lineToPoint:NSMakePoint(wavePane.origin.x + wavePane.size.width, wavePane.origin.y + wavePane.size.height)];
        [eqFill closePath];

        [theme.eqFill setFill];
        [eqFill fill];
        [eqPath setLineWidth:2.0];
        [theme.eqStroke setStroke];
        [eqPath stroke];

        // Live Audio Waveform Overlay
        NSBezierPath* wavePath = [NSBezierPath bezierPath];
        for (int i = 0; i < 256; ++i) {
            CGFloat x = wavePane.origin.x + (static_cast<CGFloat>(i) / 255.0) * wavePane.size.width;
            CGFloat y = wavePane.origin.y + wavePane.size.height * 0.5 - std::clamp(scope[i], -1.0f, 1.0f) * (wavePane.size.height * 0.42);
            if (i == 0) [wavePath moveToPoint:NSMakePoint(x, y)];
            else [wavePath lineToPoint:NSMakePoint(x, y)];
        }
        [wavePath setLineWidth:1.6];
        [theme.waveStroke setStroke];
        [wavePath stroke];

        // Right 38% of Screen: MAX1 Gain Reduction Time Graph & Hardware Telemetry Readout
        NSRect grPane = NSMakeRect(612, 70, 326, 144);
        [[theme.screenBot colorWithAlphaComponent:0.65] setFill];
        [[NSBezierPath bezierPathWithRoundedRect:grPane xRadius:5 yRadius:5] fill];

        NSBezierPath* grPath = [NSBezierPath bezierPath];
        for (int i = 0; i < 128; ++i) {
            CGFloat x = grPane.origin.x + 8 + (static_cast<CGFloat>(i) / 127.0) * (grPane.size.width - 16);
            float gr = std::clamp(grHist[i], 0.0f, 24.0f);
            CGFloat y = grPane.origin.y + 36 + (gr / 24.0f) * (grPane.size.height - 46);
            if (i == 0) [grPath moveToPoint:NSMakePoint(x, y)];
            else [grPath lineToPoint:NSMakePoint(x, y)];
        }
        [grPath setLineWidth:2.0];
        [theme.grStroke setStroke];
        [grPath stroke];

        int modelIdx = static_cast<int>(std::round(_engine->getParam(vsr::kParamMachineType)));
        const char* modelNames[6] = { "AKAI S950 12-BIT", "E-MU SP-1200", "AKAI S900 DAC", "LINN MPC60", "MpoweR μ-LAW 8", "AMIGA PAULA" };
        const char* lpfNames[3] = { "RX950 36dB BUTTERWORTH", "SP1200 SSM2044 LADDER", "RX1200 DYNAMIC ENV" };
        NSString* oledLeftTop = [NSString stringWithFormat:@"ENGINE: %s  |  CLOCK: %.2f kHz  |  QUANT: %.1f-BIT  |  %s",
                                 modelNames[std::clamp(modelIdx, 0, 5)],
                                 _engine->getParam(vsr::kParamBandwidth) * 0.001f,
                                 _engine->getParam(vsr::kParamBitDepth),
                                 lpfNames[std::clamp(lpfMode, 0, 2)]];
        NSDictionary* oledTextAttrs = @{
            NSFontAttributeName: [NSFont monospacedSystemFontOfSize:10.0 weight:NSFontWeightBold],
            NSForegroundColorAttributeName: theme.eqStroke
        };
        [oledLeftTop drawAtPoint:NSMakePoint(44, 74) withAttributes:oledTextAttrs];

        NSString* grHeader = [NSString stringWithFormat:@"MAX1 DYNAMICS // GR: -%.1f dB  |  FATTEN: %.0f%%",
                              std::abs(_engine->getGainReductionDb()),
                              _engine->getParam(vsr::kParamFatten)];
        NSDictionary* grTextAttrs = @{
            NSFontAttributeName: [NSFont monospacedSystemFontOfSize:10.0 weight:NSFontWeightBold],
            NSForegroundColorAttributeName: theme.grStroke
        };
        [grHeader drawAtPoint:NSMakePoint(620, 74) withAttributes:grTextAttrs];
    }

    // 3. Draw 4 Hardware Rack Module Panels (y: 232..508)
    struct BaySpec { NSRect rect; NSString* title; NSString* badge; };
    BaySpec bays[4] = {
        { NSMakeRect(28,  232, 226, 274), @"I. PREAMP & CIRCUIT",      @"AK9X•S12X" },
        { NSMakeRect(262, 232, 212, 274), @"II. CLOCK & QUANTIZER",    @"RX950•1200" },
        { NSMakeRect(482, 232, 232, 274), @"III. VINTAGE FILTERS",     @"MF6•SSM2044" },
        { NSMakeRect(722, 232, 230, 274), @"IV. MAX1 COMP & MASTER",   @"MAX1•WIDTH" }
    };

    NSDictionary* bayTitleAttrs = @{
        NSFontAttributeName: [NSFont systemFontOfSize:10.0 weight:NSFontWeightHeavy],
        NSForegroundColorAttributeName: theme.bayTitleText
    };
    NSMutableParagraphStyle* rightStyle = [[NSMutableParagraphStyle alloc] init];
    [rightStyle setAlignment:NSTextAlignmentRight];
    NSDictionary* bayBadgeAttrs = @{
        NSFontAttributeName: [NSFont monospacedSystemFontOfSize:8.0 weight:NSFontWeightBold],
        NSForegroundColorAttributeName: theme.bayBadgeText,
        NSParagraphStyleAttributeName: rightStyle
    };

    for (int b = 0; b < 4; ++b) {
        NSRect br = bays[b].rect;
        NSGradient* bayGrad = [[NSGradient alloc] initWithStartingColor:theme.bayTop
                                                            endingColor:theme.bayBot];
        [bayGrad drawInRect:br angle:90.0];
        [theme.bayBorder setStroke];
        [[NSBezierPath bezierPathWithRoundedRect:br xRadius:7 yRadius:7] stroke];

        NSRect hr = NSMakeRect(br.origin.x + 1, br.origin.y + 1, br.size.width - 2, 26);
        [theme.bayHeaderBg setFill];
        [[NSBezierPath bezierPathWithRoundedRect:hr xRadius:6 yRadius:6] fill];
        [bays[b].title drawAtPoint:NSMakePoint(br.origin.x + 8, br.origin.y + 7) withAttributes:bayTitleAttrs];
        [bays[b].badge drawInRect:NSMakeRect(br.origin.x + br.size.width - 92, br.origin.y + 8, 84, 14) withAttributes:bayBadgeAttrs];
    }

    // 4. Draw Bottom Hardware Memory Bank Bay (y: 514..612)
    NSRect padBay = NSMakeRect(28, 514, bounds.size.width - 56, 94);
    [theme.bayBot setFill];
    [[NSBezierPath bezierPathWithRoundedRect:padBay xRadius:7 yRadius:7] fill];
    [theme.bayBorder setStroke];
    [[NSBezierPath bezierPathWithRoundedRect:padBay xRadius:7 yRadius:7] stroke];

    NSRect padHeader = NSMakeRect(padBay.origin.x + 1, padBay.origin.y + 1, padBay.size.width - 2, 22);
    [theme.bayHeaderBg setFill];
    [[NSBezierPath bezierPathWithRoundedRect:padHeader xRadius:6 yRadius:6] fill];
    [@"V. HARDWARE MEMORY BANK // INSTANT STUDIO PRESET RECALL (BANKS 01 – 08)"
        drawAtPoint:NSMakePoint(40, 519) withAttributes:bayTitleAttrs];

    // 5. Draw All Hardware Toggle & Radio Buttons
    for (size_t i = 0; i < _toggles.size(); ++i) {
        const auto& t = _toggles[i];
        float curVal = _engine ? _engine->getParam(t.paramId) : vsr::kParamTable[t.paramId].defaultVal;
        bool isOn = (t.radioValue >= 0)
            ? (static_cast<int>(std::round(curVal)) == t.radioValue)
            : (curVal >= 0.5f);

        NSColor* bg = isOn ? theme.toggleOnBg : theme.toggleOffBg;
        [bg setFill];
        [[NSBezierPath bezierPathWithRoundedRect:t.rect xRadius:4 yRadius:4] fill];

        NSColor* border = isOn ? theme.brandTitle : theme.bayBorder;
        [border setStroke];
        [[NSBezierPath bezierPathWithRoundedRect:t.rect xRadius:4 yRadius:4] stroke];

        NSMutableParagraphStyle* pStyle = [[NSMutableParagraphStyle alloc] init];
        [pStyle setAlignment:NSTextAlignmentCenter];
        NSDictionary* tAttrs = @{
            NSFontAttributeName: [NSFont systemFontOfSize:9.0 weight:NSFontWeightBold],
            NSForegroundColorAttributeName: isOn ? theme.toggleOnText : theme.toggleOffText,
            NSParagraphStyleAttributeName: pStyle
        };
        [t.label drawInRect:NSMakeRect(t.rect.origin.x, t.rect.origin.y + (t.rect.size.height - 13) * 0.5, t.rect.size.width, 14)
             withAttributes:tAttrs];
    }

    // 6. Draw Action Buttons & 8 Preset Memory Bank Buttons
    for (size_t i = 0; i < _buttons.size(); ++i) {
        const auto& b = _buttons[i];
        bool active = false;
        if (b.actionId == 102 && _engine && _engine->isDemoLoopPlaying()) active = true;
        if (b.actionId >= 0 && b.actionId < 8 && _engine && _engine->getCurrentPreset() == b.actionId) active = true;

        NSColor* bg = active
            ? theme.accentSecondary
            : (b.actionId == 103 ? theme.toggleOnBg : theme.toggleOffBg);
        [bg setFill];
        [[NSBezierPath bezierPathWithRoundedRect:b.rect xRadius:5 yRadius:5] fill];

        NSColor* stroke = active ? [NSColor whiteColor] : theme.bayBorder;
        [stroke setStroke];
        [[NSBezierPath bezierPathWithRoundedRect:b.rect xRadius:5 yRadius:5] stroke];

        NSMutableParagraphStyle* pStyle = [[NSMutableParagraphStyle alloc] init];
        [pStyle setAlignment:NSTextAlignmentCenter];
        NSColor* fgColor = active
            ? (theme.isLightChassis ? [NSColor whiteColor] : [NSColor blackColor])
            : (b.actionId == 103 ? theme.toggleOnText : theme.toggleOffText);
        NSDictionary* bAttrs = @{
            NSFontAttributeName: [NSFont systemFontOfSize:(b.actionId < 8 ? 10.5 : 9.0) weight:NSFontWeightHeavy],
            NSForegroundColorAttributeName: fgColor,
            NSParagraphStyleAttributeName: pStyle
        };

        if (b.actionId == 103) {
            NSString* skinLabel = [NSString stringWithFormat:@"SKIN: %s", theme.name];
            [skinLabel drawInRect:NSMakeRect(b.rect.origin.x, b.rect.origin.y + (b.rect.size.height - 13) * 0.5, b.rect.size.width, 14)
                   withAttributes:bAttrs];
        } else if ([b.sublabel length] > 0) {
            [b.label drawInRect:NSMakeRect(b.rect.origin.x, b.rect.origin.y + 5, b.rect.size.width, 15) withAttributes:bAttrs];
            NSDictionary* subAttrs = @{
                NSFontAttributeName: [NSFont monospacedSystemFontOfSize:8.5 weight:NSFontWeightBold],
                NSForegroundColorAttributeName: active ? fgColor : theme.brandTitle,
                NSParagraphStyleAttributeName: pStyle
            };
            [b.sublabel drawInRect:NSMakeRect(b.rect.origin.x, b.rect.origin.y + b.rect.size.height - 18, b.rect.size.width, 13) withAttributes:subAttrs];
        } else {
            [b.label drawInRect:NSMakeRect(b.rect.origin.x, b.rect.origin.y + (b.rect.size.height - 14) * 0.5, b.rect.size.width, 16) withAttributes:bAttrs];
        }
    }

    // 7. Draw 3D Machined Rotary Knobs with Glowing Arc & Digital Value Readout
    for (size_t i = 0; i < _knobs.size(); ++i) {
        const auto& k = _knobs[i];
        NSColor* accent = vsr::colorForRole(theme, k.role);
        float norm = _engine ? _engine->getParamNormalized(k.paramId) : 0.5f;
        NSPoint center = NSMakePoint(NSMidX(k.rect), NSMidY(k.rect));
        CGFloat radius = k.rect.size.width * 0.42;

        CGFloat startDeg = 135.0;
        CGFloat endDeg = 405.0;
        CGFloat valDeg = startDeg + norm * (endDeg - startDeg);
        CGFloat arcR = radius + 4.0;

        NSBezierPath* trackArc = [NSBezierPath bezierPath];
        for (int s = 0; s <= 36; ++s) {
            double deg = startDeg + (endDeg - startDeg) * (static_cast<double>(s) / 36.0);
            double r = deg * (M_PI / 180.0);
            NSPoint pt = NSMakePoint(center.x + std::cos(r) * arcR, center.y + std::sin(r) * arcR);
            if (s == 0) [trackArc moveToPoint:pt];
            else [trackArc lineToPoint:pt];
        }
        [trackArc setLineWidth:3.0];
        [theme.knobTrack setStroke];
        [trackArc stroke];

        if (norm > 0.005f) {
            NSBezierPath* valArc = [NSBezierPath bezierPath];
            int steps = std::max(2, static_cast<int>(std::ceil(norm * 36.0f)));
            for (int s = 0; s <= steps; ++s) {
                double deg = startDeg + (valDeg - startDeg) * (static_cast<double>(s) / steps);
                double r = deg * (M_PI / 180.0);
                NSPoint pt = NSMakePoint(center.x + std::cos(r) * arcR, center.y + std::sin(r) * arcR);
                if (s == 0) [valArc moveToPoint:pt];
                else [valArc lineToPoint:pt];
            }
            [valArc setLineWidth:3.2];
            [accent setStroke];
            [valArc stroke];
        }

        // Knob Body
        NSRect bodyRect = NSMakeRect(center.x - radius, center.y - radius, radius * 2.0, radius * 2.0);
        NSGradient* knobGrad = [[NSGradient alloc] initWithStartingColor:theme.knobTop
                                                             endingColor:theme.knobBot];
        [knobGrad drawInBezierPath:[NSBezierPath bezierPathWithOvalInRect:bodyRect] angle:90.0];
        [theme.knobBorder setStroke];
        [[NSBezierPath bezierPathWithOvalInRect:bodyRect] stroke];

        // Pointer Line
        double rad = valDeg * (M_PI / 180.0);
        NSPoint p1 = NSMakePoint(center.x + std::cos(rad) * (radius * 0.28), center.y + std::sin(rad) * (radius * 0.28));
        NSPoint p2 = NSMakePoint(center.x + std::cos(rad) * (radius * 0.88), center.y + std::sin(rad) * (radius * 0.88));
        NSBezierPath* ptr = [NSBezierPath bezierPath];
        [ptr moveToPoint:p1];
        [ptr lineToPoint:p2];
        [ptr setLineWidth:2.4];
        [accent setStroke];
        [ptr stroke];

        // Title & Value readout below knob
        NSMutableParagraphStyle* pStyle = [[NSMutableParagraphStyle alloc] init];
        [pStyle setAlignment:NSTextAlignmentCenter];
        NSDictionary* kTitleAttrs = @{
            NSFontAttributeName: [NSFont systemFontOfSize:8.5 weight:NSFontWeightBold],
            NSForegroundColorAttributeName: theme.labelText,
            NSParagraphStyleAttributeName: pStyle
        };
        [k.title drawInRect:NSMakeRect(k.rect.origin.x - 8, k.rect.origin.y + k.rect.size.height + 1, k.rect.size.width + 16, 12)
             withAttributes:kTitleAttrs];

        char valBuf[32];
        [self formatParamValue:k.paramId buf:valBuf size:sizeof(valBuf)];
        NSDictionary* kValAttrs = @{
            NSFontAttributeName: [NSFont monospacedSystemFontOfSize:8.5 weight:NSFontWeightBold],
            NSForegroundColorAttributeName: accent,
            NSParagraphStyleAttributeName: pStyle
        };
        [[NSString stringWithUTF8String:valBuf]
            drawInRect:NSMakeRect(k.rect.origin.x - 10, k.rect.origin.y + k.rect.size.height + 12, k.rect.size.width + 20, 12)
        withAttributes:kValAttrs];
    }
}

- (void)mouseDown:(NSEvent*)event {
    NSPoint pt = [self convertPoint:[event locationInWindow] fromView:nil];

    // Check knobs first
    for (size_t i = 0; i < _knobs.size(); ++i) {
        if (NSPointInRect(pt, _knobs[i].rect)) {
            int pid = _knobs[i].paramId;
            if ([event clickCount] >= 2) {
                float defVal = vsr::kParamTable[pid].defaultVal;
                if (_engine) _engine->setParam(pid, defVal);
                if (_onParamChanged) _onParamChanged(pid, defVal);
                [self setNeedsDisplay:YES];
                return;
            }
            _activeKnobIdx = static_cast<int>(i);
            _dragStartY = pt.y;
            _dragStartNorm = _engine ? _engine->getParamNormalized(pid) : 0.5f;
            return;
        }
    }

    // Check toggles / radio buttons
    for (size_t i = 0; i < _toggles.size(); ++i) {
        if (NSPointInRect(pt, _toggles[i].rect)) {
            int pid = _toggles[i].paramId;
            float newVal = 0.0f;
            if (_toggles[i].radioValue >= 0) {
                newVal = static_cast<float>(_toggles[i].radioValue);
            } else {
                float cur = _engine ? _engine->getParam(pid) : 0.0f;
                newVal = (cur >= 0.5f) ? 0.0f : 1.0f;
            }
            if (_engine) _engine->setParam(pid, newVal);
            if (_onParamChanged) _onParamChanged(pid, newVal);
            [self setNeedsDisplay:YES];
            return;
        }
    }

    // Check action buttons & 8 Preset Memory Bank buttons
    for (size_t i = 0; i < _buttons.size(); ++i) {
        if (NSPointInRect(pt, _buttons[i].rect)) {
            int act = _buttons[i].actionId;
            if (act >= 0 && act < 8 && _engine) {
                _engine->loadPreset(act);
                for (int k = 0; k < vsr::kNumParams; ++k) {
                    if (_onParamChanged) _onParamChanged(k, _engine->getParam(k));
                }
            } else if (act == 100 && _engine) {
                int p = (_engine->getCurrentPreset() - 1 + vsr::kNumPresets) % vsr::kNumPresets;
                _engine->loadPreset(p);
                for (int k = 0; k < vsr::kNumParams; ++k) {
                    if (_onParamChanged) _onParamChanged(k, _engine->getParam(k));
                }
            } else if (act == 101 && _engine) {
                int p = (_engine->getCurrentPreset() + 1) % vsr::kNumPresets;
                _engine->loadPreset(p);
                for (int k = 0; k < vsr::kNumParams; ++k) {
                    if (_onParamChanged) _onParamChanged(k, _engine->getParam(k));
                }
            } else if (act == 102 && _engine) {
                _engine->setDemoLoopPlaying(!_engine->isDemoLoopPlaying());
            } else if (act == 103 && _engine) {
                _engine->setSkin(_engine->getSkin() + 1);
            } else if (act == 200 && _engine) {
                _engine->setParam(vsr::kParamBandwidth, 19200.0f);
                if (_onParamChanged) _onParamChanged(vsr::kParamBandwidth, 19200.0f);
            } else if (act == 201 && _engine) {
                _engine->setParam(vsr::kParamBandwidth, 26040.0f);
                if (_onParamChanged) _onParamChanged(vsr::kParamBandwidth, 26040.0f);
            } else if (act == 202 && _engine) {
                _engine->setParam(vsr::kParamBandwidth, 40000.0f);
                if (_onParamChanged) _onParamChanged(vsr::kParamBandwidth, 40000.0f);
            }
            [self setNeedsDisplay:YES];
            return;
        }
    }
}

- (void)mouseDragged:(NSEvent*)event {
    if (_activeKnobIdx < 0 || _activeKnobIdx >= static_cast<int>(_knobs.size())) return;
    NSPoint pt = [self convertPoint:[event locationInWindow] fromView:nil];
    CGFloat dy = _dragStartY - pt.y;
    float sensitivity = ([event modifierFlags] & NSEventModifierFlagShift) ? 450.0f : 140.0f;
    float newNorm = std::clamp(_dragStartNorm + static_cast<float>(dy) / sensitivity, 0.0f, 1.0f);
    int pid = _knobs[_activeKnobIdx].paramId;
    if (_engine) {
        _engine->setParamNormalized(pid, newNorm);
        if (_onParamChanged) _onParamChanged(pid, _engine->getParam(pid));
    }
    [self setNeedsDisplay:YES];
}

- (void)mouseUp:(NSEvent*)event {
    _activeKnobIdx = -1;
}

- (void)keyDown:(NSEvent*)event {
    NSString* chars = [event charactersIgnoringModifiers];
    if ([chars length] > 0) {
        unichar c = [chars characterAtIndex:0];
        if (c >= '1' && c <= '8') {
            int bank = c - '1';
            if (_engine) _engine->loadPreset(bank);
            [self setNeedsDisplay:YES];
            return;
        } else if (c == 's' || c == 'S') {
            if (_engine) _engine->setSkin(_engine->getSkin() + 1);
            [self setNeedsDisplay:YES];
            return;
        } else if (c == ' ') {
            if (_engine) _engine->setDemoLoopPlaying(!_engine->isDemoLoopPlaying());
            [self setNeedsDisplay:YES];
            return;
        }
    }
    [super keyDown:event];
}

@end
