/// GSF FIGHTER - Preset Select Screen
/// "SELECT YOUR FIGHTER" character grid with arcade styling.

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../../core/app_state.dart';
import '../../core/gsf_protocol.dart';
import '../theme/gsf_theme.dart';
import '../widgets/scanline_overlay.dart';

class PresetSelectScreen extends StatefulWidget {
  const PresetSelectScreen({super.key});

  @override
  State<PresetSelectScreen> createState() => _PresetSelectScreenState();
}

class _PresetSelectScreenState extends State<PresetSelectScreen>
    with SingleTickerProviderStateMixin {
  late AnimationController _titleController;

  @override
  void initState() {
    super.initState();
    _titleController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 600),
    )..forward();
  }

  @override
  void dispose() {
    _titleController.dispose();
    super.dispose();
  }

  static const _presetData = <PresetID, _PresetInfo>{
    PresetID.flat: _PresetInfo('BYPASS', 'X', GSFColors.lightGrey, 'No processing'),
    PresetID.iPhoneSpeaker: _PresetInfo('RYU', 'R', GSFColors.blue, 'iPhone Speaker simulation'),
    PresetID.airPodsPro: _PresetInfo('CHUN-LI', 'C', GSFColors.purple, 'AirPods Pro simulation'),
    PresetID.voiture: _PresetInfo('GUILE', 'G', GSFColors.green, 'Car system simulation'),
    PresetID.clubSystem: _PresetInfo('AKUMA', 'A', GSFColors.red, 'Club PA system simulation'),
    PresetID.cheapEarbuds: _PresetInfo('BLANKA', 'B', GSFColors.yellow, 'Cheap earbuds simulation'),
    PresetID.studioMonitors: _PresetInfo('SAGAT', 'S', GSFColors.orange, 'Studio monitors simulation'),
  };

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Stack(
        children: [
          Container(decoration: const BoxDecoration(gradient: GSFColors.backgroundGradient)),

          SafeArea(
            child: Consumer<GSFAppState>(
              builder: (context, appState, _) {
                return Column(
                  children: [
                    const SizedBox(height: 16),

                    // Back button
                    Align(
                      alignment: Alignment.centerLeft,
                      child: IconButton(
                        onPressed: () => Navigator.of(context).pop(),
                        icon: const Icon(Icons.arrow_back, color: GSFColors.yellow),
                      ),
                    ),

                    // Title
                    FadeTransition(
                      opacity: _titleController,
                      child: ScaleTransition(
                        scale: Tween<double>(begin: 1.5, end: 1.0).animate(
                          CurvedAnimation(
                            parent: _titleController,
                            curve: Curves.elasticOut,
                          ),
                        ),
                        child: Text(
                          'SELECT YOUR\nFIGHTER',
                          textAlign: TextAlign.center,
                          style: TextStyle(
                            color: GSFColors.yellow,
                            fontSize: 32,
                            fontWeight: FontWeight.w900,
                            letterSpacing: 4,
                            height: 1.2,
                            shadows: [
                              Shadow(
                                color: GSFColors.yellow.withValues(alpha: 0.5),
                                blurRadius: 20,
                              ),
                            ],
                          ),
                        ),
                      ),
                    ),

                    const SizedBox(height: 24),

                    // Fighter grid
                    Expanded(
                      child: GridView.count(
                        crossAxisCount: 2,
                        padding: const EdgeInsets.symmetric(horizontal: 16),
                        crossAxisSpacing: 12,
                        mainAxisSpacing: 12,
                        childAspectRatio: 1.1,
                        children: PresetID.values.map((preset) {
                          final info = _presetData[preset]!;
                          final isActive = appState.currentPreset == preset;
                          return _buildFighterCard(
                            preset, info, isActive, appState,
                          );
                        }).toList(),
                      ),
                    ),
                  ],
                );
              },
            ),
          ),

          const ScanlineOverlay(),
        ],
      ),
    );
  }

  Widget _buildFighterCard(
    PresetID preset,
    _PresetInfo info,
    bool isActive,
    GSFAppState appState,
  ) {
    return GestureDetector(
      onTap: () {
        appState.setPreset(preset);
        // Pop after brief delay for feedback
        Future.delayed(const Duration(milliseconds: 300), () {
          if (mounted) Navigator.of(context).pop();
        });
      },
      child: AnimatedContainer(
        duration: const Duration(milliseconds: 200),
        decoration: BoxDecoration(
          color: isActive
              ? info.color.withValues(alpha: 0.15)
              : GSFColors.darkGrey,
          borderRadius: BorderRadius.circular(12),
          border: Border.all(
            color: isActive ? info.color : GSFColors.lightGrey,
            width: isActive ? 2.5 : 1,
          ),
          boxShadow: [
            if (isActive)
              BoxShadow(
                color: info.color.withValues(alpha: 0.3),
                blurRadius: 16,
                spreadRadius: 2,
              ),
          ],
        ),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            // Fighter initial / silhouette
            Container(
              width: 56,
              height: 56,
              decoration: BoxDecoration(
                color: info.color.withValues(alpha: isActive ? 0.3 : 0.1),
                shape: BoxShape.circle,
                border: Border.all(
                  color: info.color.withValues(alpha: isActive ? 0.8 : 0.3),
                  width: 2,
                ),
              ),
              alignment: Alignment.center,
              child: Text(
                info.initial,
                style: TextStyle(
                  color: isActive ? info.color : info.color.withValues(alpha: 0.5),
                  fontSize: 26,
                  fontWeight: FontWeight.w900,
                ),
              ),
            ),
            const SizedBox(height: 10),

            // Fighter name
            Text(
              info.name,
              style: TextStyle(
                color: isActive ? info.color : GSFColors.white,
                fontSize: 14,
                fontWeight: FontWeight.w800,
                letterSpacing: 2,
              ),
            ),
            const SizedBox(height: 4),

            // Preset display name
            Text(
              preset.displayName,
              textAlign: TextAlign.center,
              style: TextStyle(
                color: isActive
                    ? GSFColors.white.withValues(alpha: 0.8)
                    : GSFColors.textDim,
                fontSize: 8,
              ),
            ),
            const SizedBox(height: 4),

            // Description
            Text(
              info.description,
              textAlign: TextAlign.center,
              style: const TextStyle(
                color: GSFColors.textDim,
                fontSize: 9,
              ),
            ),

            // Active indicator
            if (isActive) ...[
              const SizedBox(height: 6),
              Container(
                padding: const EdgeInsets.symmetric(horizontal: 8, vertical: 2),
                decoration: BoxDecoration(
                  color: info.color,
                  borderRadius: BorderRadius.circular(4),
                ),
                child: const Text(
                  'ACTIVE',
                  style: TextStyle(
                    color: GSFColors.black,
                    fontSize: 8,
                    fontWeight: FontWeight.w900,
                    letterSpacing: 2,
                  ),
                ),
              ),
            ],
          ],
        ),
      ),
    );
  }
}

class _PresetInfo {
  final String name;
  final String initial;
  final Color color;
  final String description;

  const _PresetInfo(this.name, this.initial, this.color, this.description);
}
