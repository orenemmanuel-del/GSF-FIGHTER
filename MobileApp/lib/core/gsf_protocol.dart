/// GSF FIGHTER - Shared Protocol (Dart mirror of GSFProtocol.h)
library;

import 'dart:typed_data';
import 'dart:convert';

// --- Ports ---
const int kDiscoveryPort = 47700;
const int kStreamPort = 47701;
const int kControlPort = 47702;
const int kProtocolMagic = 0x47534646; // "GSFF"
const int kProtocolVersion = 1;
const int kMaxClients = 8;
const int kJitterBufferMs = 30;

// --- Packet Types ---
enum PacketType {
  discovery(0x01),
  connectRequest(0x02),
  connectAccept(0x03),
  connectReject(0x04),
  audioData(0x10),
  audioConfig(0x11),
  presetChange(0x20),
  presetSync(0x21),
  heartbeat(0x30),
  disconnect(0xFF);

  final int value;
  const PacketType(this.value);

  static PacketType fromValue(int v) {
    return PacketType.values.firstWhere(
      (e) => e.value == v,
      orElse: () => PacketType.disconnect,
    );
  }
}

// --- Preset IDs ---
enum PresetID {
  flat(0, 'FLAT (NO FILTER)', 'BYPASS'),
  iPhoneSpeaker(1, 'GSF iPHONE FIGHTER', 'RYU'),
  airPodsPro(2, 'GSF AIRPODS FIGHTER', 'CHUN-LI'),
  voiture(3, 'GSF VOITURE FIGHTER', 'GUILE'),
  clubSystem(4, 'GSF CLUB FIGHTER', 'AKUMA'),
  cheapEarbuds(5, 'GSF CHEAP FIGHTER', 'BLANKA'),
  studioMonitors(6, 'GSF STUDIO FIGHTER', 'SAGAT');

  final int value;
  final String displayName;
  final String fighterName;
  const PresetID(this.value, this.displayName, this.fighterName);

  static PresetID fromValue(int v) {
    return PresetID.values.firstWhere(
      (e) => e.value == v,
      orElse: () => PresetID.flat,
    );
  }
}

// --- Packet Header (12 bytes) ---
class PacketHeader {
  final int magic;
  final int version;
  final PacketType type;
  final int payloadSize;
  final int sequenceNum;

  const PacketHeader({
    required this.magic,
    required this.version,
    required this.type,
    required this.payloadSize,
    required this.sequenceNum,
  });

  static const int size = 12;

  static PacketHeader? fromBytes(Uint8List data) {
    if (data.length < size) return null;
    final view = ByteData.sublistView(data);
    final magic = view.getUint32(0, Endian.little);
    if (magic != kProtocolMagic) return null;
    return PacketHeader(
      magic: magic,
      version: view.getUint8(4),
      type: PacketType.fromValue(view.getUint8(5)),
      payloadSize: view.getUint16(6, Endian.little),
      sequenceNum: view.getUint32(8, Endian.little),
    );
  }

  Uint8List toBytes() {
    final data = Uint8List(size);
    final view = ByteData.sublistView(data);
    view.setUint32(0, magic, Endian.little);
    view.setUint8(4, version);
    view.setUint8(5, type.value);
    view.setUint16(6, payloadSize, Endian.little);
    view.setUint32(8, sequenceNum, Endian.little);
    return data;
  }
}

// --- Discovery Payload ---
class DiscoveryPayload {
  final String hostName;
  final String dawName;
  final int sampleRate;
  final int bitDepth;
  final int numChannels;
  final List<int> connectionCode; // 4 digits
  final int currentClients;
  final int maxClients;

  const DiscoveryPayload({
    required this.hostName,
    required this.dawName,
    required this.sampleRate,
    required this.bitDepth,
    required this.numChannels,
    required this.connectionCode,
    required this.currentClients,
    required this.maxClients,
  });

