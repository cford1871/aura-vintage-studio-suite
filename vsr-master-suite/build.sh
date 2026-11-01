#!/bin/zsh
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$ROOT_DIR/build"
mkdir -p "$BUILD_DIR"

CXXFLAGS=(-std=c++17 -O3 -fobjc-arc -ObjC++ -arch arm64 -arch x86_64 -I"$ROOT_DIR/include")
FRAMEWORKS=(-framework Cocoa -framework QuartzCore -framework AudioUnit -framework AudioToolbox -framework CoreAudio -framework Accelerate)

echo "=== [1/3] Building VST3 Mastering Console: VSR-MASTER.vst3 ==="
VST3_DIR="$BUILD_DIR/VSR-MASTER.vst3/Contents"
mkdir -p "$VST3_DIR/MacOS" "$VST3_DIR/Resources"

clang++ "${CXXFLAGS[@]}" -DVSRMasterHardwareView=VSRMasterHardwareView_VST3 -bundle "${FRAMEWORKS[@]}" \
    "$ROOT_DIR/src/VST3Plugin.mm" \
    -o "$VST3_DIR/MacOS/VSR-MASTER"

cat > "$VST3_DIR/Info.plist" << 'EOF'
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleDevelopmentRegion</key>
    <string>English</string>
    <key>CFBundleExecutable</key>
    <string>VSR-MASTER</string>
    <key>CFBundleIdentifier</key>
    <string>com.beatskillz.vsrmaster.vst3</string>
    <key>CFBundleInfoDictionaryVersion</key>
    <string>6.0</string>
    <key>CFBundleName</key>
    <string>VSR-MASTER</string>
    <key>CFBundleDisplayName</key>
    <string>VSR-MASTER Console</string>
    <key>CFBundlePackageType</key>
    <string>BNDL</string>
    <key>CFBundleShortVersionString</key>
    <string>1.2.0</string>
    <key>CFBundleVersion</key>
    <string>1.2.0</string>
    <key>CFBundleSignature</key>
    <string>????</string>
    <key>CSResourcesFileMapped</key>
    <true/>
    <key>NSHighResolutionCapable</key>
    <true/>
</dict>
</plist>
EOF
printf "BNDL????" > "$VST3_DIR/PkgInfo"

echo "=== [2/3] Building Audio Unit (aufx) Mastering Console: VSR-MASTER.component ==="
AU_DIR="$BUILD_DIR/VSR-MASTER.component/Contents"
mkdir -p "$AU_DIR/MacOS" "$AU_DIR/Resources"

clang++ "${CXXFLAGS[@]}" -arch arm64e -DVSRMasterHardwareView=VSRMasterHardwareView_AU -bundle "${FRAMEWORKS[@]}" \
    "$ROOT_DIR/src/AUPlugin.mm" \
    -o "$AU_DIR/MacOS/VSR-MASTER"

cat > "$AU_DIR/Info.plist" << 'EOF'
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleDevelopmentRegion</key>
    <string>English</string>
    <key>CFBundleExecutable</key>
    <string>VSR-MASTER</string>
    <key>CFBundleIdentifier</key>
    <string>com.beatskillz.vsrmaster.au</string>
    <key>CFBundleInfoDictionaryVersion</key>
    <string>6.0</string>
    <key>CFBundleName</key>
    <string>VSR-MASTER</string>
    <key>CFBundleDisplayName</key>
    <string>VSR-MASTER Console</string>
    <key>CFBundlePackageType</key>
    <string>BNDL</string>
    <key>CFBundleShortVersionString</key>
    <string>1.2.0</string>
    <key>CFBundleVersion</key>
    <string>1.2.0</string>
    <key>CFBundleSignature</key>
    <string>????</string>
    <key>NSHighResolutionCapable</key>
    <true/>
    <key>AudioComponents</key>
    <array>
        <dict>
            <key>name</key>
            <string>Vintage Studio DSP: VSR-MASTER Console</string>
            <key>description</key>
            <string>VSR-MASTER Ozone 12 + Soothe3 + Tape56K Master &amp; Mix Effects Processor</string>
            <key>factoryFunction</key>
            <string>VSRMasterAUFactory</string>
            <key>manufacturer</key>
            <string>BSKZ</string>
            <key>subtype</key>
            <string>VMSR</string>
            <key>type</key>
            <string>aufx</string>
            <key>version</key>
            <integer>66048</integer>
            <key>sandboxSafe</key>
            <true/>
            <key>tags</key>
            <array>
                <string>Effects</string>
                <string>Mastering</string>
                <string>Dynamics</string>
                <string>Equalizer</string>
            </array>
        </dict>
    </array>
</dict>
</plist>
EOF
printf "BNDL????" > "$AU_DIR/PkgInfo"

echo "=== [3/3] Building Standalone Studio Mastering Host: VSR-MASTER.app ==="
APP_DIR="$BUILD_DIR/VSR-MASTER.app/Contents"
mkdir -p "$APP_DIR/MacOS" "$APP_DIR/Resources"

clang++ "${CXXFLAGS[@]}" "${FRAMEWORKS[@]}" \
    "$ROOT_DIR/src/StandaloneApp.mm" \
    -o "$APP_DIR/MacOS/VSR-MASTER"

cat > "$APP_DIR/Info.plist" << 'EOF'
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleExecutable</key>
    <string>VSR-MASTER</string>
    <key>CFBundleIdentifier</key>
    <string>com.beatskillz.vsrmaster.app</string>
    <key>CFBundleName</key>
    <string>VSR-MASTER</string>
    <key>CFBundleDisplayName</key>
    <string>VSR-MASTER Console</string>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>CFBundleShortVersionString</key>
    <string>1.2.0</string>
    <key>CFBundleVersion</key>
    <string>1.2.0</string>
    <key>NSHighResolutionCapable</key>
    <true/>
</dict>
</plist>
EOF

codesign --force --deep --sign - "$BUILD_DIR/VSR-MASTER.vst3" 2>/dev/null || true
codesign --force --deep --sign - "$BUILD_DIR/VSR-MASTER.component" 2>/dev/null || true
codesign --force --deep --sign - "$BUILD_DIR/VSR-MASTER.app" 2>/dev/null || true

echo "=== Build Complete! ==="
ls -ld "$BUILD_DIR/VSR-MASTER.vst3" "$BUILD_DIR/VSR-MASTER.component" "$BUILD_DIR/VSR-MASTER.app"
