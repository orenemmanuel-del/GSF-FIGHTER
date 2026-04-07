# GSF FIGHTER

**Master Bus Streaming System x Gesaffelstein**

Stream your DAW master bus in real-time to mobile devices over WiFi. Insert on master, press FIGHT!, and your mix plays instantly on any connected phone — AirPods, car system, cheap earbuds, club monitors.

Inspired by Mix to Mobile. Elevated by Gesaffelstein's 6 Translation presets. Wrapped in a Street Fighter arcade aesthetic.

---

## Architecture

```
GSF-FIGHTER/
├── Plugin/                    # JUCE 8 audio plugin (VST3/AU/AAX/Standalone)
│   ├── CMakeLists.txt
│   └── Source/
│       ├── PluginProcessor.*  # Audio capture + FIFO to network thread
│       ├── PluginEditor.*     # Main arcade UI assembly
│       ├── Network/
│       │   ├── StreamServer.* # TCP streaming server (multi-client)
│       │   ├── DeviceDiscovery.* # UDP broadcast for auto-detection
│       │   └── ClientSession.*   # Per-client connection management
│       ├── Presets/
│       │   └── TranslationEngine.* # 6 GSF EQ/simulation presets
│       └── ArcadeUI/
│           ├── GSFLookAndFeel.*    # Street Fighter colour palette + drawing
│           ├── HealthBarMeter.*    # SF health bar = audio level meter
│           ├── ArcadeButton.*     # Hadouken-style buttons
│           ├── ConnectionPanel.*  # Code display + start/stop + meters
│           ├── PresetPanel.*      # Fighter character select grid
│           └── FightScreen.*      # READY? → FIGHT! overlay animation
├── MobileApp/                 # Flutter mobile app (iOS + Android)
│   ├── pubspec.yaml
│   └── lib/
│       ├── main.dart              # App entry point
│       ├── core/
│       │   ├── gsf_protocol.dart  # Protocol (Dart mirror of C++ header)
│       │   └── app_state.dart     # Central state management
│       ├── network/
│       │   ├── discovery_client.dart  # UDP listener for auto-detection
│       │   └── stream_client.dart     # TCP client for audio stream
│       ├── audio/
│       │   └── audio_engine.dart      # Jitter buffer + playback
│       └── ui/
│           ├── theme/gsf_theme.dart   # Arcade colour palette + theme
│           ├── widgets/               # HealthBar, ArcadeButton, Scanlines
│           ├── screens/               # Home, Listening, PresetSelect
│           └── animations/            # READY? → FIGHT! overlay
├── Shared/
│   └── GSFProtocol.h          # Binary protocol shared by plugin + app
└── README.md
```

## Translation Presets (by Gesaffelstein)

| # | Preset | Fighter | Simulation |
|---|--------|---------|------------|
| 0 | FLAT | BYPASS | No processing |
| 1 | GSF iPHONE FIGHTER | RYU | Tiny speaker: no sub, harsh mids, 200Hz HPF |
| 2 | GSF AIRPODS FIGHTER | CHUN-LI | ANC seal, slight bass boost, intimate |
| 3 | GSF VOITURE FIGHTER | GUILE | Car cabin: sub emphasis, road noise masking |
| 4 | GSF CLUB FIGHTER | AKUMA | PA system: massive sub, scooped mids, wide |
| 5 | GSF CHEAP FIGHTER | BLANKA | Thin: no bass, harsh highs, distortion |
| 6 | GSF STUDIO FIGHTER | SAGAT | Near-field monitors with room interaction |

Each preset applies a 5-band parametric EQ + high/low cut + stereo width + saturation to simulate the target playback system.

## Streaming Protocol

- **Discovery**: UDP broadcast on port 47700 (1 Hz)
- **Stream**: TCP on port 47701 (24-bit PCM, interleaved stereo)
- **Control**: Embedded in TCP stream (preset changes, heartbeats)
- **Latency target**: < 50ms (audio thread → FIFO → TCP send)
- **Multi-client**: Up to 8 simultaneous connections
- **Authentication**: 4-digit PIN code

## Build Instructions

### Prerequisites