  static const int size = 64 + 32 + 4 + 2 + 2 + 4 + 1 + 1; // 110 bytes

  static DiscoveryPayload? fromBytes(Uint8List data) {
    if (data.length < size) return null;

    final hostName = _readString(data, 0, 64);
    final dawName = _readString(data, 64, 32);
    final view = ByteData.sublistView(data);

    return DiscoveryPayload(
      hostName: hostName,
      dawName: dawName,
      sampleRate: view.getUint32(96, Endian.little),
      bitDepth: view.getUint16(100, Endian.little),
      numChannels: view.getUint16(102, Endian.little),
      connectionCode: [data[104], data[105], data[106], data[107]],
      currentClients: data[108],
      maxClients: data[109],
    );
  }

  String get codeString =>
      connectionCode.map((d) => d.toString()).join();

  String get displayInfo =>
      '$dawName on $hostName ($sampleRate Hz / $bitDepth-bit)';

  static String _readString(Uint8List data, int offset, int maxLen) {
    int end = offset;
    while (end < offset + maxLen && data[end] != 0) {
      end++;
    }
    return utf8.decode(data.sublist(offset, end), allowMalformed: true);
  }
}

// --- Audio Config Payload ---
class AudioConfigPayload {
  final int sampleRate;
  final int bitDepth;
  final int numChannels;
  final int samplesPerBlock;

  const AudioConfigPayload({
    required this.sampleRate,
    required this.bitDepth,
    required this.numChannels,
    required this.samplesPerBlock,
  });

  static const int size = 8;

  static AudioConfigPayload? fromBytes(Uint8List data) {
    if (data.length < size) return null;
    final view = ByteData.sublistView(data);
    return AudioConfigPayload(
      sampleRate: view.getUint32(0, Endian.little),
      bitDepth: view.getUint16(4, Endian.little),
      numChannels: view.getUint16(6, Endian.little),
      samplesPerBlock: data.length >= 10
          ? view.getUint16(8, Endian.little)
          : 512,
    );
  }
}

// --- Audio Data Payload Header ---
class AudioDataHeader {
  final int timestampUs;
  final int numSamples;
  final int numChannels;
  final PresetID activePreset;

  const AudioDataHeader({
    required this.timestampUs,
    required this.numSamples,
    required this.numChannels,
    required this.activePreset,
  });

  static const int size = 16;

  static AudioDataHeader? fromBytes(Uint8List data) {
    if (data.length < size) return null;
    final view = ByteData.sublistView(data);
    return AudioDataHeader(
      timestampUs: view.getUint64(0, Endian.little),
      numSamples: view.getUint16(8, Endian.little),
      numChannels: view.getUint16(10, Endian.little),
      activePreset: PresetID.fromValue(data[12]),
    );
  }
}

// --- Connect Request Payload ---
class ConnectRequestPayload {
  final String deviceName;
  final List<int> connectionCode;
  final PresetID requestedPreset;

  const ConnectRequestPayload({
    required this.deviceName,
    required this.connectionCode,
    required this.requestedPreset,
  });

  Uint8List toBytes() {
    final data = Uint8List(69); // 64 + 4 + 1
    final nameBytes = utf8.encode(deviceName);
    for (int i = 0; i < nameBytes.length && i < 64; i++) {
      data[i] = nameBytes[i];
    }
    data[64] = connectionCode[0];
    data[65] = connectionCode[1];
    data[66] = connectionCode[2];
    data[67] = connectionCode[3];
    data[68] = requestedPreset.value;
    return data;
  }
}

// --- 24-bit PCM conversion ---
double float_from24bit(int b0, int b1, int b2) {
  int intSample = b0 | (b1 << 8) | (b2 << 16);
  if (intSample & 0x800000 != 0) {
    intSample |= 0xFF000000; // Sign extend
  }
  // Convert to signed int32
  intSample = intSample.toSigned(32);
  return intSample / 8388607.0;
}
