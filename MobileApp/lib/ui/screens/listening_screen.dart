/// GSF FIGHTER — Listening Screen (Oscilloscope Design)
/// Active streaming view: VU meters, stats, stop button.

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:wakelock_plus/wakelock_plus.dart';
import '../../core/app_state.dart';
import '../theme/gsf_theme.dart';
import '../widgets/arcade_button.dart';
import '../widgets/health_bar.dart';
import '../animations/fight_overlay.dart';
import 'preset_select_screen.dart';

class ListeningScreen extends StatefulWidget {
  const ListeningScreen({super.key});

  @override
  State<ListeningScreen> createState() => _ListeningScreenState();
}

class _ListeningScreenState extends State<ListeningScreen>
    with TickerProviderStateMixin {
  late AnimationController _meterController;
  bool _showConnectOverlay = true;

  @override
  void initState() {
    super.initState();
    _meterController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 50),
    )..repeat();

    WakelockPlus.enable();

    Future.delayed(const Duration(milliseconds: 2000), () {
      if (mounted) setState(() => _showConnectOverlay = false);
    });
  }

  @override
  void dispose() {
    _meterController.dispose();
    WakelockPlus.disable();
    super.dispose();
  }

  void _disconnect() {
    context.read<GSFAppState>().disconnect();
    Navigator.of(context).pop();
  }

  void _openSettings() {
    Navigator.of(context).push(
      MaterialPageRoute(builder: (_) => const PresetSelectScreen()),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: GSFColors.backgroundDeep,
      body: SafeArea(
        child: Consumer<GSFAppState>(
          builder: (context, appState, _) {
            return Stack(
              children: [
                Column(
                  children: [
                    _OscAppBar(
                      subtitle: 'SYNCED: ${appState.selectedHost?.displayName.toUpperCase() ?? "MONITOR_01"}',
                      connected: appState.isConnected,
                    ),
                    Expanded(
                      child: ListView(
                        padding: const EdgeInsets.fromLTRB(24, 28, 24, 20),
                        children: [
                          _buildTitle(appState),
                          const SizedBox(height: 30),
                          AnimatedBuilder(
                            animation: _meterController,
                            builder: (context, _) {
                              return Column(
                                children: [
                                  HealthBar(
                                    value: appState.audio.peakL,
                                    label: 'LEFT',
                                  ),
                                  const SizedBox(height: 18),
                                  HealthBar(
                                    value: appState.audio.peakR,
                                    label: 'RIGHT',
                                  ),
                                ],
                              );
                            },
                          ),
                          const SizedBox(height: 30),
                          _buildStatsRow(appState),
                          const SizedBox(height: 28),
                          _buildWarning(),
                          const SizedBox(height: 24),
                          ArcadeButton(
                            text: 'SETTINGS',
                            style: ArcadeButtonStyle.tatsumaki,
                            onPressed: _openSettings,
                          ),
                          const SizedBox(height: 12),
                          ArcadeButton(
                            text: '■  STOP SESSION',
                            style: ArcadeButtonStyle.ko,
                            onPressed: _disconnect,
                          ),
                        ],
                      ),
                    ),
                  ],
                ),
                if (_showConnectOverlay)
                  FightOverlay(
                    onComplete: () {
                      if (mounted) setState(() => _showConnectOverlay = false);
                    },
                  ),
              ],
            );
          },
        ),
      ),
    );
  }

  Widget _buildTitle(GSFAppState appState) {
    final hostName = appState.selectedHost?.displayName.toUpperCase() ?? 'MONITOR_01';
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Text(
          hostName,
          style: const TextStyle(
            fontFamily: kOscMonoFont,
            color: GSFColors.textPrimary,
            fontSize: 24,
            fontWeight: FontWeight.w600,
            letterSpacing: 1.5,
          ),
        ),
        const SizedBox(height: 4),
        const Text(
          '48KHZ • 24-BIT PCM',
          style: TextStyle(
            fontFamily: kOscMonoFont,
            color: GSFColors.textDim,
            fontSize: 11,
            letterSpacing: 2,
          ),
        ),
      ],
    );
  }

  Widget _buildStatsRow(GSFAppState appState) {
    return Container(
      padding: const EdgeInsets.symmetric(horizontal: 4, vertical: 14),
      decoration: BoxDecoration(
        color: GSFColors.backgroundCard,
        borderRadius: BorderRadius.zero,
        border: Border.all(color: GSFColors.borderSubtle, width: 1),
      ),
      child: Row(
        children: [
          _statCell('PACKETS', '${appState.audio.totalPackets}'),
          _divider(),
          _statCell(
            'BUFFER',
            '${(appState.audio.bufferFill * 100).toInt()}%',
          ),
          _divider(),
          _statCell('DROPS', '${appState.audio.underrunCount}'),
        ],
      ),
    );
  }

  Widget _divider() => Container(
        width: 1,
        height: 28,
        color: GSFColors.borderSubtle,
      );

  Widget _statCell(String label, String value) {
    return Expanded(
      child: Column(
        children: [
          Text(
            label,
            style: const TextStyle(
              fontFamily: kOscMonoFont,
              color: GSFColors.textDim,
              fontSize: 9,
              fontWeight: FontWeight.w600,
              letterSpacing: 2,
            ),
          ),
          const SizedBox(height: 4),
          Text(
            value,
            style: const TextStyle(
              fontFamily: kOscMonoFont,
              color: GSFColors.textPrimary,
              fontSize: 14,
              fontWeight: FontWeight.w600,
            ),
          ),
        ],
      ),
    );
  }

  Widget _buildWarning() {
    return Container(
      padding: const EdgeInsets.symmetric(horizontal: 14, vertical: 10),
      decoration: BoxDecoration(
        color: GSFColors.accentRedDark,
        borderRadius: BorderRadius.zero,
        border: Border.all(color: GSFColors.accentRed, width: 1),
      ),
      child: const Row(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Icon(Icons.warning_amber_rounded,
              color: GSFColors.accentRed, size: 14),
          SizedBox(width: 8),
          Expanded(
            child: Text(
              'WARNING: AUDIO OUTPUT RESTRICTED TO BUILT-IN SPEAKERS ONLY. EXTERNAL HARDWARE BYPASSED.',
              style: TextStyle(
                fontFamily: kOscMonoFont,
                color: GSFColors.accentRed,
                fontSize: 9,
                height: 1.5,
                letterSpacing: 0.5,
              ),
            ),
          ),
        ],
      ),
    );
  }
}

class _OscAppBar extends StatelessWidget {
  final String subtitle;
  final bool connected;

  const _OscAppBar({required this.subtitle, this.connected = false});

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
          Container(
            width: 16,
            margin: const EdgeInsets.only(right: 10),
            child: Row(
              children: [
                _bar(), const SizedBox(width: 2),
                _bar(), const SizedBox(width: 2),
                _bar(),
              ],
            ),
          ),
          Expanded(
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                const Text(
                  'OSCILLOSCOPE',
                  style: TextStyle(
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

  Widget _bar() => Container(
        width: 3,
        height: 16,
        color: GSFColors.accentCyan,
      );
}