- **CMake** 3.22+
- **C++20** compiler (Clang 14+, GCC 12+, MSVC 2022+)
- **JUCE 8.0.4** (auto-fetched via CMake FetchContent)

#### macOS
```bash
# Install Xcode command line tools
xcode-select --install

# Install CMake (if needed)
brew install cmake
```

#### Windows
```
# Install Visual Studio 2022 with C++ workload
# Install CMake from https://cmake.org/download/
```

### Build the Plugin

```bash
cd GSF-FIGHTER/Plugin

# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release

# The plugin will be auto-copied to your system plugin folder
```

#### macOS specific
```bash
# For Xcode project generation:
cmake -B build -G Xcode

# Open in Xcode:
open build/GSF_FIGHTER.xcodeproj
```

#### Windows specific
```bash
# For Visual Studio:
cmake -B build -G "Visual Studio 17 2022"

# Open the .sln in Visual Studio
```

### Plugin Formats Built

| Format | macOS | Windows |
|--------|-------|---------|
| VST3   | Yes   | Yes     |
| AU     | Yes   | No      |
| AAX    | Yes   | Yes     |
| Standalone | Yes | Yes  |

### Firewall Configuration

#### macOS
Allow incoming connections when prompted by the system dialog. If blocked:
```
System Settings → Network → Firewall → Options → Add GSF FIGHTER
```

#### Windows
The plugin will attempt to open ports 47700-47702. If Windows Firewall blocks it:
```
Windows Security → Firewall → Allow an app → Add GSF FIGHTER
```
Or via PowerShell (admin):
```powershell
New-NetFirewallRule -DisplayName "GSF FIGHTER" -Direction Inbound -Protocol TCP -LocalPort 47701 -Action Allow
New-NetFirewallRule -DisplayName "GSF FIGHTER Discovery" -Direction Inbound -Protocol UDP -LocalPort 47700 -Action Allow
```

## Usage

1. Insert **GSF FIGHTER** on your DAW master bus (last insert)
2. Note the 4-digit connection code displayed in the plugin
3. Open the GSF FIGHTER mobile app on your phone
4. Enter the code or auto-detect the plugin on your network
5. Press **FIGHT!** in the plugin to start streaming
6. Select a Translation preset to hear how your mix translates
7. Your mix now plays on the phone in real-time

## Network Requirements

- Plugin host and mobile devices must be on the **same WiFi network**
- UDP broadcast must not be blocked (required for auto-discovery)
- Recommended: 5 GHz WiFi for lowest latency
- Wired Ethernet on the DAW side is ideal

## Build the Mobile App

### Prerequisites

- **Flutter** 3.3+ ([install](https://docs.flutter.dev/get-started/install))
- **Xcode 15+** (iOS)
- **Android Studio** (Android)

### Build & Run

```bash
cd GSF-FIGHTER/MobileApp

# Get dependencies
flutter pub get

# Run on connected device
flutter run

# Build release iOS
flutter build ios --release

# Build release Android
flutter build apk --release
```

### iOS Notes

- Local network permission dialog will appear on first launch — user must accept
- Background audio is enabled (streaming continues when app is backgrounded)
- Requires iOS 16.0+

### Android Notes

- `CHANGE_WIFI_MULTICAST_STATE` permission enables UDP discovery
- Cleartext traffic allowed for local network IPs only (security config)
- Requires Android 8.0+ (API 26)

### Mobile App Screens

1. **Home Screen** — auto-discovers plugins on WiFi, shows host list, manual code entry
2. **Listening Screen** — live meters (SF health bars), active preset, buffer stats, STOP button
3. **Preset Select** — "SELECT YOUR FIGHTER" grid with 7 presets as fighter characters

## Tech Stack

- **Plugin**: JUCE 8, C++20, CMake (VST3/AU/AAX/Standalone)
- **Mobile App**: Flutter 3.3+ (iOS + Android single codebase)
- **Protocol**: Custom binary over TCP (stream) + UDP (discovery)
- **Audio**: 24-bit PCM, up to 96kHz, 30ms jitter buffer
- **State**: Provider (Flutter), lock-free FIFO (JUCE)

---

*GSF FIGHTER x Gesaffelstein — Dark, powerful, professional.*
