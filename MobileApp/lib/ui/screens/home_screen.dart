/// GSF FIGHTER — Connection Screen (Oscilloscope Design)
/// Searches for the plugin on WiFi and lists detected workstations.

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../../core/app_state.dart';
import '../../network/discovery_client.dart';
import '../theme/gsf_theme.dart';
import '../widgets/arcade_button.dart';
import '../widgets/connection_code_input.dart';
import 'listening_screen.dart';

class HomeScreen extends StatefulWidget {
  const HomeScreen({super.key});

  @override
  State<HomeScreen> createState() => _HomeScreenState();
}

class _HomeScreenState extends State<HomeScreen> with TickerProviderStateMixin {
  late AnimationController _pulseController;
  late AnimationController _dotsController;
  bool _showManualConnect = false;
  final TextEditingController _ipController = TextEditingController();

  @override
  void initState() {
    super.initState();
    _pulseController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 1500),
    )..repeat(reverse: true);
    _dotsController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 1200),
    )..repeat();

    WidgetsBinding.instance.addPostFrameCallback((_) {
      context.read<GSFAppState>().startDiscovery();
    });
  }

  @override
  void dispose() {
    _pulseController.dispose();
    _dotsController.dispose();
    _ipController.dispose();
    super.dispose();
  }

  void _connectToHost(DiscoveredHost host) async {
    final appState = context.read<GSFAppState>();
    final connected = await appState.connectToHost(host);
    if (connected && mounted) {
      Navigator.of(context).push(
        MaterialPageRoute(builder: (_) => const ListeningScreen()),
      );
    }
  }

  void _connectManual(String code) async {
    final ip = _ipController.text.trim();
    if (ip.isEmpty || code.length != 4) return;
    final appState = context.read<GSFAppState>();
    final connected = await appState.connectManual(ip, code);
    if (connected && mounted) {
      Navigator.of(context).push(
        MaterialPageRoute(builder: (_) => const ListeningScreen()),
      );
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: GSFColors.backgroundDeep,
      body: SafeArea(
        child: Consumer<GSFAppState>(
          builder: (context, appState, _) {
            return Column(
              children: [
                _OscAppBar(
                  title: 'OSCILLOSCOPE',
                  subtitle: appState.isConnected
                      ? 'SYNCED: ${appState.selectedHost?.displayName ?? "—"}'
                      : 'LOOKING FOR PHONECHECK ON WIFI…',
                  connected: appState.isConnected,
                ),
                Expanded(
                  child: ListView(
                    padding: const EdgeInsets.fromLTRB(20, 24, 20, 20),
                    children: [
                      _buildSearchCard(),
                      const SizedBox(height: 28),
                      _buildDetectedHeader(appState.availableHosts.length),
                      const SizedBox(height: 12),
                      ..._buildHostList(appState),
                      if (_showManualConnect) ...[
                        const SizedBox(height: 24),
                        _buildManualConnect(),
                      ],
                      const SizedBox(height: 24),
                      ArcadeButton(
                        text: _showManualConnect
                            ? 'AUTO DETECT'
                            : 'ENTER CODE MANUALLY',
                        style: ArcadeButtonStyle.shoryuken,
                        onPressed: () {
                          setState(() => _showManualConnect = !_showManualConnect);
                        },
                      ),
                    ],
                  ),
                ),
                _buildStatusBar(),
              ],
            );
          },
        ),
      ),
    );
  }

  Widget _buildSearchCard() {
    return Container(
      padding: const EdgeInsets.symmetric(horizontal: 24, vertical: 28),
      decoration: BoxDecoration(
        color: GSFColors.backgroundCard,
        borderRadius: BorderRadius.zero,
        border: Border.all(color: GSFColors.borderSubtle, width: 1),
      ),
      child: Column(
        children: [
          AnimatedBuilder(
            animation: _pulseController,
            builder: (context, _) {
              return Icon(
                Icons.wifi_tethering,
                size: 56,
                color: GSFColors.accentCyan.withValues(
                  alpha: 0.45 + 0.55 * _pulseController.value,
                ),
              );
            },
          ),
          const SizedBox(height: 18),
          const Text(
            'LOOKING FOR PHONECHECK ON WIFI…',
            textAlign: TextAlign.center,
            style: TextStyle(
              fontFamily: kOscMonoFont,
              color: GSFColors.textPrimary,
              fontSize: 14,
              fontWeight: FontWeight.w600,
              letterSpacing: 0.16,
            ),
          ),
          const SizedBox(height: 8),
          Text(
            '◉ NETWORK: ${_networkLabel()}',
            style: const TextStyle(
              fontFamily: kOscMonoFont,
              color: GSFColors.textSecondary,
              fontSize: 10,
            ),
          ),
        ],
      ),
    );
  }

  String _networkLabel() {
    // The real SSID isn't exposed without platform-specific code; show a placeholder.
    return 'STUDIO_MAIN_5GHZ';
  }

  Widget _buildDetectedHeader(int count) {
    return Row(
      children: [
        const Text(
          'DETECTED WORKSTATIONS',
          style: TextStyle(
            fontFamily: kOscMonoFont,
            color: GSFColors.textDim,
            fontSize: 9,
            fontWeight: FontWeight.w600,
            letterSpacing: 2,
          ),
        ),
        const Spacer(),
        Container(
          padding: const EdgeInsets.symmetric(horizontal: 8, vertical: 3),
          decoration: BoxDecoration(
            color: GSFColors.accentCyan.withValues(alpha: 0.15),
            borderRadius: BorderRadius.zero,
            border: Border.all(color: GSFColors.accentCyan, width: 1),
          ),
          child: Text(
            '${count.toString().padLeft(2, "0")} FOUND',
            style: const TextStyle(
              fontFamily: kOscMonoFont,
              color: GSFColors.accentCyan,
              fontSize: 9,
              fontWeight: FontWeight.w600,
              letterSpacing: 0.32,
            ),
          ),
        ),
      ],
    );
  }

  List<Widget> _buildHostList(GSFAppState appState) {
    final hosts = appState.availableHosts;
    if (hosts.isEmpty) {
      return [
        Container(
          padding: const EdgeInsets.all(18),
          decoration: BoxDecoration(
            color: GSFColors.backgroundCard,
            borderRadius: BorderRadius.zero,
            border: Border.all(color: GSFColors.borderSubtle, width: 1),
          ),
          child: const Text(
            'No workstations found yet. Make sure the plugin is inserted on your master bus and the host is on the same WiFi.',
            style: TextStyle(
              fontFamily: kOscMonoFont,
              color: GSFColors.textSecondary,
              fontSize: 11,
              height: 1.5,
            ),
          ),
        ),
      ];
    }
    return hosts.map(_buildHostItem).toList();
  }

  Widget _buildHostItem(DiscoveredHost host) {
    final ready = host.hasSlots;
    return Container(
      margin: const EdgeInsets.only(bottom: 10),
      decoration: BoxDecoration(
        color: GSFColors.backgroundCard,
        borderRadius: BorderRadius.zero,
        border: Border.all(color: GSFColors.borderSubtle, width: 1),
      ),
      child: Material(
        color: Colors.transparent,
        child: InkWell(
          borderRadius: BorderRadius.zero,
          onTap: ready ? () => _connectToHost(host) : null,
          child: Padding(
            padding: const EdgeInsets.all(14),
            child: Row(
              children: [
                const Icon(
                  Icons.desktop_windows_outlined,
                  color: GSFColors.textDim,
                  size: 22,
                ),
                const SizedBox(width: 14),
                Expanded(
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Text(
                        host.displayName.toUpperCase(),
                        style: const TextStyle(
                          fontFamily: kOscMonoFont,
                          color: GSFColors.textPrimary,
                          fontSize: 13,
                          fontWeight: FontWeight.w600,
                          letterSpacing: 0.16,
                        ),
                      ),
                      const SizedBox(height: 2),
                      Text(
                        host.ipAddress,
                        style: const TextStyle(
                          fontFamily: kOscMonoFont,
                          color: GSFColors.textDim,
                          fontSize: 10,
                        ),
                      ),
                    ],
                  ),
                ),
                _statusBadge(ready),
                const SizedBox(width: 8),
                const Icon(
                  Icons.chevron_right,
                  color: GSFColors.textDim,
                  size: 18,
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }

  Widget _statusBadge(bool ready) {
    return Container(
      padding: const EdgeInsets.symmetric(horizontal: 8, vertical: 3),
      decoration: BoxDecoration(
        color: ready ? GSFColors.accentCyan : GSFColors.backgroundHover,
        borderRadius: BorderRadius.zero,
      ),
      child: Text(
        ready ? 'READY' : 'WAITING',
        style: TextStyle(
          fontFamily: kOscMonoFont,
          color: ready ? GSFColors.backgroundDeep : GSFColors.textDim,
          fontSize: 9,
          fontWeight: FontWeight.w600,
          letterSpacing: 0.32,
        ),
      ),
    );
  }

  Widget _buildManualConnect() {
    return Container(
      padding: const EdgeInsets.all(18),
      decoration: BoxDecoration(
        color: GSFColors.backgroundCard,
        borderRadius: BorderRadius.zero,
        border: Border.all(color: GSFColors.borderSubtle, width: 1),
      ),
      child: Column(
        children: [
          const Text(
            'ENTER CONNECTION CODE',
            style: TextStyle(
              fontFamily: kOscMonoFont,
              color: GSFColors.textDim,
              fontSize: 9,
              fontWeight: FontWeight.w600,
              letterSpacing: 2,
            ),
          ),
          const SizedBox(height: 14),
          TextField(
            controller: _ipController,
            style: const TextStyle(
              fontFamily: kOscMonoFont,
              color: GSFColors.textPrimary,
              fontSize: 14,
            ),
            keyboardType: const TextInputType.numberWithOptions(decimal: true),
            decoration: InputDecoration(
              hintText: '192.168.1.XXX',
              hintStyle: const TextStyle(
                fontFamily: kOscMonoFont,
                color: GSFColors.textDim,
                fontSize: 13,
              ),
              filled: true,
              fillColor: GSFColors.backgroundDeep,
              contentPadding: const EdgeInsets.symmetric(horizontal: 14, vertical: 12),
              border: OutlineInputBorder(
                borderRadius: BorderRadius.zero,
                borderSide: const BorderSide(color: GSFColors.borderSubtle),
              ),
              enabledBorder: OutlineInputBorder(
                borderRadius: BorderRadius.zero,
                borderSide: const BorderSide(color: GSFColors.borderSubtle),
              ),
              focusedBorder: OutlineInputBorder(
                borderRadius: BorderRadius.zero,
                borderSide: const BorderSide(color: GSFColors.accentCyan),
              ),
              prefixText: 'IP: ',
              prefixStyle: const TextStyle(
                fontFamily: kOscMonoFont,
                color: GSFColors.textDim,
              ),
            ),
          ),
          const SizedBox(height: 16),
          ConnectionCodeInput(onCodeEntered: _connectManual),
        ],
      ),
    );
  }

  Widget _buildStatusBar() {
    return AnimatedBuilder(
      animation: _dotsController,
      builder: (context, _) {
        final nDots = (_dotsController.value * 4).floor() % 4;
        return Container(
          height: 28,
          alignment: Alignment.center,
          decoration: const BoxDecoration(
            color: GSFColors.backgroundPanel,
            border: Border(
              top: BorderSide(color: GSFColors.borderSubtle, width: 1),
            ),
          ),
          child: Text(
            'BROADCASTING SIGNAL${"." * nDots}',
            style: const TextStyle(
              fontFamily: kOscMonoFont,
              color: GSFColors.textDim,
              fontSize: 9,
              fontWeight: FontWeight.w600,
              letterSpacing: 2,
            ),
          ),
        );
      },
    );
  }
}

/// Shared Oscilloscope AppBar (title + subtitle + wifi).
class _OscAppBar extends StatelessWidget {
  final String title;
  final String subtitle;
  final bool connected;

  const _OscAppBar({
    required this.title,
    required this.subtitle,
    this.connected = false,
  });

  @override
  Widget build(BuildContext context) {
    return Container(
      height: 60,
      padding: const EdgeInsets.symmetric(horizontal: 20),
      decoration: const BoxDecoration(
        color: GSFColors.backgroundPanel,
        border: Border(
          bottom: BorderSide(color: GSFColors.borderSubtle, width: 1),
        ),
      ),
      child: Row(
        children: [
          // "|||" icon
          Container(
            width: 16,
            margin: const EdgeInsets.only(right: 10),
            child: Row(
              crossAxisAlignment: CrossAxisAlignment.center,
              children: [
                _iconBar(),
                const SizedBox(width: 2),
                _iconBar(),
                const SizedBox(width: 2),
                _iconBar(),
              ],
            ),
          ),
          Expanded(
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Text(
                  title,
                  style: const TextStyle(
                    fontFamily: kOscMonoFont,
                    color: GSFColors.accentCyan,
                    fontSize: 13,
                    fontWeight: FontWeight.w600,
                    letterSpacing: 3,
                  ),
                ),
                const SizedBox(height: 2),
                Text(
                  subtitle,
                  overflow: TextOverflow.ellipsis,
                  style: const TextStyle(
                    fontFamily: kOscMonoFont,
                    color: GSFColors.textDim,
                    fontSize: 10,
                    letterSpacing: 1,
                  ),
                ),
              ],
            ),
          ),
          Icon(
            connected ? Icons.wifi : Icons.wifi_tethering,
            color: connected ? GSFColors.accentCyan : GSFColors.textDim,
            size: 20,
          ),
        ],
      ),
    );
  }

  Widget _iconBar() {
    return Container(
      width: 3,
      height: 16,
      color: GSFColors.accentCyan,
    );
  }
}
