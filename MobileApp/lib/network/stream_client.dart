/// GSF FIGHTER - Stream Client
/// Connects to the plugin via TCP and receives audio stream packets.

import 'dart:async';
import 'dart:io';
import 'dart:typed_data';
import 'package:flutter/foundation.dart';
import '../core/gsf_protocol.dart';

enum ConnectionState {
  disconnected,
  connecting,
  connected,
  streaming,
  error,
}

class StreamClient extends ChangeNotifier {
  Socket? _socket;
  ConnectionState _state = ConnectionState.disconnected;
  AudioConfigPayload? _audioConfig;
  PresetID _activePreset = PresetID.flat;
  String _errorMessage = '';
  int _packetsReceived = 0;
  DateTime? _connectedAt;

  // Audio data callback
  void Function(Float64List left, Float64List right, int numSamples)?
      onAudioData;

  // State callback
  void Function(ConnectionState state)? onStateChanged;

  ConnectionState get state => _state;
  AudioConfigPayload? get audioConfig => _audioConfig;
  PresetID get activePreset => _activePreset;
  String get errorMessage => _errorMessage;
  int get packetsReceived => _packetsReceived;
  Duration get connectedDuration => _connectedAt != null
      ? DateTime.now().difference(_connectedAt!)
      : Duration.zero;

  bool get isConnected =>
      _state == ConnectionState.connected ||
      _state == ConnectionState.streaming;

  // Receive buffer for TCP stream reassembly
  final BytesBuilder _receiveBuffer = BytesBuilder(copy: false);

  /// Connect to a GSF FIGHTER plugin.
  Future<bool> connect({
    required String ipAddress,
    required List<int> connectionCode,
    required String deviceName,
    PresetID preset = PresetID.flat,
  }) async {
    if (isConnected) {
      disconnect();
    }

    _setState(ConnectionState.connecting);

    try {
      _socket = await Socket.connect(
        ipAddress,
        kStreamPort,
        timeout: const Duration(seconds: 5),
      );

      // Send connect request
      final requestPayload = ConnectRequestPayload(
        deviceName: deviceName,
        connectionCode: connectionCode,
        requestedPreset: preset,
      );

      final header = PacketHeader(
        magic: kProtocolMagic,
        version: kProtocolVersion,
        type: PacketType.connectRequest,
        payloadSize: 69, // ConnectRequestPayload size
        sequenceNum: 0,
      );

      _socket!.add(header.toBytes());
      _socket!.add(requestPayload.toBytes());
      await _socket!.flush();

      // Listen for response and audio stream
      _socket!.listen(
        _onData,
        onError: _onError,
        onDone: _onDone,
        cancelOnError: false,
      );

      // Wait briefly for accept/reject
      await Future.delayed(const Duration(milliseconds: 500));

      if (_state == ConnectionState.connecting) {
        // Assume connected if no rejection received
        _connectedAt = DateTime.now();
        _setState(ConnectionState.connected);
      }

      return isConnected;
    } catch (e) {
      _errorMessage = e.toString();
      _setState(ConnectionState.error);
      debugPrint('GSF Stream: Connection failed — $e');
      return false;
    }
  }

  /// Disconnect from the plugin.
  void disconnect() {
    if (_socket != null) {
      // Send disconnect packet
      try {
        final header = PacketHeader(
          magic: kProtocolMagic,
          version: kProtocolVersion,
          type: PacketType.disconnect,
          payloadSize: 0,
          sequenceNum: 0,
        );
        _socket!.add(header.toBytes());
      } catch (_) {}

      _socket?.close();
      _socket = null;
    }

    _receiveBuffer.clear();
    _connectedAt = null;
    _packetsReceived = 0;
    _setState(ConnectionState.disconnected);
  }

  /// Request a preset change.
  void requestPresetChange(PresetID preset) {
    if (!isConnected || _socket == null) return;

    _activePreset = preset;
    final header = PacketHeader(
      magic: kProtocolMagic,
      version: kProtocolVersion,
      type: PacketType.presetChange,
      payloadSize: 1,
      sequenceNum: 0,
    );

    _socket!.add(header.toBytes());
    _socket!.add(Uint8List.fromList([preset.value]));
    notifyListeners();
  }

