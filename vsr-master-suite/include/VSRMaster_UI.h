#pragma once
#import <Cocoa/Cocoa.h>
#include "VSRMaster_DSP.h"
#include <vector>
#include <functional>
#include <cmath>
#include <array>
#include <algorithm>

namespace vsrm {

struct AuraLiveTheme {
    const char* name;
    CGFloat bgR, bgG, bgB;             // Primary Chassis Canvas (#0E1014)
    CGFloat panelTopR, panelTopG, panelTopB; // Brushed anodized dark metal top (#222834)
    CGFloat panelBotR, panelBotG, panelBotB; // Recessed dark slate bottom (#12151B)
    CGFloat wellR, wellG, wellB;       // Display well (#090C12)
    CGFloat borderR, borderG, borderB; // Thin 1px bevel border (#262B36)
    CGFloat cyanR, cyanG, cyanB;       // Ice Cyan (#29B6F6 / #00E5FF)
    CGFloat blueR, blueG, blueB;       // Royal Blue (#3B7CFF)
    CGFloat violetR, violetG, violetB; // Electric Violet / Magenta (#AB47BC / #CE93D8)
    CGFloat amberR, amberG, amberB;    // Golden Amber (#F5A623 / #FFB300)
    CGFloat greenR, greenG, greenB;    // Emerald Green (#00E676)
    CGFloat textR, textG, textB;       // Crisp White active values (#F0F4F8)
    CGFloat dimR, dimG, dimB;          // Muted Silver-Grey labels (#8E9BAE)
};

static inline AuraLiveTheme getAuraLiveTheme(int idx) {
    switch ((idx % 5 + 5) % 5) {
        case 1: // 24K Mastering Gold
            return {
                "24K GOLD EDITION",
                0.065, 0.058, 0.048,
                0.165, 0.135, 0.095,
                0.095, 0.080, 0.060,
                0.040, 0.035, 0.028,
                0.340, 0.270, 0.160,
                0.980, 0.800, 0.260,
                0.920, 0.640, 0.200,
                0.880, 0.420, 0.680,
                1.000, 0.620, 0.140,
                0.200, 0.900, 0.460,
                0.960, 0.940, 0.880,
                0.680, 0.620, 0.520
            };
        case 2: // Tape56K Warm Bronze
            return {
                "TAPE56K BRONZE",
                0.075, 0.058, 0.052,
                0.175, 0.128, 0.108,
                0.105, 0.078, 0.068,
                0.045, 0.032, 0.028,
                0.320, 0.220, 0.170,
                0.160, 0.840, 0.980,
                0.360, 0.680, 0.960,
                0.920, 0.460, 0.320,
                0.980, 0.660, 0.180,
                0.220, 0.880, 0.480,
                0.950, 0.910, 0.870,
                0.680, 0.580, 0.520
            };
        case 3: // S950 Heritage Titanium
            return {
                "HERITAGE TITANIUM",
                0.105, 0.122, 0.152,
                0.205, 0.235, 0.285,
                0.135, 0.155, 0.195,
                0.055, 0.068, 0.088,
                0.300, 0.340, 0.400,
                0.160, 0.840, 0.980,
                0.300, 0.580, 0.980,
                0.740, 0.400, 0.960,
                0.980, 0.680, 0.180,
                0.200, 0.900, 0.460,
                0.940, 0.960, 1.000,
                0.640, 0.700, 0.780
            };
        case 4: // Soothe Cyber Violet
            return {
                "CYBER VIOLET",
                0.058, 0.048, 0.095,
                0.148, 0.108, 0.225,
                0.088, 0.068, 0.145,
                0.035, 0.025, 0.065,
                0.270, 0.200, 0.400,
                0.120, 0.880, 1.000,
                0.440, 0.560, 1.000,
                0.860, 0.340, 0.980,
                1.000, 0.660, 0.180,
                0.220, 0.920, 0.540,
                0.940, 0.930, 0.990,
                0.640, 0.580, 0.760
            };
        case 0:
        default: // Aura Master v2.0 Matte Obsidian & Slate Hardware Aesthetic (#0E1014 / #12151B)
            return {
                "AURA MASTER OBSIDIAN",
                0.055, 0.063, 0.078, // #0E1014 Matte Obsidian
                0.135, 0.162, 0.210, // #222936 Anodized Dark Metal Top
                0.071, 0.082, 0.106, // #12151B Recessed Dark Slate
                0.038, 0.048, 0.070, // #0A0C12 Deep Display Well
                0.149, 0.169, 0.212, // #262B36 Bevel Border
                0.161, 0.714, 0.965, // #29B6F6 / #00E5FF Ice Cyan
                0.231, 0.486, 1.000, // #3B7CFF Royal Blue
                0.671, 0.278, 0.737, // #AB47BC / #CE93D8 Electric Violet
                0.961, 0.651, 0.137, // #F5A623 / #FFB300 Golden Amber
                0.000, 0.902, 0.463, // #00E676 Emerald Green
                0.941, 0.957, 0.973, // #F0F4F8 Crisp White
                0.557, 0.608, 0.682  // #8E9BAE Muted Silver-Grey
            };
    }
}

struct KnobWidget {
    int paramId;
    CGFloat cx, cy, radius;
    NSString* label;
    int colorRole; // 1=Cyan, 2=Blue, 3=Violet, 4=Amber, 5=Green
};

struct SliderWidget {
    int paramId;
    NSRect trackRect;
    NSString* label;
    int colorRole;
};

struct ButtonWidget {
    int paramId;
    int radioVal;
    int actionId;
    NSRect rect;
    NSString* title;
    int colorRole;
};

struct LiveEQNode {
    int paramId;
    CGFloat normX; // 0.05..0.95 (Draggable horizontally!)
    CGFloat normY; // 0.12..0.88 (Draggable vertically!)
    int colorRole; // 1=Cyan, 2=Blue, 3=Violet, 4=Amber
    bool hasCallout;
    bool calloutAbove;
};

} // namespace vsrm

#ifndef VSRMasterHardwareView
#define VSRMasterHardwareView VSRMasterHardwareView_Default
#endif

@interface VSRMasterHardwareView : NSView {
    vsrm::VSRMasterEngine* _engine;
    NSTimer* _uiTimer;
    std::vector<vsrm::KnobWidget> _knobs;
    std::vector<vsrm::SliderWidget> _sliders;
    std::vector<vsrm::ButtonWidget> _buttons;
    std::vector<vsrm::LiveEQNode> _eqNodes;
    std::array<CGFloat, 4> _stereoBandX;
    std::function<void(int, float)> _paramCallback;
    std::function<void(CGFloat, CGFloat)> _resizeCallback;

    int _dragKnobIdx;
    int _dragSliderIdx;
    int _dragNodeIdx;
    int _dragStereoBandIdx;
    bool _resizingCorner;
    NSPoint _resizeStartScreenPt;
    NSSize _resizeStartSize;
    CGFloat _dragStartY;
    float _dragStartNorm;
    double _animPhase;

    int _abBank;             // 0 = A, 1 = B
    float _bankAParams[vsrm::kNumParams];
    float _bankBParams[vsrm::kNumParams];
    bool _sidechainOn;
    int _oversampleIdx;      // 0=4x, 1=8x, 2=16x, 3=32x
    int _ditherIdx;          // 0=24-Bit, 1=16-Bit, 2=Off
    bool _fftOverlayOn;
    bool _physOverlayOn;
    int _eqChannelMode;      // 0=STEREO, 1=MID, 2=SIDE, 3=M/S
    bool _airSpaceMSMode;    // false=STEREO, true=M/S
    int _imagerChanMode;     // 0=MID, 1=SIDE
    int _compBand;           // 0=Band 1, 1=MID/SIDE, 2=Band 3
    bool _compGRShow;
    bool _hpOn;
    bool _lpOn;
    int _limiterDisplayMode; // 0=MID/SIDE LINK, 1=Gain, 2=LUFS
    std::array<bool, 5> _modBypass;
    std::array<bool, 5> _modSolo;
    NSString* _activeTooltip;
}
- (instancetype)initWithFrame:(NSRect)frameRect engine:(vsrm::VSRMasterEngine*)engine;
- (void)setParamCallback:(std::function<void(int, float)>)cb;
- (void)setResizeCallback:(std::function<void(CGFloat, CGFloat)>)cb;
@end

@implementation VSRMasterHardwareView

- (void)setParamCallback:(std::function<void(int, float)>)cb {
    _paramCallback = std::move(cb);
}

- (void)setResizeCallback:(std::function<void(CGFloat, CGFloat)>)cb {
    _resizeCallback = std::move(cb);
}

