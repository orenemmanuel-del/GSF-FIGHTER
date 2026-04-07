/// GSF FIGHTER - App State
/// Central state management connecting network, audio, and UI.

import 'dart:io';
import 'package:flutter/foundation.dart';
import '../network/discovery_client.dart';
import '../network/stream_client.dart';
import '../audio/audio_engine.dart';
import 'gsf_protocol.dart';

class GSFAppState extends ChangeNotifier {
  final DiscoveryClient discovery = DiscoveryClient();
  final StreamClient stream = StreamClient();
  final AudioEngine audio = AudioEngine();

  DiscoveredHost? _selectedHost;
  PresetID _currentPreset = PresetID.flat;
  bool _fightSequenceTriggered = false;
  bool _koSequenceTriggered = false;

  GSFAppState() {
    // Wire stream audio data to audio engine
    stream.onAudioData = (left, right, numSamples) {
      audio.pushAudio(left, right, numSamples);
    };

    // Wire state changes
    stream.onStateChanged = (state) {
      if (state == ConnectionState.connected && stream.audioConfig != null) {
        audio.configure(stream.audioConfig!);
      } else if (state == ConnectionState.disconnected) {
        audio.stop();
      }
      notifyListeners();
    };

    // Forward discovery and audio changes
    discovery.addListener(_onSubStateChanged);
    stream.addListener(_onSubStateChanged);
    audio.addListener(_onSubStateChanged);
  }

  // --- Getters ---
  DiscoveredHost? get selectedHost => _selectedHost;
  PresetID get currentPreset => _currentPreset;
  bool get isConnected => stream.isConnected;
  bool get isStreaming => stream.state == ConnectionState.streaming;
  bool get isDiscovering => discovery.isListening;
  List<DiscoveredHost> get availableHosts => discovery.hosts;

  bool get fightSequenceTriggered {
    if (_fightSequenceTriggered) {
      _fightSequenceTriggered = false;
      return true;
    }
    return false;
  }

  bool get koSequenceTriggered {
    if (_koSequenceTriggered) {
      _koSequenceTriggered = false;
      return true;
    }
    return false;
  }

  String get deviceName {
    try {
      return Platform.localHostname;
    } catch (_) {
      return 'GSF Mobile';
    }
  }

  // --- Actions ---

  /// Start scanning for plugins on the network.
  Future<void> startDiscovery() async {
    await discovery.startListening();
  }

  /// Stop scanning.
  void stopDiscovery() {
    discovery.stopListening();
  }

  /// Connect to a discovered host.
  Future<bool> connectToHost(DiscoveredHost host) async {
    _selectedHost = host;
    notifyListeners();

    final connected = await stream.connect(
      ipAddress: host.ipAddress,
      connectionCode: host.payload.connectionCode,
      deviceName: deviceName,
      preset: _currentPreset,
    );

    if (connected) {
      _fightSequenceTriggered = true;
    }

    notifyListeners();
    return connected;
  }

  /// Connect with manual code entry.
  Future<bool> connectManual(String ipAddress, String code) async {
    if (code.length != 4) return false;

    final codeDigits = code.split('').map((c) => int.tryParse(c) ?? 0).toList();

    final connected = await stream.connect(
      ipAddress: ipAddress,
      connectionCode: codeDigits,
      deviceName: deviceName,
      preset: _currentPreset,
    );

    if (connected) {
      _fightSequenceTriggered = true;
      _selectedHost = null; // Manual connection
    }

    notifyListeners();
    return connected;
  }

  /// Disconnect from current host.
  void disconnect() {
    _koSequenceTriggered = true;
    stream.disconnect();
    audio.stop();
    _selectedHost = null;
    notifyListeners();
  }

  /// Change translation preset.
  void setPreset(PresetID preset) {
    _currentPreset = preset;
    if (isConnected) {
      stream.requestPresetChange(preset);
    }
    notifyListeners();
  }

  void _onSubStateChanged() {
    notifyListeners();
  }

  @override
  void dispose() {
    discovery.removeListener(_onSubStateChanged);
    stream.removeListener(_onSubStateChanged);
    audio.removeListener(_onSubStateChanged);
    discovery.dispose();
    stream.dispose();
    audio.dispose();
    super.dispose();
  }
}