  void _onData(Uint8List data) {
    _receiveBuffer.add(data);
    _processBuffer();
  }

  void _processBuffer() {
    final buffer = _receiveBuffer.takeBytes();
    int offset = 0;

    while (offset + PacketHeader.size <= buffer.length) {
      final headerData = Uint8List.fromList(
        buffer.sublist(offset, offset + PacketHeader.size),
      );
      final header = PacketHeader.fromBytes(headerData);
      if (header == null) {
        // Invalid data, skip one byte
        offset++;
        continue;
      }

      final totalPacketSize = PacketHeader.size + header.payloadSize;
      if (offset + totalPacketSize > buffer.length) {
        // Incomplete packet, put remaining data back in buffer
        _receiveBuffer.add(buffer.sublist(offset));
        return;
      }

      // Extract payload
      final payload = Uint8List.fromList(
        buffer.sublist(
          offset + PacketHeader.size,
          offset + totalPacketSize,
        ),
      );

      _handlePacket(header, payload);
      offset += totalPacketSize;
    }

    // Put any remaining bytes back
    if (offset < buffer.length) {
      _receiveBuffer.add(buffer.sublist(offset));
    }
  }

  void _handlePacket(PacketHeader header, Uint8List payload) {
    switch (header.type) {
      case PacketType.connectAccept:
        _audioConfig = AudioConfigPayload.fromBytes(payload);
        _connectedAt = DateTime.now();
        _setState(ConnectionState.connected);
        debugPrint(
          'GSF Stream: Connected — '
          '${_audioConfig?.sampleRate}Hz / '
          '${_audioConfig?.bitDepth}-bit',
        );

      case PacketType.connectReject:
        _errorMessage = 'Connection rejected (wrong code or server full)';
        _setState(ConnectionState.error);
        _socket?.close();
        _socket = null;

      case PacketType.audioData:
        _handleAudioData(payload);

      case PacketType.presetSync:
        if (payload.isNotEmpty) {
          _activePreset = PresetID.fromValue(payload[0]);
          notifyListeners();
        }

      case PacketType.heartbeat:
        // Keep-alive acknowledged
        break;

      case PacketType.disconnect:
        debugPrint('GSF Stream: Server disconnected');
        disconnect();

      default:
        break;
    }
  }

  void _handleAudioData(Uint8List payload) {
    final audioHeader = AudioDataHeader.fromBytes(payload);
    if (audioHeader == null) return;

    _packetsReceived++;
    if (_state != ConnectionState.streaming) {
      _setState(ConnectionState.streaming);
    }

    _activePreset = audioHeader.activePreset;

    final numSamples = audioHeader.numSamples;
    final numChannels = audioHeader.numChannels;
    final pcmOffset = AudioDataHeader.size;
    final expectedPcmSize = numSamples * numChannels * 3; // 24-bit = 3 bytes

    if (payload.length < pcmOffset + expectedPcmSize) return;

    // Decode 24-bit interleaved PCM to float arrays
    final left = Float64List(numSamples);
    final right = Float64List(numSamples);

    for (int i = 0; i < numSamples; i++) {
      final baseIdx = pcmOffset + i * numChannels * 3;

      // Left channel
      left[i] = float_from24bit(
        payload[baseIdx],
        payload[baseIdx + 1],
        payload[baseIdx + 2],
      );

      // Right channel
      if (numChannels >= 2) {
        right[i] = float_from24bit(
          payload[baseIdx + 3],
          payload[baseIdx + 4],
          payload[baseIdx + 5],
        );
      } else {
        right[i] = left[i];
      }
    }

    onAudioData?.call(left, right, numSamples);
  }

  void _onError(dynamic error) {
    debugPrint('GSF Stream: Socket error — $error');
    _errorMessage = error.toString();
    _setState(ConnectionState.error);
  }

  void _onDone() {
    debugPrint('GSF Stream: Socket closed');
    if (_state != ConnectionState.disconnected) {
      _setState(ConnectionState.disconnected);
    }
    _socket = null;
  }

  void _setState(ConnectionState newState) {
    if (_state == newState) return;
    _state = newState;
    onStateChanged?.call(newState);
    notifyListeners();
  }

  @override
  void dispose() {
    disconnect();
    super.dispose();
  }
}
