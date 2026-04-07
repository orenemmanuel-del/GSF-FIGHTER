/// GSF FIGHTER - Discovery Client
/// Listens for UDP broadcast from the plugin to auto-detect on local network.

import 'dart:async';
import 'dart:io';
import 'dart:typed_data';
import 'package:flutter/foundation.dart';
import '../core/gsf_protocol.dart';

/// Represents a discovered GSF FIGHTER plugin instance.
class DiscoveredHost {
  final String ipAddress;
  final DiscoveryPayload payload;
  DateTime lastSeen;

  DiscoveredHost({
    required this.ipAddress,
    required this.payload,
    required this.lastSeen,
  });

  String get displayName => payload.dawName.isNotEmpty
      ? '${payload.dawName} — ${payload.hostName}'
      : payload.hostName;

  String get connectionCode => payload.codeString;
  bool get hasSlots => payload.currentClients < payload.maxClients;

  bool get isStale =>
      DateTime.now().difference(lastSeen).inSeconds > 5;
}

class DiscoveryClient extends ChangeNotifier {
  RawDatagramSocket? _socket;
  final Map<String, DiscoveredHost> _hosts = {};
  Timer? _cleanupTimer;
  bool _listening = false;

  List<DiscoveredHost> get hosts =>
      _hosts.values.where((h) => !h.isStale).toList()
        ..sort((a, b) => b.lastSeen.compareTo(a.lastSeen));

  bool get isListening => _listening;

  /// Start listening for discovery broadcasts.
  Future<void> startListening() async {
    if (_listening) return;

    try {
      _socket = await RawDatagramSocket.bind(
        InternetAddress.anyIPv4,
        kDiscoveryPort,
        reuseAddress: true,
        reusePort: true,
      );

      _socket!.broadcastEnabled = true;
      _listening = true;

      _socket!.listen((event) {
        if (event == RawSocketEvent.read) {
          final datagram = _socket!.receive();
          if (datagram != null) {
            _handleDatagram(datagram);
          }
        }
      });

      // Cleanup stale hosts every 3 seconds
      _cleanupTimer = Timer.periodic(
        const Duration(seconds: 3),
        (_) => _cleanupStaleHosts(),
      );

      notifyListeners();
      debugPrint('GSF Discovery: Listening on port $kDiscoveryPort');
    } catch (e) {
      debugPrint('GSF Discovery: Failed to bind — $e');
      _listening = false;
    }
  }

  /// Stop listening.
  void stopListening() {
    _cleanupTimer?.cancel();
    _cleanupTimer = null;
    _socket?.close();
    _socket = null;
    _listening = false;
    notifyListeners();
  }

  void _handleDatagram(Datagram datagram) {
    final data = Uint8List.fromList(datagram.data);

    // Parse header
    final header = PacketHeader.fromBytes(data);
    if (header == null || header.type != PacketType.discovery) return;

    // Parse payload
    if (data.length < PacketHeader.size + DiscoveryPayload.size) return;
    final payloadData = data.sublist(PacketHeader.size);
    final payload = DiscoveryPayload.fromBytes(Uint8List.fromList(payloadData));
    if (payload == null) return;

    final ip = datagram.address.address;
    final existing = _hosts[ip];

    if (existing != null) {
      existing.lastSeen = DateTime.now();
      // Update payload in case something changed (client count, etc.)
      _hosts[ip] = DiscoveredHost(
        ipAddress: ip,
        payload: payload,
        lastSeen: DateTime.now(),
      );
    } else {
      _hosts[ip] = DiscoveredHost(
        ipAddress: ip,
        payload: payload,
        lastSeen: DateTime.now(),
      );
      debugPrint('GSF Discovery: Found ${payload.dawName} at $ip');
    }

    notifyListeners();
  }

  void _cleanupStaleHosts() {
    final before = _hosts.length;
    _hosts.removeWhere((_, host) => host.isStale);
    if (_hosts.length != before) {
      notifyListeners();
    }
  }

  @override
  void dispose() {
    stopListening();
    super.dispose();
  }
}