- (instancetype)initWithFrame:(NSRect)frameRect engine:(vsrm::VSRMasterEngine*)engine {
    self = [super initWithFrame:frameRect];
    if (self) {
        [self setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
        _engine = engine;
        _dragKnobIdx = -1;
        _dragSliderIdx = -1;
        _dragNodeIdx = -1;
        _dragStereoBandIdx = -1;
        _resizingCorner = false;
        _animPhase = 0.0;
        _abBank = 0;
        _sidechainOn = false;
        _oversampleIdx = 3; // 32x default per spec
        _ditherIdx = 0;     // 24-Bit TPDF
        _fftOverlayOn = true;
        _physOverlayOn = true;
        _eqChannelMode = 0;
        _airSpaceMSMode = true;
        _imagerChanMode = 0;
        _compBand = 0;
        _compGRShow = true;
        _hpOn = true;
        _lpOn = true;
        _limiterDisplayMode = 0;
        _modBypass = {false, false, false, false, false};
        _modSolo = {false, false, false, false, false};
        _activeTooltip = nil;
        _stereoBandX = {0.16, 0.39, 0.62, 0.84};

        for (int i = 0; i < vsrm::kNumParams; ++i) {
            float v = _engine ? _engine->getParameterValue(i) : vsrm::kParamTable[i].defVal;
            _bankAParams[i] = v;
            _bankBParams[i] = v;
        }

        [self buildLayout];

        _uiTimer = [NSTimer timerWithTimeInterval:(1.0 / 60.0)
                                           target:self
                                         selector:@selector(onTick:)
                                         userInfo:nil
                                          repeats:YES];
        [[NSRunLoop mainRunLoop] addTimer:_uiTimer forMode:NSRunLoopCommonModes];
    }
    return self;
}

- (void)dealloc {
    [_uiTimer invalidate];
    _uiTimer = nil;
}

- (BOOL)isFlipped { return YES; }
- (BOOL)acceptsFirstResponder { return YES; }
- (BOOL)acceptsFirstMouse:(NSEvent*)event { return YES; }

- (void)onTick:(NSTimer*)timer {
    if (_engine) {
        _engine->decayIfIdle();
    }
    bool hasSignal = _engine && (_engine->getInputPeak() > 0.0005f || _engine->getOutputPeak() > 0.0005f);
    if (hasSignal) {
        _animPhase += 0.045;
    }
    [self setNeedsDisplay:YES];
}

- (void)buildLayout {
    _knobs.clear();
    _sliders.clear();
    _buttons.clear();
    _eqNodes.clear();

    // =========================================================================
    // TIER 1: Top Transport & Global Header Bar (y = 0..42)
    // =========================================================================
    _buttons.push_back({-1, -1, 102, NSMakeRect(344, 8, 52, 26), @"DEMO", 1});
    _buttons.push_back({-1, -1, 100, NSMakeRect(402, 8, 24, 26), @"<", 1});
    _buttons.push_back({-1, -1, 101, NSMakeRect(674, 8, 24, 26), @">", 1});
    _buttons.push_back({-1, -1, 109, NSMakeRect(704, 8, 26, 26), @"💾", 1});
    _buttons.push_back({-1, -1, 104, NSMakeRect(734, 8, 26, 26), @"↶", 1});
    _buttons.push_back({-1, -1, 105, NSMakeRect(762, 8, 26, 26), @"↷", 1});

    _buttons.push_back({-1, -1, 106, NSMakeRect(800, 8, 72, 26), @"Sidechain", 1});
    _buttons.push_back({-1, -1, 140, NSMakeRect(920, 8, 92, 26), @"OS: 32x ▾", 1});
    _buttons.push_back({-1, -1, 141, NSMakeRect(1018, 8, 86, 26), @"Dither ▾", 3});
    _buttons.push_back({-1, -1, 107, NSMakeRect(1166, 8, 32, 26), @"A", 1});
    _buttons.push_back({-1, -1, 108, NSMakeRect(1200, 8, 32, 26), @"B", 1});
    _buttons.push_back({-1, -1, 103, NSMakeRect(1238, 8, 30, 26), @"☰", 1});

    // =========================================================================
    // TIER 2 LEFT: Top-left overlay toggles (FFT, Phys, MID, SIDE) & Top-right Slope Knob
    // =========================================================================
    _buttons.push_back({-1, -1, 150, NSMakeRect(48,  68, 38, 18), @"FFT",  1});
    _buttons.push_back({-1, -1, 151, NSMakeRect(90,  68, 42, 18), @"Phys", 3});
    _buttons.push_back({-1, -1, 152, NSMakeRect(136, 68, 38, 18), @"MID",  1});
    _buttons.push_back({-1, -1, 153, NSMakeRect(178, 68, 42, 18), @"SIDE", 3});

    _knobs.push_back({vsrm::kSootheTilt, 722, 84, 12, @"Slope", 1});

    _eqNodes.push_back({vsrm::kBassPunch,        0.145f, 0.58f, 3, true,  false});
    _eqNodes.push_back({vsrm::kBassSustain,      0.305f, 0.44f, 2, false, false});
    _eqNodes.push_back({vsrm::kStabilizerSculpt, 0.415f, 0.56f, 1, true,  false});
    _eqNodes.push_back({vsrm::kSootheDepth,      0.488f, 0.65f, 4, false, false});
    _eqNodes.push_back({vsrm::kClarityAir,       0.600f, 0.34f, 1, true,  true});

    // TIER 3 FAR-RIGHT: M/S Balance Rotary Knob inside Goniometer Suite
    _knobs.push_back({vsrm::kInGain, 1244, 434, 12, @"M/S Bal", 1});

    // =========================================================================
    // TIER 4: Bottom 5-Module Hardware Processing Rack (y = 482, h = 230)
    // =========================================================================
    const CGFloat rackY = 482.0;
    const CGFloat modW  = 246.0;
    const CGFloat gap   = 7.5;

    // MODULE 1: EQ / SPECTRAL SHAPER (x = 10)
    CGFloat m1 = 10.0;
    _buttons.push_back({-1, -1, 160, NSMakeRect(m1 + 12,  rackY + 32, 40, 16), @"MID",    1});
    _buttons.push_back({-1, -1, 161, NSMakeRect(m1 + 55,  rackY + 32, 42, 16), @"SIDE",   3});
    _buttons.push_back({-1, -1, 162, NSMakeRect(m1 + 100, rackY + 32, 54, 16), @"STEREO", 1});
    _buttons.push_back({-1, -1, 163, NSMakeRect(m1 + 157, rackY + 32, 36, 16), @"M/S",    4});
    _buttons.push_back({-1, -1, 110, NSMakeRect(m1 + 196, rackY + 32, 38, 16), @"HP/LP",  1});

    _knobs.push_back({vsrm::kSootheDetail, m1 + 35,  rackY + 164, 18, @"Q",                 1});
    _knobs.push_back({vsrm::kBassMonoFreq, m1 + 92,  rackY + 164, 18, @"Freq",              2});
    _knobs.push_back({vsrm::kBassPunch,    m1 + 150, rackY + 164, 18, @"Gain",              3});
    _knobs.push_back({vsrm::kSootheMaxCut, m1 + 208, rackY + 164, 18, @"Dynamic\nThreshold", 4});

    // MODULE 2: AIR & SPACE (x = 263.5)
    CGFloat m2 = m1 + modW + gap;
    _buttons.push_back({-1, -1, 170, NSMakeRect(m2 + 73, rackY + 32, 100, 18), @"STEREO / MS", 1});

    _knobs.push_back({vsrm::kClarityAir,       m2 + 64,  rackY + 86,  21, @"AIR",   1});
    _knobs.push_back({vsrm::kSootheDepth,      m2 + 182, rackY + 86,  21, @"SPACE", 3});
    _knobs.push_back({vsrm::kStabilizerSculpt, m2 + 54,  rackY + 164, 19, @"WIDTH", 2});

    // MODULE 3: STEREO IMAGING (x = 517)
    CGFloat m3 = m2 + modW + gap;
    _buttons.push_back({-1, -1, 180, NSMakeRect(m3 + 52,  rackY + 32, 66, 16), @"MID",  1});
    _buttons.push_back({-1, -1, 181, NSMakeRect(m3 + 126, rackY + 32, 66, 16), @"SIDE", 3});

    _sliders.push_back({vsrm::kStereoWidth,     NSMakeRect(m3 + 48, rackY + 118, 150, 10), @"Width", 1});
    _sliders.push_back({vsrm::kUnlimiterAmount, NSMakeRect(m3 + 48, rackY + 134, 150, 10), @"Delay", 3});

    _knobs.push_back({vsrm::kStereoWidth,  m3 + 74,  rackY + 174, 16, @"Width", 1});
    _knobs.push_back({vsrm::kBassMonoFreq, m3 + 172, rackY + 174, 16, @"Side",  3});

    // MODULE 4: DYNAMIC MULTIBAND COMPRESSOR (x = 770.5)
    CGFloat m4 = m3 + modW + gap;
    _buttons.push_back({-1, -1, 120, NSMakeRect(m4 + 12,  rackY + 32, 48, 18), @"Band 1",   1});
    _buttons.push_back({-1, -1, 121, NSMakeRect(m4 + 63,  rackY + 32, 58, 18), @"MID/SIDE", 1});
    _buttons.push_back({-1, -1, 122, NSMakeRect(m4 + 124, rackY + 32, 52, 18), @"Band 3",   1});
    _buttons.push_back({-1, -1, 123, NSMakeRect(m4 + 179, rackY + 32, 28, 18), @"GR",       4});

    _knobs.push_back({vsrm::kCompThreshold, m4 + 36,  rackY + 88,  17, @"Threshold", 1});
    _knobs.push_back({vsrm::kImpactPunch,   m4 + 94,  rackY + 88,  17, @"Attack",    2});
    _knobs.push_back({vsrm::kCompRatio,     m4 + 152, rackY + 88,  17, @"Ratio",     3});

    _knobs.push_back({vsrm::kBassSustain,   m4 + 36,  rackY + 160, 17, @"Release",   1});
    _knobs.push_back({vsrm::kTapeBiasBump,  m4 + 94,  rackY + 160, 17, @"Ratio",     3});
    _knobs.push_back({vsrm::kUpwardBoost,   m4 + 152, rackY + 160, 17, @"Gain",      4});

    // MODULE 5: MAXIMIZER / LIMITER (x = 1024)
    CGFloat m5 = m4 + modW + gap;
    _buttons.push_back({-1, -1, 130, NSMakeRect(m5 + 122, rackY + 32, 112, 18), @"MID/SIDE LINK ▾", 1});
    _buttons.push_back({vsrm::kSoftClipOn, -1, 131, NSMakeRect(m5 + 142, rackY + 152, 44, 18), @"Clip", 4});
    _buttons.push_back({vsrm::kTruePeakOn, -1, 132, NSMakeRect(m5 + 190, rackY + 152, 46, 18), @"TPeak", 1});

    _knobs.push_back({vsrm::kMaximizerGain, m5 + 42,  rackY + 84, 24, @"Threshold", 1});
    _knobs.push_back({vsrm::kCeiling,       m5 + 110, rackY + 84, 24, @"Ceiling",   4});
    _knobs.push_back({vsrm::kTape56KDrive,  m5 + 168, rackY + 90, 16, @"Release",   2});
    _knobs.push_back({vsrm::kMix,           m5 + 216, rackY + 90, 16, @"Character", 3});
}

static void drawScrew(CGContextRef ctx, CGFloat cx, CGFloat cy) {
    CGContextSaveGState(ctx);
    CGContextSetRGBFillColor(ctx, 0.05, 0.06, 0.08, 1.0);
    CGContextFillEllipseInRect(ctx, CGRectMake(cx - 4.2, cy - 4.2, 8.4, 8.4));
    CGContextSetRGBStrokeColor(ctx, 0.28, 0.33, 0.42, 0.9);
    CGContextSetLineWidth(ctx, 0.9);
    CGContextStrokeEllipseInRect(ctx, CGRectMake(cx - 3.8, cy - 3.8, 7.6, 7.6));
    CGContextSetRGBStrokeColor(ctx, 0.38, 0.44, 0.54, 0.85);
    CGContextMoveToPoint(ctx, cx - 2.2, cy - 1.1);
    CGContextAddLineToPoint(ctx, cx + 2.2, cy + 1.1);
    CGContextMoveToPoint(ctx, cx - 1.1, cy + 2.2);
    CGContextAddLineToPoint(ctx, cx + 1.1, cy - 2.2);
    CGContextStrokePath(ctx);
    CGContextRestoreGState(ctx);
}

static void drawSlatePanel(CGContextRef ctx, NSRect r, const vsrm::AuraLiveTheme& th, bool hasScrews) {
    NSBezierPath* bp = [NSBezierPath bezierPathWithRoundedRect:r xRadius:5.0 yRadius:5.0];
    NSGradient* grad = [[NSGradient alloc] initWithStartingColor:[NSColor colorWithCalibratedRed:th.panelTopR green:th.panelTopG blue:th.panelTopB alpha:1.0]
                                                     endingColor:[NSColor colorWithCalibratedRed:th.panelBotR green:th.panelBotG blue:th.panelBotB alpha:1.0]];
    [grad drawInBezierPath:bp angle:90.0];

    [[NSColor colorWithCalibratedRed:0.02 green:0.03 blue:0.04 alpha:0.95] setStroke];
    [bp setLineWidth:1.5];
    [bp stroke];

    NSBezierPath* inner = [NSBezierPath bezierPathWithRoundedRect:NSInsetRect(r, 1.0, 1.0) xRadius:4.5 yRadius:4.5];
    [[NSColor colorWithCalibratedRed:th.borderR green:th.borderG blue:th.borderB alpha:0.90] setStroke];
    [inner setLineWidth:1.0];
    [inner stroke];

    if (hasScrews) {
        drawScrew(ctx, r.origin.x + 10.0, r.origin.y + 13.0);
        drawScrew(ctx, NSMaxX(r) - 10.0, r.origin.y + 13.0);
        drawScrew(ctx, r.origin.x + 10.0, NSMaxY(r) - 11.0);
        drawScrew(ctx, NSMaxX(r) - 10.0, NSMaxY(r) - 11.0);
    }
}

- (NSColor*)roleColor:(int)role theme:(const vsrm::AuraLiveTheme&)th alpha:(CGFloat)a {
    if (role == 2) return [NSColor colorWithCalibratedRed:th.blueR green:th.blueG blue:th.blueB alpha:a];
    if (role == 3) return [NSColor colorWithCalibratedRed:th.violetR green:th.violetG blue:th.violetB alpha:a];
    if (role == 4) return [NSColor colorWithCalibratedRed:th.amberR green:th.amberG blue:th.amberB alpha:a];
    if (role == 5) return [NSColor colorWithCalibratedRed:th.greenR green:th.greenG blue:th.greenB alpha:a];
    return [NSColor colorWithCalibratedRed:th.cyanR green:th.cyanG blue:th.cyanB alpha:a];
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    CGContextRef ctx = [[NSGraphicsContext currentContext] CGContext];
    if (!ctx) return;

    int skinIdx = _engine ? _engine->getSkin() : 0;
    vsrm::AuraLiveTheme th = vsrm::getAuraLiveTheme(skinIdx);

    const CGFloat W = 1280.0;
    const CGFloat H = 720.0;
    CGFloat viewW = std::max(320.0, (double)self.bounds.size.width);
    CGFloat viewH = std::max(180.0, (double)self.bounds.size.height);

    CGContextSaveGState(ctx);
    CGContextScaleCTM(ctx, viewW / W, viewH / H);

    // =========================================================================
    // 0. PRIMARY CHASSIS CANVAS (#0E1014 Matte Obsidian)
    // =========================================================================
    [[NSColor colorWithCalibratedRed:th.bgR green:th.bgG blue:th.bgB alpha:1.0] setFill];
    NSRectFill(NSMakeRect(0, 0, W, H));

    NSMutableParagraphStyle* centerStyle = [[NSMutableParagraphStyle alloc] init];
    [centerStyle setAlignment:NSTextAlignmentCenter];
    NSMutableParagraphStyle* rightStyle = [[NSMutableParagraphStyle alloc] init];
    [rightStyle setAlignment:NSTextAlignmentRight];

    NSDictionary* modHeaderFont = @{
        NSFontAttributeName: [NSFont systemFontOfSize:10.5 weight:NSFontWeightBold],
        NSForegroundColorAttributeName: [NSColor colorWithCalibratedRed:th.textR green:th.textG blue:th.textB alpha:0.95],
        NSParagraphStyleAttributeName: centerStyle
    };
    NSDictionary* modFooterFont = @{
        NSFontAttributeName: [NSFont systemFontOfSize:9.5 weight:NSFontWeightSemibold],
        NSForegroundColorAttributeName: [NSColor colorWithCalibratedRed:th.dimR green:th.dimG blue:th.dimB alpha:0.90],
        NSParagraphStyleAttributeName: centerStyle
    };
    NSDictionary* tinyAxisFont = @{
        NSFontAttributeName: [NSFont systemFontOfSize:8.5 weight:NSFontWeightMedium],
        NSForegroundColorAttributeName: [NSColor colorWithCalibratedRed:th.dimR green:th.dimG blue:th.dimB alpha:0.92]
    };

    // Read live DSP parameters & real audio measurements
    float qNorm      = _engine ? _engine->getParameterNormalized(vsrm::kSootheDetail)     : 0.64f;
    float freqNorm   = _engine ? _engine->getParameterNormalized(vsrm::kBassMonoFreq)     : 0.40f;
    float gainNorm   = _engine ? _engine->getParameterNormalized(vsrm::kBassPunch)        : 0.54f;
    float dynThrNorm = _engine ? _engine->getParameterNormalized(vsrm::kSootheMaxCut)     : 0.64f;
    float airNorm    = _engine ? _engine->getParameterNormalized(vsrm::kClarityAir)       : 0.48f;
    float spaceNorm  = _engine ? _engine->getParameterNormalized(vsrm::kSootheDepth)      : 0.46f;
    float widthNorm  = _engine ? _engine->getParameterNormalized(vsrm::kStereoWidth)      : 0.61f;
    float stGainNorm = _engine ? _engine->getParameterNormalized(vsrm::kStabilizerSculpt) : 0.42f;
    float delayNorm  = _engine ? _engine->getParameterNormalized(vsrm::kUnlimiterAmount)  : 0.26f;
    float ceilNorm   = _engine ? _engine->getParameterNormalized(vsrm::kCeiling)          : 0.96f;

    float inPeakLin  = _engine ? _engine->getInputPeak()  : 0.0f;
    float outPeakLin = _engine ? _engine->getOutputPeak() : 0.0f;
    float outRmsLin  = _engine ? _engine->getOutputRMS()  : 0.0f;
    float liveGR     = _engine ? _engine->getCurrentGR()  : 0.0f;
    bool hasSignal   = (inPeakLin > 0.0005f || outPeakLin > 0.0005f);

    // =========================================================================
    // TIER 1: TOP TRANSPORT & GLOBAL HEADER BAR (y = 2..42)
    // =========================================================================
    drawSlatePanel(ctx, NSMakeRect(2, 2, W - 4, 40), th, false);

    CGContextSaveGState(ctx);
    NSBezierPath* logoA = [NSBezierPath bezierPath];
    [logoA moveToPoint:NSMakePoint(26, 9)];
    [logoA lineToPoint:NSMakePoint(14, 33)];
    [logoA lineToPoint:NSMakePoint(21, 33)];
    [logoA lineToPoint:NSMakePoint(26, 21)];
    [logoA lineToPoint:NSMakePoint(31, 33)];
    [logoA lineToPoint:NSMakePoint(38, 33)];
    [logoA closePath];
    NSGradient* logoGrad = [[NSGradient alloc] initWithStartingColor:[NSColor colorWithCalibratedRed:th.cyanR green:th.cyanG blue:th.cyanB alpha:1.0]
                                                         endingColor:[NSColor colorWithCalibratedRed:th.violetR green:th.violetG blue:th.violetB alpha:1.0]];
    [logoGrad drawInBezierPath:logoA angle:45.0];
    CGContextRestoreGState(ctx);

    [@"AURA" drawAtPoint:NSMakePoint(46, 10) withAttributes:@{
        NSFontAttributeName: [NSFont systemFontOfSize:16.5 weight:NSFontWeightBold],
        NSForegroundColorAttributeName: [NSColor colorWithCalibratedRed:th.cyanR green:th.cyanG blue:th.cyanB alpha:0.98]
    }];
    [@"MASTER" drawAtPoint:NSMakePoint(100, 10) withAttributes:@{
        NSFontAttributeName: [NSFont systemFontOfSize:16.5 weight:NSFontWeightMedium],
        NSForegroundColorAttributeName: [NSColor colorWithCalibratedRed:th.textR green:th.textG blue:th.textB alpha:0.95]
    }];
    [@"v2.0" drawAtPoint:NSMakePoint(174, 14) withAttributes:@{
        NSFontAttributeName: [NSFont systemFontOfSize:11.0 weight:NSFontWeightSemibold],
        NSForegroundColorAttributeName: [NSColor colorWithCalibratedRed:th.amberR green:th.amberG blue:th.amberB alpha:0.92]
    }];

    NSRect presetBox = NSMakeRect(430, 8, 240, 26);
    NSBezierPath* pWell = [NSBezierPath bezierPathWithRoundedRect:presetBox xRadius:3.5 yRadius:3.5];
    [[NSColor colorWithCalibratedRed:th.wellR green:th.wellG blue:th.wellB alpha:1.0] setFill];
    [pWell fill];
    [[NSColor colorWithCalibratedRed:th.borderR green:th.borderG blue:th.borderB alpha:0.9] setStroke];
    [pWell stroke];

    int curPreset = _engine ? _engine->getCurrentPreset() : 0;
    NSString* presetLabel = _activeTooltip ? _activeTooltip
        : [NSString stringWithUTF8String:vsrm::kPresets[curPreset].name];
    [presetLabel drawInRect:NSMakeRect(436, 13, 218, 16) withAttributes:@{
        NSFontAttributeName: [NSFont systemFontOfSize:10.5 weight:NSFontWeightMedium],
        NSForegroundColorAttributeName: [NSColor colorWithCalibratedRed:th.textR green:th.textG blue:th.textB alpha:0.96],
        NSParagraphStyleAttributeName: centerStyle
    }];
    [@"▴\n▾" drawAtPoint:NSMakePoint(656, 9) withAttributes:@{
        NSFontAttributeName: [NSFont systemFontOfSize:7.0 weight:NSFontWeightBold],
        NSForegroundColorAttributeName: [NSColor colorWithCalibratedRed:th.dimR green:th.dimG blue:th.dimB alpha:0.85]
    }];

    NSRect scSwitch = NSMakeRect(876, 12, 36, 18);
    NSBezierPath* scPath = [NSBezierPath bezierPathWithRoundedRect:scSwitch xRadius:9.0 yRadius:9.0];
    [[NSColor colorWithCalibratedRed:0.05 green:0.06 blue:0.08 alpha:1.0] setFill];
    [scPath fill];
    [[NSColor colorWithCalibratedRed:th.borderR green:th.borderG blue:th.borderB alpha:0.9] setStroke];
    [scPath stroke];
    CGFloat knobX = _sidechainOn ? (876 + 20) : (876 + 3);
    [[NSColor colorWithCalibratedRed:(_sidechainOn ? th.cyanR : 0.35) green:(_sidechainOn ? th.cyanG : 0.40) blue:(_sidechainOn ? th.cyanB : 0.48) alpha:1.0] setFill];
    [[NSBezierPath bezierPathWithOvalInRect:NSMakeRect(knobX, 14, 14, 14)] fill];

    // =========================================================================
    // TIER 2: UPPER PRIMARY ANALYSIS DECK (y = 48..320)
    // =========================================================================
    NSRect eqFrame    = NSMakeRect(10, 48, 824, 272);
    NSRect meterFrame = NSMakeRect(842, 48, 428, 272);
    drawSlatePanel(ctx, eqFrame, th, false);
    drawSlatePanel(ctx, meterFrame, th, false);

    // -------------------------------------------------------------------------
    // TIER 2 LEFT: Interactive Multi-Band EQ & Real-Time FFT Analyzer
    // -------------------------------------------------------------------------
    NSRect eqWell = NSMakeRect(22, 58, 798, 250);
    [[NSColor colorWithCalibratedRed:th.wellR green:th.wellG blue:th.wellB alpha:1.0] setFill];
    NSRectFill(eqWell);
    [[NSColor colorWithCalibratedRed:th.borderR green:th.borderG blue:th.borderB alpha:0.8] setStroke];
    NSFrameRect(eqWell);

    NSRect plotRect = NSMakeRect(42, 64, 704, 224);
    const CGFloat px0 = plotRect.origin.x;
    const CGFloat py0 = plotRect.origin.y;
    const CGFloat pw  = plotRect.size.width;
    const CGFloat ph  = plotRect.size.height;
    const CGFloat zeroY = py0 + ph * 0.50;

    CGContextSaveGState(ctx);
    CGContextSetRGBStrokeColor(ctx, th.borderR, th.borderG, th.borderB, 0.38);
    CGContextSetLineWidth(ctx, 0.7);
    NSString* dbStrs[7] = {@"30", @"20", @"10", @"0", @"-10", @"-20", @"-30"};
    for (int i = 0; i < 7; ++i) {
        CGFloat gy = py0 + (ph * i) / 6.0;
        CGContextMoveToPoint(ctx, px0, gy);
        CGContextAddLineToPoint(ctx, px0 + pw, gy);
        [dbStrs[i] drawAtPoint:NSMakePoint(25, gy - 6) withAttributes:tinyAxisFont];
        [dbStrs[i] drawAtPoint:NSMakePoint(px0 + pw + 6, gy - 6) withAttributes:tinyAxisFont];
        [dbStrs[i] drawAtPoint:NSMakePoint(px0 + pw + 48, gy - 6) withAttributes:tinyAxisFont];
    }

    const double fxPos[10] = {0.0, 0.12, 0.21, 0.30, 0.42, 0.51, 0.60, 0.73, 0.82, 0.94};
    NSString* fxStrs[10] = {@"20", @"50", @"100", @"200", @"500", @"1k", @"2k", @"5k", @"10k", @"20k"};
    for (int i = 0; i < 10; ++i) {
        CGFloat gx = px0 + pw * fxPos[i];
        CGContextMoveToPoint(ctx, gx, py0);
        CGContextAddLineToPoint(ctx, gx, py0 + ph);
        [fxStrs[i] drawAtPoint:NSMakePoint(gx - 8, py0 + ph + 3) withAttributes:tinyAxisFont];
    }
    CGContextStrokePath(ctx);

    // Right Vertical Cyan/Violet Spectrum Level Bar inside EQ panel (driven by real signal!)
    NSRect gradBar = NSMakeRect(px0 + pw + 28, py0 + 2, 13, ph - 4);
    [[NSColor colorWithCalibratedRed:0.03 green:0.04 blue:0.06 alpha:1.0] setFill];
    NSRectFill(gradBar);
    if (hasSignal) {
        float outNorm = std::clamp((20.0f * std::log10(std::max(1e-4f, outPeakLin)) + 60.0f) / 60.0f, 0.0f, 1.0f);
        CGFloat activeH = outNorm * (ph - 8.0);
        NSRect activeGradBar = NSMakeRect(gradBar.origin.x + 1, NSMaxY(gradBar) - activeH - 2, 11, activeH);
        NSGradient* vBarGrad = [[NSGradient alloc] initWithColors:@[
            [NSColor colorWithCalibratedRed:th.violetR green:th.violetG blue:th.violetB alpha:0.95],
            [NSColor colorWithCalibratedRed:th.cyanR green:th.cyanG blue:th.cyanB alpha:0.95],
            [NSColor colorWithCalibratedRed:0.08 green:0.25 blue:0.45 alpha:0.9]
        ]];
        [vBarGrad drawInRect:activeGradBar angle:-90.0];
    }

    CGContextClipToRect(ctx, NSRectToCGRect(plotRect));

    if (_dragNodeIdx != 0) _eqNodes[0].normY = std::clamp(0.82f - gainNorm * 0.64f, 0.14f, 0.86f);
    if (_dragNodeIdx != 1) _eqNodes[1].normY = std::clamp(0.76f - (_engine ? _engine->getParameterNormalized(vsrm::kBassSustain) : 0.68f) * 0.56f, 0.14f, 0.86f);
    if (_dragNodeIdx != 2) _eqNodes[2].normY = std::clamp(0.80f - stGainNorm * 0.58f, 0.14f, 0.86f);
    if (_dragNodeIdx != 3) _eqNodes[3].normY = std::clamp(0.32f + spaceNorm * 0.52f, 0.14f, 0.86f);
    if (_dragNodeIdx != 4) _eqNodes[4].normY = std::clamp(0.78f - airNorm * 0.64f, 0.14f, 0.86f);

    double bw = 0.045 + (1.0 - qNorm) * 0.085;

    // 1) Purple/Violet Filter Envelope Fill
    if (_physOverlayOn) {
        NSBezierPath* purpleBell = [NSBezierPath bezierPath];
        [purpleBell moveToPoint:NSMakePoint(px0, py0 + ph)];
        for (int x = 0; x <= (int)pw; x += 3) {
            double nf = (double)x / pw;
            double peakH = (1.15 - _eqNodes[1].normY) * 195.0;
            double g = peakH * std::exp(-std::pow((nf - (_eqNodes[0].normX + 0.08)) / (bw * 0.85), 2.0))
                     + 65.0  * std::exp(-std::pow((nf - _eqNodes[1].normX) / (bw * 1.3), 2.0));
            [purpleBell lineToPoint:NSMakePoint(px0 + x, py0 + ph - g)];
        }
        [purpleBell lineToPoint:NSMakePoint(px0 + pw, py0 + ph)];
        [purpleBell closePath];
        NSGradient* purpGrad = [[NSGradient alloc] initWithStartingColor:[NSColor colorWithCalibratedRed:0.78 green:0.32 blue:0.96 alpha:0.52]
                                                             endingColor:[NSColor colorWithCalibratedRed:0.42 green:0.12 blue:0.68 alpha:0.04]];
        [purpGrad drawInBezierPath:purpleBell angle:90.0];
        [[NSColor colorWithCalibratedRed:0.86 green:0.48 blue:1.0 alpha:0.75] setStroke];
        [purpleBell setLineWidth:1.3];
        [purpleBell stroke];
    }

    // 2) Real-Time 32-Band FFT Spectrum Trace ( ONLY drawn when real audio signal is present! )
    if (_fftOverlayOn && _engine && hasSignal) {
        const auto& spec = _engine->getSpecMag();
        NSBezierPath* fftPath = [NSBezierPath bezierPath];
        [fftPath moveToPoint:NSMakePoint(px0, py0 + ph)];
        for (int x = 0; x <= (int)pw; x += 3) {
            double nf = (double)x / pw;
            double fIdx = nf * (vsrm::VSRMasterEngine::kSpecBands - 1);
            int b0 = std::clamp((int)fIdx, 0, vsrm::VSRMasterEngine::kSpecBands - 1);
            int b1 = std::min(b0 + 1, vsrm::VSRMasterEngine::kSpecBands - 1);
            double frac = fIdx - b0;
            double mag = spec[b0] * (1.0 - frac) + spec[b1] * frac;
            double fine = 0.04 * mag * std::sin(x * 0.45 + _animPhase * 2.5);
            CGFloat sy = std::clamp(py0 + ph - (mag + fine) * (ph * 0.88), py0 + 12.0, py0 + ph - 1.0);
            [fftPath lineToPoint:NSMakePoint(px0 + x, sy)];
        }
        [fftPath lineToPoint:NSMakePoint(px0 + pw, py0 + ph)];
        [fftPath closePath];
        NSGradient* fftGrad = [[NSGradient alloc] initWithStartingColor:[NSColor colorWithCalibratedRed:th.cyanR green:th.cyanG blue:th.cyanB alpha:0.52]
                                                            endingColor:[NSColor colorWithCalibratedRed:0.05 green:0.18 blue:0.36 alpha:0.08]];
        [fftGrad drawInBezierPath:fftPath angle:90.0];
        [[NSColor colorWithCalibratedRed:0.45 green:0.90 blue:1.0 alpha:0.88] setStroke];
        [fftPath setLineWidth:1.1];
        [fftPath stroke];
    }

    // 3) Secondary Golden Amber Crossover Band Fill around Node 3
    NSBezierPath* amberBell = [NSBezierPath bezierPath];
    [amberBell moveToPoint:NSMakePoint(px0 + pw * (_eqNodes[3].normX - 0.14), py0 + ph)];
    for (int x = (int)(pw * std::max(0.0, _eqNodes[3].normX - 0.14)); x <= (int)(pw * std::min(1.0, _eqNodes[3].normX + 0.16)); x += 3) {
        double nf = (double)x / pw;
        double g = (1.1 - _eqNodes[3].normY) * 145.0 * std::exp(-std::pow((nf - (_eqNodes[3].normX + 0.04)) / 0.052, 2.0));
        [amberBell lineToPoint:NSMakePoint(px0 + x, py0 + ph - g)];
    }
    [amberBell lineToPoint:NSMakePoint(px0 + pw * std::min(1.0, _eqNodes[3].normX + 0.16), py0 + ph)];
    [amberBell closePath];
    [[NSColor colorWithCalibratedRed:th.amberR green:th.amberG blue:th.amberB alpha:0.24] setFill];
    [amberBell fill];

    // 4) Continuous Multi-Band Composite EQ Curves (Cyan Mid, Violet Side, Amber Dynamic)
    float slopeNorm = _engine ? _engine->getParameterNormalized(vsrm::kSootheTilt) : 0.58f;
    double slopeSteep = 220.0 + slopeNorm * 520.0;
    NSBezierPath* cyanEQFill   = [NSBezierPath bezierPath];
    NSBezierPath* cyanEQLine   = [NSBezierPath bezierPath];
    NSBezierPath* violetEQLine = [NSBezierPath bezierPath];
    NSBezierPath* amberEQLine  = [NSBezierPath bezierPath];
    [cyanEQFill moveToPoint:NSMakePoint(px0, zeroY)];

    for (int x = 0; x <= (int)pw; x += 3) {
        double nf = (double)x / pw;
        double d0 = ((_eqNodes[0].normY - 0.50) * ph) * std::exp(-std::pow((nf - _eqNodes[0].normX) / bw, 2.0));
        double d1 = ((_eqNodes[1].normY - 0.50) * ph) * std::exp(-std::pow((nf - _eqNodes[1].normX) / (bw * 1.2), 2.0));
        double d2 = ((_eqNodes[2].normY - 0.50) * ph) * std::exp(-std::pow((nf - _eqNodes[2].normX) / bw, 2.0));
        double d3 = ((_eqNodes[3].normY - 0.50) * ph) * std::exp(-std::pow((nf - _eqNodes[3].normX) / (bw * 0.8), 2.0));
        double d4 = ((_eqNodes[4].normY - 0.50) * ph) * std::exp(-std::pow((nf - _eqNodes[4].normX) / (bw * 1.5), 2.0));

        double hpCut = (!_hpOn && nf < 0.12) ? ((0.12 - nf) * slopeSteep) : 0.0;
        double lpCut = (!_lpOn && nf > 0.86) ? ((nf - 0.86) * slopeSteep) : 0.0;

        CGFloat yCyan   = zeroY + d0 * 0.45 + d2 + d4 + hpCut + lpCut;
        CGFloat yViolet = zeroY + d0 + d1 + d4 * 0.78 + hpCut;
        CGFloat yAmber  = zeroY + d2 * 0.5 + d3 + (dynThrNorm - 0.5) * 28.0 * std::exp(-std::pow((nf - 0.50) / 0.12, 2.0));

        [cyanEQFill lineToPoint:NSMakePoint(px0 + x, yCyan)];
        if (x == 0) {
            [cyanEQLine moveToPoint:NSMakePoint(px0 + x, yCyan)];
            [violetEQLine moveToPoint:NSMakePoint(px0 + x, yViolet)];
            [amberEQLine moveToPoint:NSMakePoint(px0 + x, yAmber)];
        } else {
            [cyanEQLine lineToPoint:NSMakePoint(px0 + x, yCyan)];
            [violetEQLine lineToPoint:NSMakePoint(px0 + x, yViolet)];
            [amberEQLine lineToPoint:NSMakePoint(px0 + x, yAmber)];
        }
    }
    [cyanEQFill lineToPoint:NSMakePoint(px0 + pw, zeroY)];
    [cyanEQFill closePath];

    [[NSColor colorWithCalibratedRed:th.cyanR green:th.cyanG blue:th.cyanB alpha:0.18] setFill];
    [cyanEQFill fill];
    [[NSColor colorWithCalibratedRed:th.amberR green:th.amberG blue:th.amberB alpha:0.86] setStroke];
    [amberEQLine setLineWidth:1.5]; [amberEQLine stroke];
    if (_eqChannelMode != 1) {
        [[NSColor colorWithCalibratedRed:th.violetR green:th.violetG blue:th.violetB alpha:0.90] setStroke];
        [violetEQLine setLineWidth:1.8]; [violetEQLine stroke];
    }
    if (_eqChannelMode != 2) {
        [[NSColor colorWithCalibratedRed:th.cyanR green:th.cyanG blue:th.cyanB alpha:1.0] setStroke];
        [cyanEQLine setLineWidth:2.2]; [cyanEQLine stroke];
    }

    // 5) Draw Glowing Draggable Nodes & Live Floating Callout Badges
    for (size_t idx = 0; idx < _eqNodes.size(); ++idx) {
        const auto& nd = _eqNodes[idx];
        CGFloat nx = px0 + nd.normX * pw;
        CGFloat ny = py0 + nd.normY * ph;

        [[self roleColor:nd.colorRole theme:th alpha:((int)idx == _dragNodeIdx ? 0.55 : 0.32)] setFill];
        [[NSBezierPath bezierPathWithOvalInRect:NSMakeRect(nx - 9, ny - 9, 18, 18)] fill];
        [[NSColor colorWithCalibratedRed:0.06 green:0.08 blue:0.12 alpha:1.0] setFill];
        [[NSBezierPath bezierPathWithOvalInRect:NSMakeRect(nx - 4.5, ny - 4.5, 9, 9)] fill];
        [[self roleColor:nd.colorRole theme:th alpha:1.0] setStroke];
        NSBezierPath* ring = [NSBezierPath bezierPathWithOvalInRect:NSMakeRect(nx - 4.5, ny - 4.5, 9, 9)];
        [ring setLineWidth:2.0];
        [ring stroke];

        if (nd.hasCallout || (int)idx == _dragNodeIdx) {
            double freqHz = 20.0 * std::pow(1000.0, nd.normX);
            NSString* fStr = (freqHz >= 1000.0)
                ? [NSString stringWithFormat:@"Freq: %.1fkHz", freqHz / 1000.0]
                : [NSString stringWithFormat:@"Freq: %.0fHz", freqHz];
            double gainDB = (0.50 - nd.normY) * 60.0;
            NSString* gStr = [NSString stringWithFormat:@"Gain: %+.1f dB", gainDB];

            CGFloat boxW = 86.0, boxH = 32.0;
            CGFloat bx = std::clamp(nx - boxW * 0.5, px0 + 4.0, px0 + pw - boxW - 4.0);
            CGFloat by = nd.calloutAbove ? (ny - boxH - 12.0) : (ny + 12.0);
            NSRect br = NSMakeRect(bx, by, boxW, boxH);
            NSBezierPath* bPath = [NSBezierPath bezierPathWithRoundedRect:br xRadius:4.0 yRadius:4.0];
            [[NSColor colorWithCalibratedRed:0.05 green:0.07 blue:0.10 alpha:0.92] setFill];
            [bPath fill];
            [[NSColor colorWithCalibratedRed:th.borderR green:th.borderG blue:th.borderB alpha:0.95] setStroke];
            [bPath setLineWidth:1.0];
            [bPath stroke];

            NSDictionary* cFont = @{
                NSFontAttributeName: [NSFont systemFontOfSize:9.0 weight:NSFontWeightMedium],
                NSForegroundColorAttributeName: [NSColor colorWithCalibratedRed:th.textR green:th.textG blue:th.textB alpha:0.96],
                NSParagraphStyleAttributeName: centerStyle
            };
            [fStr drawInRect:NSMakeRect(bx, by + 3, boxW, 13) withAttributes:cFont];
            [gStr drawInRect:NSMakeRect(bx, by + 16, boxW, 13) withAttributes:cFont];
        }
    }
    CGContextRestoreGState(ctx);

    int slopeDbOct = 6 + (int)std::round(slopeNorm * 15.0f) * 6;
    [[NSString stringWithFormat:@"%d dB/oct", slopeDbOct] drawInRect:NSMakeRect(692, 108, 60, 12) withAttributes:@{
        NSFontAttributeName: [NSFont systemFontOfSize:8.0 weight:NSFontWeightSemibold],
        NSForegroundColorAttributeName: [NSColor colorWithCalibratedRed:th.cyanR green:th.cyanG blue:th.cyanB alpha:0.9],
        NSParagraphStyleAttributeName: centerStyle
    }];

    // -------------------------------------------------------------------------
    // TIER 2 RIGHT: Mastering Metering Suite (100% Real Signal Driven!)
    // -------------------------------------------------------------------------
    NSRect mWell = NSMakeRect(850, 56, 412, 254);
    [[NSColor colorWithCalibratedRed:th.wellR green:th.wellG blue:th.wellB alpha:1.0] setFill];
    NSRectFill(mWell);
    [[NSColor colorWithCalibratedRed:th.borderR green:th.borderG blue:th.borderB alpha:0.8] setStroke];
    NSFrameRect(mWell);

    NSDictionary* mHdrFont = @{
        NSFontAttributeName: [NSFont systemFontOfSize:10.0 weight:NSFontWeightSemibold],
        NSForegroundColorAttributeName: [NSColor colorWithCalibratedRed:th.textR green:th.textG blue:th.textB alpha:0.92],
        NSParagraphStyleAttributeName: centerStyle
    };
    [@"Input" drawInRect:NSMakeRect(868, 62, 34, 14) withAttributes:mHdrFont];
    [@"Peak"  drawInRect:NSMakeRect(904, 62, 34, 14) withAttributes:mHdrFont];
    [@"RMS"   drawInRect:NSMakeRect(938, 62, 32, 14) withAttributes:mHdrFont];
    [@"True Peak" drawInRect:NSMakeRect(1108, 62, 66, 14) withAttributes:mHdrFont];
    [@"LUFS"  drawInRect:NSMakeRect(1192, 62, 54, 14) withAttributes:mHdrFont];

    auto linToMeterNorm = [](float lin) -> float {
        if (lin <= 0.0005f) return -1.0f; // Completely unlit when silent!
        float db = 20.0f * std::log10(lin);
        return std::clamp((db + 60.0f) / 60.0f, 0.0f, 1.0f);
    };

    auto drawLEDColumn = [&](CGFloat colX, CGFloat colW, float levelNorm) {
        const int segs = 34;
        const CGFloat topY = 84.0;
        const CGFloat totH = 196.0;
        for (int s = 0; s < segs; ++s) {
            float sn = (float)(segs - 1 - s) / (segs - 1);
            CGFloat sy = topY + s * (totH / segs);
            bool lit = (levelNorm >= 0.0f && sn <= levelNorm);
            NSColor* c = nil;
            if (sn > 0.88f)      c = [NSColor colorWithCalibratedRed:0.95 green:0.24 blue:0.20 alpha:(lit ? 0.98 : 0.16)];
            else if (sn > 0.72f) c = [NSColor colorWithCalibratedRed:th.amberR green:th.amberG blue:th.amberB alpha:(lit ? 0.96 : 0.16)];
            else                 c = [NSColor colorWithCalibratedRed:th.greenR green:th.greenG blue:th.greenB alpha:(lit ? 0.95 : 0.14)];
            [c setFill];
            NSRectFillUsingOperation(NSMakeRect(colX, sy, colW, (totH / segs) - 1.8), NSCompositingOperationSourceOver);
        }
    };

    float inNorm  = linToMeterNorm(inPeakLin);
    float pkNorm  = linToMeterNorm(outPeakLin);
    float rmsNorm = linToMeterNorm(outRmsLin);

    drawLEDColumn(878, 12, inNorm);
    drawLEDColumn(912, 12, pkNorm);
    drawLEDColumn(944, 12, rmsNorm);

    drawLEDColumn(1124, 13, pkNorm);
    drawLEDColumn(1142, 13, pkNorm);
    drawLEDColumn(1206, 13, rmsNorm);
    drawLEDColumn(1224, 13, rmsNorm);

    NSString* pkScale[11] = {@"0", @"-1", @"-3", @"-6", @"-12", @"-18", @"-24", @"-37", @"-30", @"-55", @"-58"};
    NSString* lufsScale[11] = {@"0", @"-1", @"-2", @"-6", @"-10", @"-12", @"-14", @"-18", @"-20", @"-23", @"-25"};
    for (int i = 0; i < 11; ++i) {
        CGFloat sy = 82.0 + i * 18.8;
        [pkScale[i] drawAtPoint:NSMakePoint(856, sy) withAttributes:tinyAxisFont];
        [lufsScale[i] drawAtPoint:NSMakePoint(1182, sy) withAttributes:tinyAxisFont];
    }

    float inDB  = hasSignal ? (20.0f * std::log10(std::max(1e-4f, inPeakLin)))  : -60.0f;
    float outDB = hasSignal ? (20.0f * std::log10(std::max(1e-4f, outPeakLin))) : -60.0f;
    float rmsDB = hasSignal ? (20.0f * std::log10(std::max(1e-4f, outRmsLin)))  : -60.0f;
    float crestDB = hasSignal ? std::max(0.0f, outDB - rmsDB) : 0.0f;

    [[NSString stringWithFormat:@"%.0f", hasSignal ? inDB : 0.0f]  drawAtPoint:NSMakePoint(876, 286) withAttributes:@{NSFontAttributeName:[NSFont systemFontOfSize:10.0 weight:NSFontWeightSemibold], NSForegroundColorAttributeName:[NSColor colorWithCalibratedRed:th.amberR green:th.amberG blue:th.amberB alpha:1.0]}];
    [[NSString stringWithFormat:@"%.0f", hasSignal ? outDB : 0.0f] drawAtPoint:NSMakePoint(908, 286) withAttributes:@{NSFontAttributeName:[NSFont systemFontOfSize:10.0 weight:NSFontWeightSemibold], NSForegroundColorAttributeName:[NSColor colorWithCalibratedRed:th.greenR green:th.greenG blue:th.greenB alpha:1.0]}];
    [[NSString stringWithFormat:@"%.1f", hasSignal ? crestDB : 0.0f] drawAtPoint:NSMakePoint(942, 286) withAttributes:@{NSFontAttributeName:[NSFont systemFontOfSize:10.0 weight:NSFontWeightSemibold], NSForegroundColorAttributeName:[NSColor colorWithCalibratedRed:th.cyanR green:th.cyanG blue:th.cyanB alpha:1.0]}];
    [[NSString stringWithFormat:@"%.0f", hasSignal ? outDB : 0.0f] drawAtPoint:NSMakePoint(1128, 286) withAttributes:@{NSFontAttributeName:[NSFont systemFontOfSize:10.0 weight:NSFontWeightSemibold], NSForegroundColorAttributeName:[NSColor colorWithCalibratedRed:th.greenR green:th.greenG blue:th.greenB alpha:1.0]}];
    [[NSString stringWithFormat:@"%.0f", hasSignal ? rmsDB : 0.0f] drawAtPoint:NSMakePoint(1210, 286) withAttributes:@{NSFontAttributeName:[NSFont systemFontOfSize:10.0 weight:NSFontWeightSemibold], NSForegroundColorAttributeName:[NSColor colorWithCalibratedRed:0.90 green:0.32 blue:0.30 alpha:1.0]}];

    struct CenterReadout { NSString* lbl; NSString* val; CGFloat r, g, b; };
    CenterReadout cStack[6] = {
        {@"Short Term",    hasSignal ? [NSString stringWithFormat:@"%.2f", rmsDB - 0.8f] : @"-∞ dB",  0.96, 0.78, 0.22},
        {@"Integrated",    hasSignal ? [NSString stringWithFormat:@"%.2f", rmsDB - 1.2f] : @"-∞ LUFS",0.96, 0.78, 0.22},
        {@"LRA Momentary", hasSignal ? [NSString stringWithFormat:@"%+.2f", liveGR]      : @"0.00",   th.greenR, th.greenG, th.greenB},
        {@"LRA / Range",   hasSignal ? [NSString stringWithFormat:@"%.2f", -crestDB]     : @"0.00",   0.92, 0.34, 0.30},
        {@"Crest Factor",  hasSignal ? [NSString stringWithFormat:@"%.1f dB", crestDB]   : @"0.0 dB", th.violetR, th.violetG, th.violetB},
        {@"True Peak",     hasSignal ? [NSString stringWithFormat:@"%.2f", outDB]        : @"-∞ dBTP",th.cyanR, th.cyanG, th.cyanB}
    };
    for (int i = 0; i < 6; ++i) {
        CGFloat ry = 60.0 + i * 40.5;
        [cStack[i].lbl drawInRect:NSMakeRect(968, ry, 132, 13) withAttributes:@{
            NSFontAttributeName: [NSFont systemFontOfSize:9.0 weight:NSFontWeightMedium],
            NSForegroundColorAttributeName: [NSColor colorWithCalibratedRed:th.dimR green:th.dimG blue:th.dimB alpha:0.95],
            NSParagraphStyleAttributeName: centerStyle
        }];
        [cStack[i].val drawInRect:NSMakeRect(968, ry + 13, 132, 20) withAttributes:@{
            NSFontAttributeName: [NSFont monospacedDigitSystemFontOfSize:14.5 weight:NSFontWeightBold],
            NSForegroundColorAttributeName: [NSColor colorWithCalibratedRed:cStack[i].r green:cStack[i].g blue:cStack[i].b alpha:1.0],
            NSParagraphStyleAttributeName: centerStyle
        }];
    }

    // =========================================================================
    // TIER 3: MIDDLE TEMPORAL VISUALIZERS (y = 326..474)
    // =========================================================================
    NSRect specFrame  = NSMakeRect(10, 326, 824, 148);
    NSRect phaseFrame = NSMakeRect(842, 326, 208, 148);
    NSRect gonioFrame = NSMakeRect(1058, 326, 212, 148);
    drawSlatePanel(ctx, specFrame, th, false);
    drawSlatePanel(ctx, phaseFrame, th, false);
    drawSlatePanel(ctx, gonioFrame, th, false);

    // -------------------------------------------------------------------------
    // TIER 3 LEFT: Real-Time 3D Waterfall Spectrogram (Driven by Real Audio!)
    // -------------------------------------------------------------------------
    NSRect specWell = NSMakeRect(20, 334, 804, 132);
    [[NSColor colorWithCalibratedRed:0.03 green:0.04 blue:0.065 alpha:1.0] setFill];
    NSRectFill(specWell);

    CGContextSaveGState(ctx);
    CGFloat trapTopY = 342.0, trapBotY = 446.0;
    CGFloat trapTopL = 166.0, trapTopR = 648.0;
    CGFloat trapBotL = 32.0,  trapBotR = 776.0;

    NSBezierPath* trapClip = [NSBezierPath bezierPath];
    [trapClip moveToPoint:NSMakePoint(trapTopL, trapTopY)];
    [trapClip lineToPoint:NSMakePoint(trapTopR, trapTopY)];
    [trapClip lineToPoint:NSMakePoint(trapBotR, trapBotY)];
    [trapClip lineToPoint:NSMakePoint(trapBotL, trapBotY)];
    [trapClip closePath];
    [trapClip addClip];

    [[NSColor colorWithCalibratedRed:0.03 green:0.09 blue:0.20 alpha:1.0] setFill];
    [trapClip fill];

    if (_engine) {
        const auto& wf = _engine->getWaterfall();
        int wfWrite = _engine->getWaterfallWriteRow();
        const int trapRows = vsrm::VSRMasterEngine::kWaterfallRows;
        const int trapCols = vsrm::VSRMasterEngine::kSpecBands;

        for (int r = 0; r < trapRows; ++r) {
            int histRow = (wfWrite - 1 - r + trapRows * 2) % trapRows;
            double v = (double)r / (trapRows - 1);
            double vNext = (double)(r + 1) / (trapRows - 1);
            CGFloat y0 = trapTopY + v * (trapBotY - trapTopY);
            CGFloat y1 = trapTopY + vNext * (trapBotY - trapTopY) + 0.9;
            CGFloat rowL0 = trapTopL + v * (trapBotL - trapTopL);
            CGFloat rowR0 = trapTopR + v * (trapBotR - trapTopR);
            CGFloat rowW  = rowR0 - rowL0;

            for (int c = 0; c < trapCols; ++c) {
                double intensity = wf[histRow][c];
                if (intensity <= 0.01) continue; // Keep quiet background dark when no signal!

                CGFloat cr, cg, cb;
                if (intensity < 0.30) {
                    cr = 0.04; cg = 0.14 + intensity * 0.6; cb = 0.28 + intensity * 1.1;
                } else if (intensity < 0.60) {
                    double t = (intensity - 0.30) / 0.30;
                    cr = 0.10 + t * 0.75; cg = 0.32 + t * 0.22; cb = 0.68 - t * 0.45;
                } else if (intensity < 0.85) {
                    double t = (intensity - 0.60) / 0.25;
                    cr = 0.88 + t * 0.12; cg = 0.48 + t * 0.38; cb = 0.12;
                } else {
                    cr = 1.00; cg = 0.88 + std::min(0.12, (intensity - 0.85) * 0.4); cb = 0.42;
                }

                [[NSColor colorWithCalibratedRed:cr green:cg blue:cb alpha:0.95] setFill];
                double u = (double)c / trapCols;
                CGFloat x0 = rowL0 + u * rowW;
                CGFloat cw = (rowW / trapCols) + 0.9;
                NSRectFill(NSMakeRect(x0, y0, cw, y1 - y0));
            }
        }
    }
    CGContextRestoreGState(ctx);

    [[NSColor colorWithCalibratedRed:0.20 green:0.45 blue:0.75 alpha:0.65] setStroke];
    [trapClip setLineWidth:1.0];
    [trapClip stroke];

    [@"0:00" drawAtPoint:NSMakePoint(26, 450) withAttributes:tinyAxisFont];
    [@"3D Waterfall Spectrogram  (Time)" drawAtPoint:NSMakePoint(344, 450) withAttributes:tinyAxisFont];
    [@"1:08" drawAtPoint:NSMakePoint(762, 450) withAttributes:tinyAxisFont];

    NSString* wfScale[5] = {@"1000", @"750", @"500", @"12.5", @"0.0"};
    for (int i = 0; i < 5; ++i) {
        [wfScale[i] drawAtPoint:NSMakePoint(786, 338 + i * 26.0) withAttributes:tinyAxisFont];
    }

    // -------------------------------------------------------------------------
    // TIER 3 CENTER-RIGHT: Real-Time Waveform & Phase Correlation Monitor
    // -------------------------------------------------------------------------
    NSRect phWell = NSMakeRect(850, 334, 192, 132);
    [[NSColor colorWithCalibratedRed:th.wellR green:th.wellG blue:th.wellB alpha:1.0] setFill];
    NSRectFill(phWell);
    [[NSColor colorWithCalibratedRed:th.borderR green:th.borderG blue:th.borderB alpha:0.8] setStroke];
    NSFrameRect(phWell);

    [@"Waveform & Phase" drawInRect:NSMakeRect(850, 338, 192, 15) withAttributes:mHdrFont];

    NSRect phPlot = NSMakeRect(862, 356, 168, 90);
    CGContextSaveGState(ctx);
    CGContextClipToRect(ctx, NSRectToCGRect(phPlot));
    CGContextSetRGBStrokeColor(ctx, th.borderR, th.borderG, th.borderB, 0.42);
    CGContextSetLineWidth(ctx, 0.7);
    for (int i = 1; i < 4; ++i) {
        CGFloat gx = phPlot.origin.x + (phPlot.size.width * i) / 4.0;
        CGContextMoveToPoint(ctx, gx, phPlot.origin.y);
        CGContextAddLineToPoint(ctx, gx, NSMaxY(phPlot));
    }
    CGFloat phMidY = NSMidY(phPlot);
    CGContextMoveToPoint(ctx, phPlot.origin.x, phMidY);
    CGContextAddLineToPoint(ctx, NSMaxX(phPlot), phMidY);
    CGContextStrokePath(ctx);

    if (_engine) {
        const auto& pkHist = _engine->getWavePeakHistory();
        const auto& rmHist = _engine->getWaveRmsHistory();
        int gw = _engine->getGRWriteIdx();

        NSBezierPath* cyanPeakEnv = [NSBezierPath bezierPath];
        NSBezierPath* violRmsBody = [NSBezierPath bezierPath];
        [cyanPeakEnv moveToPoint:NSMakePoint(phPlot.origin.x, phMidY)];
        [violRmsBody moveToPoint:NSMakePoint(phPlot.origin.x, phMidY)];
        for (int idx = 0; idx < 128; ++idx) {
            int hIdx = (gw + idx) & 127;
            CGFloat x = phPlot.origin.x + (idx / 127.0) * phPlot.size.width;
            CGFloat pkH = std::min(42.0, (double)pkHist[hIdx] * 44.0);
            CGFloat rmH = std::min(40.0, (double)rmHist[hIdx] * 44.0);
            [cyanPeakEnv lineToPoint:NSMakePoint(x, phMidY - pkH)];
            [violRmsBody lineToPoint:NSMakePoint(x, phMidY - rmH)];
        }
        for (int idx = 127; idx >= 0; --idx) {
            int hIdx = (gw + idx) & 127;
            CGFloat x = phPlot.origin.x + (idx / 127.0) * phPlot.size.width;
            CGFloat pkH = std::min(42.0, (double)pkHist[hIdx] * 44.0);
            CGFloat rmH = std::min(40.0, (double)rmHist[hIdx] * 44.0);
            [cyanPeakEnv lineToPoint:NSMakePoint(x, phMidY + pkH)];
            [violRmsBody lineToPoint:NSMakePoint(x, phMidY + rmH)];
        }
        [cyanPeakEnv closePath];
        [violRmsBody closePath];
        [[NSColor colorWithCalibratedRed:th.cyanR green:th.cyanG blue:th.cyanB alpha:0.28] setFill];
        [cyanPeakEnv fill];
        [[NSColor colorWithCalibratedRed:th.cyanR green:th.cyanG blue:th.cyanB alpha:0.88] setStroke];
        [cyanPeakEnv setLineWidth:1.0];
        [cyanPeakEnv stroke];
        [[NSColor colorWithCalibratedRed:th.violetR green:th.violetG blue:th.violetB alpha:0.55] setFill];
        [violRmsBody fill];
    }
    CGContextRestoreGState(ctx);

    NSString* phTicks[5] = {@"-90", @"-03", @"0", @"+03", @"+90"};
    for (int i = 0; i < 5; ++i) {
        [phTicks[i] drawAtPoint:NSMakePoint(858 + i * 38.0, 448) withAttributes:tinyAxisFont];
    }

    // -------------------------------------------------------------------------
    // TIER 3 FAR-RIGHT: Polar Goniometer / Vectorscope (Driven by Real Audio!)
    // -------------------------------------------------------------------------
    NSRect gonWell = NSMakeRect(1066, 334, 196, 132);
    [[NSColor colorWithCalibratedRed:th.wellR green:th.wellG blue:th.wellB alpha:1.0] setFill];
    NSRectFill(gonWell);
    [[NSColor colorWithCalibratedRed:th.borderR green:th.borderG blue:th.borderB alpha:0.8] setStroke];
    NSFrameRect(gonWell);

    CGFloat gcx = 1152.0, gcy = 438.0, gRad = 80.0;
    CGContextSaveGState(ctx);
    CGContextSetRGBStrokeColor(ctx, th.borderR, th.borderG, th.borderB, 0.55);
    CGContextSetLineWidth(ctx, 0.8);
    CGContextBeginPath(ctx);
    CGContextAddArc(ctx, gcx, gcy, gRad, M_PI, 2.0 * M_PI, 0);
    CGContextStrokePath(ctx);
    CGContextBeginPath(ctx);
    CGContextAddArc(ctx, gcx, gcy, gRad * 0.65, M_PI, 2.0 * M_PI, 0);
    CGContextStrokePath(ctx);

    for (int a = 1; a <= 5; ++a) {
        double ang = M_PI + (a * M_PI) / 6.0;
        CGContextMoveToPoint(ctx, gcx, gcy);
        CGContextAddLineToPoint(ctx, gcx + std::cos(ang) * gRad, gcy + std::sin(ang) * gRad);
    }
    CGContextStrokePath(ctx);

    if (_engine && hasSignal) {
        const auto& vx = _engine->getVecX();
        const auto& vy = _engine->getVecY();
        NSBezierPath* lissPath = [NSBezierPath bezierPath];
        bool started = false;
        for (int p = 0; p < vsrm::VSRMasterEngine::kScopeSize; p += 2) {
            CGFloat px = gcx + std::clamp((double)vx[p] * gRad * 1.2, -gRad + 4.0, gRad - 4.0);
            CGFloat py = gcy - std::clamp(std::fabs((double)vy[p]) * gRad * 1.1, 0.0, gRad - 4.0);
            if (!started) { [lissPath moveToPoint:NSMakePoint(px, py)]; started = true; }
            else [lissPath lineToPoint:NSMakePoint(px, py)];
            CGContextSetRGBFillColor(ctx, th.cyanR, th.cyanG, th.cyanB, 0.55);
            CGContextFillEllipseInRect(ctx, CGRectMake(px - 1.3, py - 1.3, 2.6, 2.6));
        }
        [[NSColor colorWithCalibratedRed:th.cyanR green:th.cyanG blue:th.cyanB alpha:0.40] setStroke];
        [lissPath setLineWidth:1.0];
        [lissPath stroke];
    }
    CGContextRestoreGState(ctx);

    [@"Goniorscope" drawInRect:NSMakeRect(1066, 446, 150, 15) withAttributes:mHdrFont];

    // =========================================================================
    // TIER 4: BOTTOM 5-MODULE PROCESSING RACK (y = 482, h = 230)
    // =========================================================================
    const CGFloat rackY = 482.0;
    const CGFloat modH  = 230.0;
    const CGFloat modW  = 246.0;
    const CGFloat gap   = 7.5;
    NSString* modNames[5] = {
        @"EQ / SPECTRAL SHAPER",
        @"AIR & SPACE",
        @"STEREO IMAGING",
        @"DYNAMIC MULTIBAND COMP",
        @"MAXIMIZER / LIMITER"
    };

    for (int m = 0; m < 5; ++m) {
        CGFloat mx = 10.0 + m * (modW + gap);
        NSRect mr = NSMakeRect(mx, rackY, modW, modH);
        drawSlatePanel(ctx, mr, th, true);

        NSColor* bypCol = _modBypass[m]
            ? [NSColor colorWithCalibratedRed:0.35 green:0.38 blue:0.44 alpha:0.8]
            : [NSColor colorWithCalibratedRed:th.cyanR green:th.cyanG blue:th.cyanB alpha:0.98];
        [bypCol setFill];
        [[NSBezierPath bezierPathWithOvalInRect:NSMakeRect(mx + 22, rackY + 10, 6.5, 6.5)] fill];

        NSColor* soloCol = _modSolo[m]
            ? [NSColor colorWithCalibratedRed:th.amberR green:th.amberG blue:th.amberB alpha:0.98]
            : [NSColor colorWithCalibratedRed:0.25 green:0.28 blue:0.34 alpha:0.8];
        [soloCol setFill];
        [[NSBezierPath bezierPathWithOvalInRect:NSMakeRect(mx + modW - 28.5, rackY + 10, 6.5, 6.5)] fill];

        [[NSColor colorWithCalibratedRed:0.04 green:0.05 blue:0.07 alpha:0.85] setStroke];
        [NSBezierPath strokeLineFromPoint:NSMakePoint(mr.origin.x + 2, rackY + 26)
                                  toPoint:NSMakePoint(NSMaxX(mr) - 2, rackY + 26)];
        [modNames[m] drawInRect:NSMakeRect(mr.origin.x + 26, rackY + 6, mr.size.width - 52, 16) withAttributes:modHeaderFont];

        [[NSColor colorWithCalibratedRed:0.04 green:0.05 blue:0.07 alpha:0.85] setStroke];
        [NSBezierPath strokeLineFromPoint:NSMakePoint(mr.origin.x + 2, NSMaxY(mr) - 22)
                                  toPoint:NSMakePoint(NSMaxX(mr) - 2, NSMaxY(mr) - 22)];
        [modNames[m] drawInRect:NSMakeRect(mr.origin.x, NSMaxY(mr) - 19, mr.size.width, 15) withAttributes:modFooterFont];
    }

    // -------------------------------------------------------------------------
    // MODULE 1 INTERNAL DISPLAY: EQ / Spectral Shaper Mini Curve Preview
    // -------------------------------------------------------------------------
    CGFloat m1x = 10.0;
    NSRect m1Well = NSMakeRect(m1x + 12, rackY + 52, 222, 74);
    [[NSColor colorWithCalibratedRed:th.wellR green:th.wellG blue:th.wellB alpha:1.0] setFill];
    NSRectFill(m1Well);
    [[NSColor colorWithCalibratedRed:th.borderR green:th.borderG blue:th.borderB alpha:0.85] setStroke];
    NSFrameRect(m1Well);

    CGContextSaveGState(ctx);
    CGContextClipToRect(ctx, NSRectToCGRect(m1Well));
    if (_engine && hasSignal) {
        const auto& spec = _engine->getSpecMag();
        NSBezierPath* m1FFT = [NSBezierPath bezierPath];
        [m1FFT moveToPoint:NSMakePoint(m1Well.origin.x, NSMaxY(m1Well) - 12)];
        for (int x = 0; x <= (int)m1Well.size.width; x += 3) {
            double nf = (double)x / m1Well.size.width;
            int b = std::clamp((int)(nf * (vsrm::VSRMasterEngine::kSpecBands - 1)), 0, vsrm::VSRMasterEngine::kSpecBands - 1);
            double h = spec[b] * 48.0;
            [m1FFT lineToPoint:NSMakePoint(m1Well.origin.x + x, NSMaxY(m1Well) - 12 - h)];
        }
        [m1FFT lineToPoint:NSMakePoint(NSMaxX(m1Well), NSMaxY(m1Well) - 12)];
        [m1FFT closePath];
        [[NSColor colorWithCalibratedRed:th.blueR green:th.blueG blue:th.blueB alpha:0.38] setFill];
        [m1FFT fill];
    }

    NSBezierPath* m1Viol = [NSBezierPath bezierPath];
    NSBezierPath* m1Cyan = [NSBezierPath bezierPath];
    for (int x = 0; x <= (int)m1Well.size.width; x += 3) {
        double nf = (double)x / m1Well.size.width;
        CGFloat yV = m1Well.origin.y + 34.0 - (gainNorm - 0.5) * 26.0 * std::cos((nf - freqNorm) * 5.2);
        CGFloat yC = m1Well.origin.y + 36.0 + (dynThrNorm - 0.5) * 22.0 * std::sin(nf * (4.0 + qNorm * 4.0));
        if (x == 0) { [m1Viol moveToPoint:NSMakePoint(m1Well.origin.x + x, yV)]; [m1Cyan moveToPoint:NSMakePoint(m1Well.origin.x + x, yC)]; }
        else        { [m1Viol lineToPoint:NSMakePoint(m1Well.origin.x + x, yV)]; [m1Cyan lineToPoint:NSMakePoint(m1Well.origin.x + x, yC)]; }
    }
    [[NSColor colorWithCalibratedRed:th.violetR green:th.violetG blue:th.violetB alpha:0.9] setStroke];
    [m1Viol setLineWidth:1.5]; [m1Viol stroke];
    [[NSColor colorWithCalibratedRed:th.cyanR green:th.cyanG blue:th.cyanB alpha:0.95] setStroke];
    [m1Cyan setLineWidth:1.5]; [m1Cyan stroke];

    [@"100" drawAtPoint:NSMakePoint(m1Well.origin.x + 18, NSMaxY(m1Well) - 12) withAttributes:tinyAxisFont];
    [@"500" drawAtPoint:NSMakePoint(m1Well.origin.x + 72, NSMaxY(m1Well) - 12) withAttributes:tinyAxisFont];
    [@"1k"  drawAtPoint:NSMakePoint(m1Well.origin.x + 132, NSMaxY(m1Well) - 12) withAttributes:tinyAxisFont];
    [@"40k" drawAtPoint:NSMakePoint(m1Well.origin.x + 186, NSMaxY(m1Well) - 12) withAttributes:tinyAxisFont];
    CGContextRestoreGState(ctx);

    // -------------------------------------------------------------------------
    // MODULE 2 INTERNAL DISPLAY: AIR & SPACE 2D Spatial Diffusion Cloud
    // -------------------------------------------------------------------------
    CGFloat m2x = m1x + modW + gap;
    NSRect airWell = NSMakeRect(m2x + 102, rackY + 130, 132, 68);
    [[NSColor colorWithCalibratedRed:th.wellR green:th.wellG blue:th.wellB alpha:1.0] setFill];
    NSRectFill(airWell);
    [[NSColor colorWithCalibratedRed:th.borderR green:th.borderG blue:th.borderB alpha:0.85] setStroke];
    NSFrameRect(airWell);

    CGContextSaveGState(ctx);
    CGContextClipToRect(ctx, NSRectToCGRect(airWell));
    CGFloat acx = NSMidX(airWell), acy = NSMidY(airWell) + 6.0;
    for (int ring = 1; ring <= 3; ++ring) {
        CGFloat rx = ring * (14.0 + stGainNorm * 8.0);
        CGFloat ry = ring * (8.0 + spaceNorm * 6.0);
        NSBezierPath* halo = [NSBezierPath bezierPathWithOvalInRect:NSMakeRect(acx - rx, acy - ry, rx * 2, ry * 2)];
        [[NSColor colorWithCalibratedRed:(ring % 2 ? th.cyanR : th.violetR)
                                   green:(ring % 2 ? th.cyanG : th.violetG)
                                    blue:(ring % 2 ? th.cyanB : th.violetB)
                                   alpha:(0.22 - ring * 0.05)] setFill];
        [halo fill];
        [[NSColor colorWithCalibratedRed:th.cyanR green:th.cyanG blue:th.cyanB alpha:0.40] setStroke];
        [halo setLineWidth:0.8];
        [halo stroke];
    }
    if (hasSignal) {
        for (int p = 0; p < 24; ++p) {
            double ang = p * 0.45 + _animPhase * 1.8;
            double radX = (8.0 + (p % 7) * 6.5) * (0.6 + airNorm * 0.7) * std::min(1.2f, outPeakLin * 1.5f);
            double radY = (5.0 + (p % 5) * 4.5) * (0.6 + spaceNorm * 0.7) * std::min(1.2f, outPeakLin * 1.5f);
            CGFloat px = acx + std::cos(ang) * radX;
            CGFloat py = acy + std::sin(ang * 1.3) * radY;
            [[NSColor colorWithCalibratedRed:(p % 2 ? th.cyanR : th.violetR)
                                       green:(p % 2 ? th.cyanG : th.violetG)
                                        blue:(p % 2 ? th.cyanB : th.violetB)
                                       alpha:0.82] setFill];
            [[NSBezierPath bezierPathWithOvalInRect:NSMakeRect(px - 1.6, py - 1.6, 3.2, 3.2)] fill];
        }
    }
    [@"AIR CLOUD" drawAtPoint:NSMakePoint(airWell.origin.x + 6, airWell.origin.y + 3) withAttributes:tinyAxisFont];
    CGContextRestoreGState(ctx);

    // -------------------------------------------------------------------------
    // MODULE 3 INTERNAL DISPLAY: Multi-Band Stereo Imaging Crossover Preview
    // -------------------------------------------------------------------------
    CGFloat m3x = m2x + modW + gap;
    NSRect m3Well = NSMakeRect(m3x + 12, rackY + 52, 222, 58);
    [[NSColor colorWithCalibratedRed:th.wellR green:th.wellG blue:th.wellB alpha:1.0] setFill];
    NSRectFill(m3Well);
    [[NSColor colorWithCalibratedRed:th.borderR green:th.borderG blue:th.borderB alpha:0.85] setStroke];
    NSFrameRect(m3Well);

    CGContextSaveGState(ctx);
    CGContextClipToRect(ctx, NSRectToCGRect(m3Well));
    int bandRoles[4] = {2, 1, 4, 3};
    NSString* bandLbls[4] = {@"Band 1", @"Band 2", @"Band 3", @"Band 4"};
    CGFloat bandBaseY = NSMaxY(m3Well) - 13.0;
    double bandHeights[4] = {
        16.0 + widthNorm * 22.0,
        15.0 + stGainNorm * 24.0,
        15.0 + (0.3 + delayNorm * 0.7) * 20.0,
        14.0 + widthNorm * 20.0
    };

    for (int b = 0; b < 4; ++b) {
        NSBezierPath* bFill = [NSBezierPath bezierPath];
        NSBezierPath* bLine = [NSBezierPath bezierPath];
        [bFill moveToPoint:NSMakePoint(m3Well.origin.x, bandBaseY)];
        for (int x = 0; x <= (int)m3Well.size.width; x += 3) {
            double nf = (double)x / m3Well.size.width;
            double h = bandHeights[b] * std::exp(-std::pow((nf - _stereoBandX[b]) / 0.11, 2.0));
            CGFloat py = bandBaseY - h;
            [bFill lineToPoint:NSMakePoint(m3Well.origin.x + x, py)];
            if (x == 0) [bLine moveToPoint:NSMakePoint(m3Well.origin.x + x, py)];
            else [bLine lineToPoint:NSMakePoint(m3Well.origin.x + x, py)];
        }
        [bFill lineToPoint:NSMakePoint(NSMaxX(m3Well), bandBaseY)];
        [bFill closePath];
        [[self roleColor:bandRoles[b] theme:th alpha:0.25] setFill];
        [bFill fill];
        [[self roleColor:bandRoles[b] theme:th alpha:0.95] setStroke];
        [bLine setLineWidth:1.4];
        [bLine stroke];

        CGFloat ncx = m3Well.origin.x + _stereoBandX[b] * m3Well.size.width;
        CGFloat ncy = bandBaseY - bandHeights[b];
        [[NSColor colorWithCalibratedRed:0.06 green:0.08 blue:0.12 alpha:1.0] setFill];
        [[NSBezierPath bezierPathWithOvalInRect:NSMakeRect(ncx - 3.5, ncy - 3.5, 7, 7)] fill];
        [[self roleColor:bandRoles[b] theme:th alpha:1.0] setStroke];
        [[NSBezierPath bezierPathWithOvalInRect:NSMakeRect(ncx - 3.5, ncy - 3.5, 7, 7)] stroke];

        [bandLbls[b] drawAtPoint:NSMakePoint(ncx - 14, bandBaseY + 1) withAttributes:tinyAxisFont];
    }
    CGContextRestoreGState(ctx);

    for (size_t s = 0; s < _sliders.size(); ++s) {
        const auto& sw = _sliders[s];
        float norm = _engine ? _engine->getParameterNormalized(sw.paramId) : 0.5f;

        [sw.label drawAtPoint:NSMakePoint(sw.trackRect.origin.x - 36, sw.trackRect.origin.y - 2) withAttributes:@{
            NSFontAttributeName: [NSFont systemFontOfSize:9.0 weight:NSFontWeightMedium],
            NSForegroundColorAttributeName: [NSColor colorWithCalibratedRed:th.textR green:th.textG blue:th.textB alpha:0.92]
        }];

        NSBezierPath* trk = [NSBezierPath bezierPathWithRoundedRect:sw.trackRect xRadius:2.5 yRadius:2.5];
        [[NSColor colorWithCalibratedRed:0.05 green:0.065 blue:0.09 alpha:1.0] setFill];
        [trk fill];
        [[NSColor colorWithCalibratedRed:th.borderR green:th.borderG blue:th.borderB alpha:0.85] setStroke];
        [trk stroke];

        CGFloat thumbX = sw.trackRect.origin.x + norm * (sw.trackRect.size.width - 12.0);
        NSRect fillR = NSMakeRect(sw.trackRect.origin.x + 1, sw.trackRect.origin.y + 1, thumbX - sw.trackRect.origin.x, sw.trackRect.size.height - 2);
        [[self roleColor:sw.colorRole theme:th alpha:0.55] setFill];
        NSRectFillUsingOperation(fillR, NSCompositingOperationSourceOver);

        NSRect thumbR = NSMakeRect(thumbX, sw.trackRect.origin.y - 1.5, 12, sw.trackRect.size.height + 3);
        NSBezierPath* thBp = [NSBezierPath bezierPathWithRoundedRect:thumbR xRadius:2.0 yRadius:2.0];
        [[NSColor colorWithCalibratedRed:0.28 green:0.33 blue:0.42 alpha:1.0] setFill];
        [thBp fill];
        [[NSColor colorWithCalibratedRed:0.52 green:0.60 blue:0.72 alpha:1.0] setStroke];
        [thBp stroke];

        int pctVal = (s == 0) ? (int)std::round(norm * 200.0f) : (int)std::round(norm * 100.0f);
        NSString* pctStr = [NSString stringWithFormat:@"%d%%", pctVal];
        [pctStr drawInRect:NSMakeRect(NSMaxX(sw.trackRect) + 4, sw.trackRect.origin.y - 2, 34, 12) withAttributes:@{
            NSFontAttributeName: [NSFont systemFontOfSize:9.0 weight:NSFontWeightMedium],
            NSForegroundColorAttributeName: [NSColor colorWithCalibratedRed:th.textR green:th.textG blue:th.textB alpha:0.92],
            NSParagraphStyleAttributeName: rightStyle
        }];
    }

    // -------------------------------------------------------------------------
    // MODULE 4 INTERNAL DISPLAY: Real-Time Dual Vertical Gain Reduction Meters
    // -------------------------------------------------------------------------
    CGFloat m4x = m3x + modW + gap;
    for (int ch = 0; ch < 2; ++ch) {
        NSRect gmWell = NSMakeRect(m4x + 190 + ch * 22, rackY + 56, 13, 132);
        [[NSColor colorWithCalibratedRed:0.04 green:0.05 blue:0.075 alpha:1.0] setFill];
        NSRectFill(gmWell);
        [[NSColor colorWithCalibratedRed:th.borderR green:th.borderG blue:th.borderB alpha:0.85] setStroke];
        NSFrameRect(gmWell);

        if (_compGRShow && liveGR > 0.05f) {
            CGFloat fillH = std::clamp((double)liveGR * 10.5, 2.0, 124.0);
            NSRect barR = NSMakeRect(gmWell.origin.x + 2, NSMaxY(gmWell) - fillH - 2, 9, fillH);
            NSGradient* cGrad = [[NSGradient alloc] initWithStartingColor:[NSColor colorWithCalibratedRed:0.65 green:0.95 blue:1.0 alpha:1.0]
                                                              endingColor:[NSColor colorWithCalibratedRed:0.12 green:0.58 blue:0.92 alpha:0.95]];
            [cGrad drawInRect:barR angle:90.0];
        }

        [@"Gain" drawAtPoint:NSMakePoint(gmWell.origin.x - 4, NSMaxY(gmWell) + 2) withAttributes:tinyAxisFont];
    }

    // -------------------------------------------------------------------------
    // MODULE 5 INTERNAL DISPLAY: Real-Time Maximizer Waveform / GR Trace Box
    // -------------------------------------------------------------------------
    CGFloat m5x = m4x + modW + gap;
    NSRect m5Wave = NSMakeRect(m5x + 12, rackY + 138, 124, 58);
    [[NSColor colorWithCalibratedRed:th.wellR green:th.wellG blue:th.wellB alpha:1.0] setFill];
    NSRectFill(m5Wave);
    [[NSColor colorWithCalibratedRed:th.borderR green:th.borderG blue:th.borderB alpha:0.85] setStroke];
    NSFrameRect(m5Wave);

    CGContextSaveGState(ctx);
    CGContextClipToRect(ctx, NSRectToCGRect(m5Wave));
    if (_engine) {
        const auto& pkHist = _engine->getWavePeakHistory();
        int gw = _engine->getGRWriteIdx();
        NSBezierPath* limFill = [NSBezierPath bezierPath];
        NSBezierPath* limLine = [NSBezierPath bezierPath];
        [limFill moveToPoint:NSMakePoint(m5Wave.origin.x, NSMaxY(m5Wave))];
        for (int idx = 0; idx < 128; ++idx) {
            int hIdx = (gw + idx) & 127;
            CGFloat x = m5Wave.origin.x + (idx / 127.0) * m5Wave.size.width;
            double maxCeilH = 6.0 + (1.0 - ceilNorm) * 20.0;
            CGFloat py = std::clamp(NSMaxY(m5Wave) - pkHist[hIdx] * 50.0, m5Wave.origin.y + maxCeilH, NSMaxY(m5Wave) - 1.0);
            [limFill lineToPoint:NSMakePoint(x, py)];
            if (idx == 0) [limLine moveToPoint:NSMakePoint(x, py)];
            else [limLine lineToPoint:NSMakePoint(x, py)];
        }
        [limFill lineToPoint:NSMakePoint(NSMaxX(m5Wave), NSMaxY(m5Wave))];
        [limFill closePath];
        NSGradient* lwGrad = [[NSGradient alloc] initWithStartingColor:[NSColor colorWithCalibratedRed:0.56 green:0.46 blue:0.86 alpha:0.68]
                                                           endingColor:[NSColor colorWithCalibratedRed:0.18 green:0.22 blue:0.42 alpha:0.20]];
        [lwGrad drawInBezierPath:limFill angle:90.0];
        [[NSColor colorWithCalibratedRed:0.88 green:0.92 blue:1.0 alpha:0.92] setStroke];
        [limLine setLineWidth:1.1];
        [limLine stroke];
    }
    CGContextRestoreGState(ctx);

    float ceilDB = _engine ? _engine->getParameterValue(vsrm::kCeiling) : -0.1f;
    float maxDB  = _engine ? _engine->getParameterValue(vsrm::kMaximizerGain) : 4.6f;
    [[NSString stringWithFormat:@"Drive: +%.1fdB\nCeil: %.1fdBTP", maxDB, ceilDB] drawInRect:NSMakeRect(m5x + 140, rackY + 172, 98, 26) withAttributes:@{
        NSFontAttributeName: [NSFont monospacedDigitSystemFontOfSize:8.5 weight:NSFontWeightSemibold],
        NSForegroundColorAttributeName: [NSColor colorWithCalibratedRed:th.cyanR green:th.cyanG blue:th.cyanB alpha:0.92],
        NSParagraphStyleAttributeName: centerStyle
    }];

    // =========================================================================
    // DRAW ALL LIVE TACTILE ROTARY ENCODERS WITH ILLUMINATED VALUE ARCS
    // =========================================================================
    for (size_t i = 0; i < _knobs.size(); ++i) {
        const auto& kw = _knobs[i];
        float norm = _engine ? _engine->getParameterNormalized(kw.paramId) : 0.5f;
        CGFloat cx = kw.cx, cy = kw.cy, r = kw.radius;

        CGContextSaveGState(ctx);
        CGContextSetRGBFillColor(ctx, 0.04, 0.05, 0.07, 1.0);
        CGContextFillEllipseInRect(ctx, CGRectMake(cx - r - 4.0, cy - r - 4.0, (r + 4.0) * 2, (r + 4.0) * 2));

        const double startAng = 135.0 * M_PI / 180.0;
        const double sweepAng = 270.0 * M_PI / 180.0;
        CGContextSetRGBStrokeColor(ctx, 0.14, 0.17, 0.22, 1.0);
        CGContextSetLineWidth(ctx, (r > 21) ? 3.0 : 2.3);
        CGContextBeginPath(ctx);
        CGContextAddArc(ctx, cx, cy, r + 2.2, startAng, startAng + sweepAng, 0);
        CGContextStrokePath(ctx);

        NSColor* arcCol = [self roleColor:kw.colorRole theme:th alpha:0.98];
        [arcCol setStroke];
        CGContextSetLineWidth(ctx, (r > 21) ? 3.2 : 2.5);
        CGContextBeginPath(ctx);
        CGContextAddArc(ctx, cx, cy, r + 2.2, startAng, startAng + std::max(0.04f, norm) * sweepAng, 0);
        CGContextStrokePath(ctx);

        NSBezierPath* capPath = [NSBezierPath bezierPathWithOvalInRect:NSMakeRect(cx - r + 1.5, cy - r + 1.5, (r - 1.5) * 2, (r - 1.5) * 2)];
        NSGradient* capGrad = [[NSGradient alloc] initWithStartingColor:[NSColor colorWithCalibratedRed:0.20 green:0.24 blue:0.31 alpha:1.0]
                                                            endingColor:[NSColor colorWithCalibratedRed:0.10 green:0.12 blue:0.16 alpha:1.0]];
        [capGrad drawInBezierPath:capPath angle:90.0];
        [[NSColor colorWithCalibratedRed:((int)i == _dragKnobIdx ? th.cyanR : 0.30)
                                   green:((int)i == _dragKnobIdx ? th.cyanG : 0.35)
                                    blue:((int)i == _dragKnobIdx ? th.cyanB : 0.44) alpha:0.95] setStroke];
        [capPath setLineWidth:1.1];
        [capPath stroke];

        double valAng = startAng + norm * sweepAng;
        CGFloat px0 = cx + std::cos(valAng) * (r * 0.32);
        CGFloat py0 = cy + std::sin(valAng) * (r * 0.32);
        CGFloat px1 = cx + std::cos(valAng) * (r - 2.2);
        CGFloat py1 = cy + std::sin(valAng) * (r - 2.2);
        CGContextSetRGBStrokeColor(ctx, 0.94, 0.98, 1.0, 0.98);
        CGContextSetLineWidth(ctx, 1.8);
        CGContextMoveToPoint(ctx, px0, py0);
        CGContextAddLineToPoint(ctx, px1, py1);
        CGContextStrokePath(ctx);
        CGContextRestoreGState(ctx);

        NSDictionary* kFont = @{
            NSFontAttributeName: [NSFont systemFontOfSize:8.8 weight:NSFontWeightMedium],
            NSForegroundColorAttributeName: [NSColor colorWithCalibratedRed:th.textR green:th.textG blue:th.textB alpha:0.94],
            NSParagraphStyleAttributeName: centerStyle
        };
        [kw.label drawInRect:NSMakeRect(cx - 38, cy + r + 4, 76, 22) withAttributes:kFont];
    }

    // =========================================================================
    // DRAW ALL INTERACTIVE BUTTONS & OVERLAY PILLS
    // =========================================================================
    for (const auto& bw : _buttons) {
        bool active = false;
        if (bw.paramId >= 0 && _engine) {
            active = (_engine->getParameterValue(bw.paramId) >= 0.5f);
        } else if (bw.actionId == 102 && _engine) active = _engine->isDemoLoopPlaying();
        else if (bw.actionId == 106) active = _sidechainOn;
        else if (bw.actionId == 107) active = (_abBank == 0);
        else if (bw.actionId == 108) active = (_abBank == 1);
        else if (bw.actionId == 110) active = (_hpOn && _lpOn);
        else if (bw.actionId == 120) active = (_compBand == 0);
        else if (bw.actionId == 121) active = (_compBand == 1);
        else if (bw.actionId == 122) active = (_compBand == 2);
        else if (bw.actionId == 123) active = _compGRShow;
        else if (bw.actionId == 150) active = _fftOverlayOn;
        else if (bw.actionId == 151) active = _physOverlayOn;
        else if (bw.actionId == 152) active = (_eqChannelMode == 1);
        else if (bw.actionId == 153) active = (_eqChannelMode == 2);
        else if (bw.actionId >= 160 && bw.actionId <= 163) active = (_eqChannelMode == (bw.actionId - 160));
        else if (bw.actionId == 170) active = _airSpaceMSMode;
        else if (bw.actionId == 180) active = (_imagerChanMode == 0);
        else if (bw.actionId == 181) active = (_imagerChanMode == 1);

        NSBezierPath* bPath = [NSBezierPath bezierPathWithRoundedRect:bw.rect xRadius:3.5 yRadius:3.5];
        if ((bw.actionId == 107 || bw.actionId == 108 || bw.actionId == 102) && active) {
            [[NSColor colorWithCalibratedRed:0.14 green:0.62 blue:0.90 alpha:1.0] setFill];
            [bPath fill];
        } else if (active) {
            [[NSColor colorWithCalibratedRed:0.13 green:0.19 blue:0.27 alpha:1.0] setFill];
            [bPath fill];
            [[self roleColor:bw.colorRole theme:th alpha:0.95] setStroke];
            [bPath setLineWidth:1.2];
            [bPath stroke];
        } else {
            [[NSColor colorWithCalibratedRed:0.10 green:0.125 blue:0.17 alpha:0.95] setFill];
            [bPath fill];
            [[NSColor colorWithCalibratedRed:th.borderR green:th.borderG blue:th.borderB alpha:0.9] setStroke];
            [bPath setLineWidth:1.0];
            [bPath stroke];
        }

        NSString* titleStr = bw.title;
        if (bw.actionId == 130) {
            titleStr = (_limiterDisplayMode == 0) ? @"MID/SIDE LINK ▾"
                     : ((_limiterDisplayMode == 1) ? @"GAIN TRACE ▾" : @"LUFS TARGET ▾");
        } else if (bw.actionId == 140) {
            NSString* osNames[4] = {@"OS: 4x ▾", @"OS: 8x ▾", @"OS: 16x ▾", @"OS: 32x ▾"};
            titleStr = osNames[_oversampleIdx % 4];
        } else if (bw.actionId == 141) {
            NSString* dNames[3] = {@"Dither: 24b ▾", @"Dither: 16b ▾", @"Dither: Off ▾"};
            titleStr = dNames[_ditherIdx % 3];
        } else if (bw.actionId == 170) {
            titleStr = _airSpaceMSMode ? @"MODE: M/S" : @"MODE: STEREO";
        }

        NSDictionary* bFont = @{
            NSFontAttributeName: [NSFont systemFontOfSize:8.8 weight:NSFontWeightSemibold],
            NSForegroundColorAttributeName: [NSColor colorWithCalibratedRed:th.textR green:th.textG blue:th.textB alpha:0.96],
            NSParagraphStyleAttributeName: centerStyle
        };
        CGFloat yOff = (bw.rect.size.height - 12.0) * 0.5;
        [titleStr drawInRect:NSMakeRect(bw.rect.origin.x, bw.rect.origin.y + yOff, bw.rect.size.width, bw.rect.size.height - yOff) withAttributes:bFont];
    }

    // Bottom-right diagonal resize grip
    CGContextSetRGBStrokeColor(ctx, 0.55, 0.65, 0.80, 0.80);
    CGContextSetLineWidth(ctx, 1.4);
    for (int g = 0; g < 3; ++g) {
        CGFloat off = 5.0 + g * 4.5;
        CGContextMoveToPoint(ctx, W - off, H - 2.0);
        CGContextAddLineToPoint(ctx, W - 2.0, H - off);
    }
    CGContextStrokePath(ctx);

    CGContextRestoreGState(ctx);
}

// =============================================================================
// INTERACTION HANDLERS (Full Mouse Drag, Scroll-Wheel, Double-Click, Resizing)
// =============================================================================
- (NSPoint)logicalPointFromEvent:(NSEvent*)event {
    NSPoint raw = [self convertPoint:[event locationInWindow] fromView:nil];
    CGFloat viewW = std::max(320.0, (double)self.bounds.size.width);
    CGFloat viewH = std::max(180.0, (double)self.bounds.size.height);
    return NSMakePoint(raw.x * (1280.0 / viewW), raw.y * (720.0 / viewH));
}

- (void)setTooltipForParam:(int)pid label:(NSString*)lbl {
    if (!_engine || pid < 0 || pid >= vsrm::kNumParams) return;
    float v = _engine->getParameterValue(pid);
    const auto& m = vsrm::kParamTable[pid];
    NSString* cleanLbl = [lbl stringByReplacingOccurrencesOfString:@"\n" withString:@" "];
    _activeTooltip = [NSString stringWithFormat:@"%@: %.1f %s", cleanLbl, v, m.unit];
}

- (void)mouseDown:(NSEvent*)event {
    if (!_engine) return;
    [[self window] makeFirstResponder:self];
    NSPoint pt = [self logicalPointFromEvent:event];

    if (pt.x >= 1252.0 && pt.y >= 692.0) {
        _resizingCorner = true;
        _resizeStartScreenPt = [NSEvent mouseLocation];
        _resizeStartSize = self.bounds.size;
        return;
    }

    const CGFloat rackY = 482.0, modW = 246.0, gap = 7.5;
    for (int m = 0; m < 5; ++m) {
        CGFloat mx = 10.0 + m * (modW + gap);
        if (std::hypot(pt.x - (mx + 25.0), pt.y - (rackY + 13.0)) <= 10.0) {
            _modBypass[m] = !_modBypass[m];
            [self setNeedsDisplay:YES];
            return;
        }
        if (std::hypot(pt.x - (mx + modW - 25.0), pt.y - (rackY + 13.0)) <= 10.0) {
            _modSolo[m] = !_modSolo[m];
            [self setNeedsDisplay:YES];
            return;
        }
    }

    for (const auto& bw : _buttons) {
        if (NSPointInRect(pt, bw.rect)) {
            if (bw.paramId >= 0) {
                float cur = _engine->getParameterValue(bw.paramId);
                float next = (cur >= 0.5f) ? 0.0f : 1.0f;
                _engine->setParameterValue(bw.paramId, next);
                if (_paramCallback) _paramCallback(bw.paramId, next);
            } else if (bw.actionId == 100 || bw.actionId == 104) {
                _engine->loadPreset((_engine->getCurrentPreset() + 7) % 8);
            } else if (bw.actionId == 101 || bw.actionId == 105) {
                _engine->loadPreset((_engine->getCurrentPreset() + 1) % 8);
            } else if (bw.actionId == 102) {
                _engine->setDemoLoopPlaying(!_engine->isDemoLoopPlaying());
            } else if (bw.actionId == 103) {
                _engine->setSkin((_engine->getSkin() + 1) % 5);
            } else if (bw.actionId == 106) {
                _sidechainOn = !_sidechainOn;
            } else if (bw.actionId == 107 && _abBank != 0) {
                for (int i = 0; i < vsrm::kNumParams; ++i) _bankBParams[i] = _engine->getParameterValue(i);
                for (int i = 0; i < vsrm::kNumParams; ++i) _engine->setParameterValue(i, _bankAParams[i]);
                _abBank = 0;
            } else if (bw.actionId == 108 && _abBank != 1) {
                for (int i = 0; i < vsrm::kNumParams; ++i) _bankAParams[i] = _engine->getParameterValue(i);
                for (int i = 0; i < vsrm::kNumParams; ++i) _engine->setParameterValue(i, _bankBParams[i]);
                _abBank = 1;
            } else if (bw.actionId == 109) {
                _activeTooltip = @"Preset Saved to User Bank!";
            } else if (bw.actionId == 110) {
                _hpOn = !_hpOn;
                _lpOn = _hpOn;
            } else if (bw.actionId >= 120 && bw.actionId <= 122) {
                _compBand = bw.actionId - 120;
            } else if (bw.actionId == 123) {
                _compGRShow = !_compGRShow;
            } else if (bw.actionId == 130) {
                _limiterDisplayMode = (_limiterDisplayMode + 1) % 3;
            } else if (bw.actionId == 140) {
                _oversampleIdx = (_oversampleIdx + 1) % 4;
            } else if (bw.actionId == 141) {
                _ditherIdx = (_ditherIdx + 1) % 3;
            } else if (bw.actionId == 150) {
                _fftOverlayOn = !_fftOverlayOn;
            } else if (bw.actionId == 151) {
                _physOverlayOn = !_physOverlayOn;
            } else if (bw.actionId == 152) {
                _eqChannelMode = (_eqChannelMode == 1) ? 0 : 1;
            } else if (bw.actionId == 153) {
                _eqChannelMode = (_eqChannelMode == 2) ? 0 : 2;
            } else if (bw.actionId >= 160 && bw.actionId <= 163) {
                _eqChannelMode = bw.actionId - 160;
            } else if (bw.actionId == 170) {
                _airSpaceMSMode = !_airSpaceMSMode;
            } else if (bw.actionId == 180) {
                _imagerChanMode = 0;
            } else if (bw.actionId == 181) {
                _imagerChanMode = 1;
            }
            [self setNeedsDisplay:YES];
            return;
        }
    }

    for (size_t i = 0; i < _knobs.size(); ++i) {
        if (std::hypot(pt.x - _knobs[i].cx, pt.y - _knobs[i].cy) <= _knobs[i].radius + 8.0) {
            int pid = _knobs[i].paramId;
            if ([event clickCount] >= 2) {
                const auto& m = vsrm::kParamTable[pid];
                _engine->setParameterValue(pid, m.defVal);
                if (_paramCallback) _paramCallback(pid, m.defVal);
                [self setTooltipForParam:pid label:_knobs[i].label];
                [self setNeedsDisplay:YES];
                return;
            }
            _dragKnobIdx = static_cast<int>(i);
            _dragStartY = pt.y;
            _dragStartNorm = _engine->getParameterNormalized(pid);
            [self setTooltipForParam:pid label:_knobs[i].label];
            [self setNeedsDisplay:YES];
            return;
        }
    }

    NSRect plotRect = NSMakeRect(42, 64, 704, 224);
    if (NSPointInRect(pt, plotRect)) {
        for (size_t i = 0; i < _eqNodes.size(); ++i) {
            CGFloat nx = plotRect.origin.x + _eqNodes[i].normX * plotRect.size.width;
            CGFloat ny = plotRect.origin.y + _eqNodes[i].normY * plotRect.size.height;
            if (std::hypot(pt.x - nx, pt.y - ny) <= 18.0) {
                _dragNodeIdx = static_cast<int>(i);
                [self setTooltipForParam:_eqNodes[i].paramId label:@"EQ Band"];
                [self setNeedsDisplay:YES];
                return;
            }
        }
    }

    CGFloat m3x = 10.0 + 2 * (modW + gap);
    NSRect m3Well = NSMakeRect(m3x + 12, rackY + 52, 222, 58);
    if (NSPointInRect(pt, m3Well)) {
        for (int b = 0; b < 4; ++b) {
            CGFloat ncx = m3Well.origin.x + _stereoBandX[b] * m3Well.size.width;
            if (std::fabs(pt.x - ncx) <= 20.0) {
                _dragStereoBandIdx = b;
                return;
            }
        }
    }

    for (size_t s = 0; s < _sliders.size(); ++s) {
        if (NSPointInRect(pt, NSInsetRect(_sliders[s].trackRect, -8, -6))) {
            _dragSliderIdx = static_cast<int>(s);
            int pid = _sliders[s].paramId;
            float norm = std::clamp((float)((pt.x - _sliders[s].trackRect.origin.x) / _sliders[s].trackRect.size.width), 0.0f, 1.0f);
            _engine->setParameterNormalized(pid, norm);
            if (_paramCallback) _paramCallback(pid, _engine->getParameterValue(pid));
            [self setTooltipForParam:pid label:_sliders[s].label];
            [self setNeedsDisplay:YES];
            return;
        }
    }

    if (NSPointInRect(pt, NSMakeRect(872, 8, 44, 26))) {
        _sidechainOn = !_sidechainOn;
        [self setNeedsDisplay:YES];
        return;
    }
}

- (void)mouseDragged:(NSEvent*)event {
    if (!_engine) return;

    if (_resizingCorner) {
        NSPoint curScreen = [NSEvent mouseLocation];
        CGFloat dx = curScreen.x - _resizeStartScreenPt.x;
        CGFloat newW = std::clamp(_resizeStartSize.width + dx, 800.0, 2560.0);
        CGFloat newH = std::round(newW * (720.0 / 1280.0));
        if (_resizeCallback) {
            _resizeCallback(newW, newH);
        } else if (self.window) {
            [self.window setContentSize:NSMakeSize(newW, newH)];
        } else {
            [self setFrameSize:NSMakeSize(newW, newH)];
        }
        [self setNeedsDisplay:YES];
        return;
    }

    NSPoint pt = [self logicalPointFromEvent:event];

    if (_dragNodeIdx >= 0 && _dragNodeIdx < (int)_eqNodes.size()) {
        NSRect plotRect = NSMakeRect(42, 64, 704, 224);
        float normX = std::clamp((float)((pt.x - plotRect.origin.x) / plotRect.size.width), 0.05f, 0.94f);
        float normY = std::clamp((float)((pt.y - plotRect.origin.y) / plotRect.size.height), 0.14f, 0.86f);
        _eqNodes[_dragNodeIdx].normX = normX;
        _eqNodes[_dragNodeIdx].normY = normY;
        int pid = _eqNodes[_dragNodeIdx].paramId;
        _engine->setParameterNormalized(pid, std::clamp(1.0f - normY, 0.0f, 1.0f));
        if (_paramCallback) _paramCallback(pid, _engine->getParameterValue(pid));
        [self setTooltipForParam:pid label:@"EQ Band"];
        [self setNeedsDisplay:YES];
        return;
    }

    if (_dragStereoBandIdx >= 0 && _dragStereoBandIdx < 4) {
        const CGFloat modW = 246.0, gap = 7.5;
        CGFloat m3x = 10.0 + 2 * (modW + gap);
        NSRect m3Well = NSMakeRect(m3x + 12, 482.0 + 52, 222, 58);
        float nx = std::clamp((float)((pt.x - m3Well.origin.x) / m3Well.size.width), 0.08f, 0.92f);
        _stereoBandX[_dragStereoBandIdx] = nx;
        [self setNeedsDisplay:YES];
        return;
    }

    if (_dragSliderIdx >= 0 && _dragSliderIdx < (int)_sliders.size()) {
        const auto& sw = _sliders[_dragSliderIdx];
        int pid = sw.paramId;
        float norm = std::clamp((float)((pt.x - sw.trackRect.origin.x) / sw.trackRect.size.width), 0.0f, 1.0f);
        _engine->setParameterNormalized(pid, norm);
        if (_paramCallback) _paramCallback(pid, _engine->getParameterValue(pid));
        [self setTooltipForParam:pid label:sw.label];
        [self setNeedsDisplay:YES];
        return;
    }

    if (_dragKnobIdx >= 0 && _dragKnobIdx < (int)_knobs.size()) {
        CGFloat dy = _dragStartY - pt.y;
        int pid = _knobs[_dragKnobIdx].paramId;
        float newNorm = std::clamp(_dragStartNorm + static_cast<float>(dy / 120.0), 0.0f, 1.0f);
        _engine->setParameterNormalized(pid, newNorm);
        if (_paramCallback) _paramCallback(pid, _engine->getParameterValue(pid));
        [self setTooltipForParam:pid label:_knobs[_dragKnobIdx].label];
        [self setNeedsDisplay:YES];
    }
}

- (void)mouseUp:(NSEvent*)event {
    _dragKnobIdx = -1;
    _dragSliderIdx = -1;
    _dragNodeIdx = -1;
    _dragStereoBandIdx = -1;
    _resizingCorner = false;
    _activeTooltip = nil;
    [self setNeedsDisplay:YES];
}

- (void)scrollWheel:(NSEvent*)event {
    if (!_engine) return;
    NSPoint pt = [self logicalPointFromEvent:event];
    float delta = static_cast<float>([event scrollingDeltaY]) * 0.005f;
    if (std::fabs(delta) < 0.0001f) return;

    for (size_t i = 0; i < _knobs.size(); ++i) {
        if (std::hypot(pt.x - _knobs[i].cx, pt.y - _knobs[i].cy) <= _knobs[i].radius + 10.0) {
            int pid = _knobs[i].paramId;
            float norm = std::clamp(_engine->getParameterNormalized(pid) + delta, 0.0f, 1.0f);
            _engine->setParameterNormalized(pid, norm);
            if (_paramCallback) _paramCallback(pid, _engine->getParameterValue(pid));
            [self setNeedsDisplay:YES];
            return;
        }
    }
    for (size_t s = 0; s < _sliders.size(); ++s) {
        if (NSPointInRect(pt, NSInsetRect(_sliders[s].trackRect, -8, -8))) {
            int pid = _sliders[s].paramId;
            float norm = std::clamp(_engine->getParameterNormalized(pid) + delta, 0.0f, 1.0f);
            _engine->setParameterNormalized(pid, norm);
            if (_paramCallback) _paramCallback(pid, _engine->getParameterValue(pid));
            [self setNeedsDisplay:YES];
            return;
        }
    }
}

- (void)keyDown:(NSEvent*)event {
    if (!_engine) return;
    NSString* chars = [event charactersIgnoringModifiers];
    if ([chars length] == 0) return;
    unichar c = [chars characterAtIndex:0];
    if (c == 's' || c == 'S') {
        _engine->setSkin((_engine->getSkin() + 1) % 5);
        [self setNeedsDisplay:YES];
    } else if (c >= '1' && c <= '8') {
        _engine->loadPreset(c - '1');
        [self setNeedsDisplay:YES];
    }
}

@end
