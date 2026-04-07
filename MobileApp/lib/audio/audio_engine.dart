/// GSF FIGHTER - Audio Engine
/// Manages real-time audio playback from the network stream.
/// Uses platform audio channels for low-latency output.

import 'dart:async';
import 'dart:typed_data';
import 'dart:collection';
import 'package:flutter/foundation.dart';
import '../core/gsf_protocol.dart';

/// Jitter buffer that accumulates audio packets before playback
/// to smooth out network timing variations.
class JitterBuffer {
  final int targetSizeMs;
  final Queue<AudioPacket> _packets = Queue();
  int _sampleRate = 48000;
  bool _primed = false;

  JitterBuffer({this.targetSizeMs = kJitterBufferMs});

  void configure(int sampleRate) {
    _sampleRate = sampleRate;
    _primed = false;
    _packets.clear();
  }

  int get _targetSamples => (_sampleRate * targetSizeMs) ~/ 1000;

  int get bufferedSamples {
    int total = 0;
    for (final pkt in _packets) {
      total += pkt.numSamples;
    }
    return total;
  }

  bool get isReady => _primed && _packets.isNotEmpty;

  double get bufferFillRatio {
    if (_targetSamples == 0) return 0;
    return bufferedSamples / _targetSamples;
  }

  void push(AudioPacket packet) {
    _packets.addLast(packet);

    // Prime: wait until we have enough buffered
    if (!_primed && bufferedSamples >= _targetSamples) {
      _primed = true;
    }

    // Overflow protection: drop oldest if too much buffered
    while (bufferedSamples > _targetSamples * 4) {
      _packets.removeFirst();
    }
  }

  AudioPacket? pop() {
    if (!_primed || _packets.isEmpty) return null;
    return _packets.removeFirst();
  }

  void clear() {
    _packets.clear();
    _primed = false;
  }
}

class AudioPacket {
  final Float64List left;
  final Float64List right;
  final int numSamples;
  final int timestampUs;

  const AudioPacket({
    required this.left,
    required this.right,
    required this.numSamples,
    this.timestampUs = 0,
  });
}

/// Audio engine state.
enum AudioEngineState {
  idle,
  buffering,
  playing,
  underrun,
}

class AudioEngine extends ChangeNotifier {
  AudioEngineState _state = AudioEngineState.idle;
  final JitterBuffer _jitterBuffer = JitterBuffer();

  int _sampleRate = 48000;
  int _bitDepth = 24;
  int _numChannels = 2;
  int _underrunCount = 0;
  double _peakL = 0.0;
  double _peakR = 0.0;

  // Stats
  int _totalPackets = 0;

  AudioEngineState get state => _state;
  int get sampleRate => _sampleRate;
  int get underrunCount => _underrunCount;
  double get peakL => _peakL;
  double get peakR => _peakR;
  double get bufferFill => _jitterBuffer.bufferFillRatio;
  int get totalPackets => _totalPackets;

  /// Configure audio engine with stream parameters.
  void configure(AudioConfigPayload config) {
    _sampleRate = config.sampleRate;
    _bitDepth = config.bitDepth;
    _numChannels = config.numChannels;
    _jitterBuffer.configure(_sampleRate);
    _state = AudioEngineState.buffering;
    _underrunCount = 0;
    _totalPackets = 0;
    notifyListeners();
    debugPrint(
      'GSF Audio: Configured $_sampleRate Hz / $_bitDepth-bit / $_numChannels ch',
    );
  }

  /// Push decoded audio data into the jitter buffer.
  void pushAudio(Float64List left, Float64List right, int numSamples) {
    _totalPackets++;

    // Update peak meters
    double pL = 0, pR = 0;
    for (int i = 0; i < numSamples; i++) {
      final absL = left[i].abs();
      final absR = right[i].abs();
      if (absL > pL) pL = absL;
      if (absR > pR) pR = absR;
    }
    _peakL = pL;
    _peakR = pR;

    _jitterBuffer.push(AudioPacket(
      left: left,
      right: right,
      numSamples: numSamples,
    ));

    if (_state == AudioEngineState.buffering && _jitterBuffer.isReady) {
      _state = AudioEngineState.playing;
      notifyListeners();
    }
  }

  /// Pull next audio packet for playback.
  /// Returns null if buffer underrun.
  AudioPacket? pullAudio() {
    final packet = _jitterBuffer.pop();

    if (packet == null && _state == AudioEngineState.playing) {
      _state = AudioEngineState.underrun;
      _underrunCount++;
      notifyListeners();
    } else if (packet != null && _state == AudioEngineState.underrun) {
      _state = AudioEngineState.playing;
      notifyListeners();
    }

    return packet;
  }

  /// Stop and reset.
  void stop() {
    _jitterBuffer.clear();
    _state = AudioEngineState.idle;
    _peakL = 0;
    _peakR = 0;
    notifyListeners();
  }

  @override
  void dispose() {
    stop();
    super.dispose();
  }
}
