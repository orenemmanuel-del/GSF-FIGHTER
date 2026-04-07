/// GSF FIGHTER - Listening Screen
/// Active streaming view: meters, preset selector, stop button.
/// Full arcade aesthetic with fight overlay on connect.

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:wakelock_plus/wakelock_plus.dart';
import '../../core/app_state.dart';
import '../../core/gsf_protocol.dart';
import '../../network/stream_client.dart' as net;
import '../theme/gsf_theme.dart';
import '../widgets/arcade_button.dart';
import '../widgets/health_bar.dart';
import '../widgets/scanline_overlay.dart';
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
  late AnimationController _glowController;
  bool _showFightOverlay = true;

  @override
  void initState() {
    super.initState();
    _meterController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 50),
    )..repeat();

    _glowController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 2000),
    )..repeat(reverse: true);

    // Keep screen on while listening
    WakelockPlus.enable();

    // Auto-dismiss fight overlay
    Future.delayed(const Duration(milliseconds: 2500), () {
      if (mounted) {
        setState(() => _showFightOverlay = false);
      }
    });
  }

  @override
  void dispose() {
    _meterController.dispose();
    _glowController.dispose();
    WakelockPlus.disable();
    super.dispose();
  }

  void _disconnect() {
    context.read<GSFAppState>().disconnect();
    Navigator.of(context).pop();
  }

  void _openPresetSelect() {
    Navigator.of(context).push(
      MaterialPageRoute(builder: (_) => const PresetSelectScreen()),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Stack(
        children: [
          // Background
          Container(decoration: const BoxDecoration(gradient: GSFColors.backgroundGradient)),

          // Main content
          SafeArea(
            child: Consumer<GSFAppState>(
              builder: (context, appState, _) {
                return Column(
                  children: [
                    const SizedBox(height: 20),

                    // Header
                    _buildHeader(appState),
                    const SizedBox(height: 24),

                    // Level meters
                    _buildMeters(appState),
                    const SizedBox(height: 24),

                    // Active preset
                    _buildActivePreset(appState),
                    const SizedBox(height: 16),

                    // Status info
                    _buildStatus(appState),

                    const Spacer(),

                    // Preset select button
                    Padding(
                      padding: const EdgeInsets.symmetric(horizontal: 24),
                      child: ArcadeButton(
                        text: 'SELECT FIGHTER',
                        style: ArcadeButtonStyle.shoryuken,
                        height: 50,
                        onPressed: _openPresetSelect,
                      ),
                    ),
                    const SizedBox(height: 12),

                    // Stop button
                    Padding(
                      padding: const EdgeInsets.symmetric(horizontal: 24),
                      child: ArcadeButton(
                        text: 'STOP LISTENING',
                        style: ArcadeButtonStyle.ko,
                        isActive: true,
                        height: 56,
                        onPressed: _disconnect,
                      ),
                    ),
                    const SizedBox(height: 32),
                  ],
                );
              },
            ),
          ),

          // Scanlines
          const ScanlineOverlay(),

          // Fight overlay
          if (_showFightOverlay) const FightOverlay(),
        ],
      ),
    );
  }

  Widget _buildHeader(GSFAppState appState) {
    final hostName = appState.selectedHost?.displayName ?? 'GSF FIGHTER';

    return AnimatedBuilder(
      animation: _glowController,
      builder: (context, _) {
        return Column(
          children: [
            // Connection status dot
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                Container(
                  width: 10,
                  height: 10,
                  decoration: BoxDecoration(
                    color: appState.isStreaming
                        ? GSFColors.green
                        : GSFColors.yellow,
                    shape: BoxShape.circle,
                    boxShadow: [
                      BoxShadow(
                        color: (appState.isStreaming
                                ? GSFColors.green
                                : GSFColors.yellow)
                            .withValues(
                              alpha: 0.3 + 0.3 * _glowController.value,
                            ),
                        blurRadius: 12,
                        spreadRadius: 2,
                      ),
                    ],
                  ),
                ),
                const SizedBox(width: 10),
                Text(
                  appState.isStreaming ? 'STREAMING' : 'CONNECTED',
                  style: TextStyle(
                    color: appState.isStreaming
                        ? GSFColors.green
                        : GSFColors.yellow,
                    fontSize: 14,
                    fontWeight: FontWeight.w800,
                    letterSpacing: 3,
                  ),
                ),
              ],
            ),
            const SizedBox(height: 8),

            // Host name
            Text(
              hostName,
              style: const TextStyle(
                color: GSFColors.white,
                fontSize: 16,
                fontWeight: FontWeight.w700,
              ),
            ),
          ],
        );
      },
    );
  }

  Widget _buildMeters(GSFAppState appState) {
    return AnimatedBuilder(
      animation: _meterController,
      builder: (context, _) {
        return Padding(
          padding: const EdgeInsets.symmetric(horizontal: 24),
          child: Column(
            children: [
              HealthBar(
                value: appState.audio.peakL,
                label: 'L',
                height: 28,
              ),
              const SizedBox(height: 8),
              HealthBar(
                value: appState.audio.peakR,
                label: 'R',
                reversed: true,
                height: 28,
              ),
            ],
          ),
        );
      },
    );
  }

  Widget _buildActivePreset(GSFAppState appState) {
    final preset = appState.currentPreset;

    return GestureDetector(
      onTap: _openPresetSelect,
      child: Container(
        margin: const EdgeInsets.symmetric(horizontal: 24),
        padding: const EdgeInsets.all(16),
        decoration: BoxDecoration(
          color: GSFColors.darkGrey,
          borderRadius: BorderRadius.circular(10),
          border: Border.all(
            color: _presetColor(preset),
            width: 1.5,
          ),
          boxShadow: [
            BoxShadow(
              color: _presetColor(preset).withValues(alpha: 0.2),
              blurRadius: 12,
              spreadRadius: 1,
            ),
          ],
        ),
        child: Row(
          children: [
            // Fighter icon
            Container(
              width: 44,
              height: 44,
              decoration: BoxDecoration(
                color: _presetColor(preset).withValues(alpha: 0.2),
                borderRadius: BorderRadius.circular(8),
                border: Border.all(color: _presetColor(preset), width: 1),
              ),
              alignment: Alignment.center,
              child: Text(
                preset.fighterName.isNotEmpty
                    ? preset.fighterName[0]
                    : 'X',
                style: TextStyle(
                  color: _presetColor(preset),
                  fontSize: 22,
                  fontWeight: FontWeight.w900,
                ),
              ),
            ),
            const SizedBox(width: 14),

            // Preset info
            Expanded(
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text(
                    preset.fighterName,
                    style: TextStyle(
                      color: _presetColor(preset),
                      fontSize: 16,
                      fontWeight: FontWeight.w800,
                      letterSpacing: 2,
                    ),
                  ),
                  const SizedBox(height: 4),
                  Text(
                    preset.displayName,
                    style: const TextStyle(
                      color: GSFColors.textDim,
                      fontSize: 11,
                    ),
                  ),
                ],
              ),
            ),

            const Icon(
              Icons.chevron_right,
              color: GSFColors.textDim,
            ),
          ],
        ),
      ),
    );
  }

  Widget _buildStatus(GSFAppState appState) {
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 24, vertical: 8),
      child: Row(
        mainAxisAlignment: MainAxisAlignment.spaceAround,
        children: [
          _statusItem(
            'PACKETS',
            '${appState.audio.totalPackets}',
            GSFColors.blue,
          ),
          _statusItem(
            'BUFFER',
            '${(appState.audio.bufferFill * 100).toInt()}%',
            appState.audio.bufferFill > 0.5
                ? GSFColors.green
                : GSFColors.orange,
          ),
          _statusItem(
            'DROPS',
            '${appState.audio.underrunCount}',
            appState.audio.underrunCount > 0
                ? GSFColors.red
                : GSFColors.green,
          ),
        ],
      ),
    );
  }

  Widget _statusItem(String label, String value, Color color) {
    return Column(
      children: [
        Text(
          label,
          style: const TextStyle(
            color: GSFColors.textDim,
            fontSize: 9,
            fontWeight: FontWeight.w700,
            letterSpacing: 1,
          ),
        ),
        const SizedBox(height: 4),
        Text(
          value,
          style: TextStyle(
            color: color,
            fontSize: 18,
            fontWeight: FontWeight.w800,
          ),
        ),
      ],
    );
  }

  Color _presetColor(PresetID preset) {
    switch (preset) {
      case PresetID.flat:
        return GSFColors.lightGrey;
      case PresetID.iPhoneSpeaker:
        return GSFColors.blue;
      case PresetID.airPodsPro:
        return GSFColors.purple;
      case PresetID.voiture:
        return GSFColors.green;
      case PresetID.clubSystem:
        return GSFColors.red;
      case PresetID.cheapEarbuds:
        return GSFColors.yellow;
      case PresetID.studioMonitors:
        return GSFColors.orange;
    }
  }
}
