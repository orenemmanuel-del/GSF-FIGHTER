/// GSF FIGHTER — Settings / Preset Screen (Oscilloscope Design)
/// System status + audio processing (buffer, preset), persistence, about.

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../../core/app_state.dart';
import '../../core/gsf_protocol.dart';
import '../theme/gsf_theme.dart';

class PresetSelectScreen extends StatefulWidget {
  const PresetSelectScreen({super.key});

  @override
  State<PresetSelectScreen> createState() => _PresetSelectScreenState();
}

class _PresetSelectScreenState extends State<PresetSelectScreen> {
  int _bufferIndex = 1; // 0=LOW, 1=STANDARD, 2=HIGH
  bool _stayAwake = true;

  static const _bufferLabels = ['LOW', 'STANDARD', 'HIGH'];
  static const _bufferNotes = [
    'Lowest latency — may cause dropouts on weaker networks.',
    'Balanced latency and stability. Recommended for most setups.',
    'Maximum stability — higher latency on the phone side.',
  ];

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: GSFColors.backgroundDeep,
      body: SafeArea(
        child: Consumer<GSFAppState>(
          builder: (context, appState, _) {
            return Column(
              children: [
                _buildAppBar(),
                Expanded(
                  child: ListView(
                    padding: const EdgeInsets.fromLTRB(20, 24, 20, 32),
                    children: [
                      _buildSystemStatus(),
                      const SizedBox(height: 24),
                      _buildAudioProcessing(),
                      const SizedBox(height: 24),
                      _buildPresetSelector(appState),
                      const SizedBox(height: 24),
                      _buildPersistence(),
                      const SizedBox(height: 24),
                      _buildAbout(),
                    ],
                  ),
                ),
              ],
            );
          },
        ),
      ),
    );
  }

  Widget _buildAppBar() {
    return Container(
      height: 60,
      padding: const EdgeInsets.symmetric(horizontal: 12),
      decoration: const BoxDecoration(
        color: GSFColors.backgroundPanel,
        border: Border(
          bottom: BorderSide(color: GSFColors.borderSubtle, width: 1),
        ),
      ),
      child: Row(
        children: [
          IconButton(
            icon: const Icon(Icons.arrow_back, color: GSFColors.accentCyan),
            onPressed: () => Navigator.of(context).pop(),
          ),
          const SizedBox(width: 4),
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
          const Spacer(),
          const Text(
            'SETTINGS',
            style: TextStyle(
              fontFamily: kOscMonoFont,
              color: GSFColors.textDim,
              fontSize: 10,
              letterSpacing: 2,
            ),
          ),
        ],
      ),
    );
  }

  // --- SECTIONS ---

  Widget _sectionLabel(String label) {
    return Padding(
      padding: const EdgeInsets.only(bottom: 10, left: 2),
      child: Text(
        label,
        style: const TextStyle(
          fontFamily: kOscMonoFont,
          color: GSFColors.textDim,
          fontSize: 9,
          fontWeight: FontWeight.w600,
          letterSpacing: 2,
        ),
      ),
    );
  }

  Widget _card({required Widget child}) {
    return Container(
      padding: const EdgeInsets.all(16),
      decoration: BoxDecoration(
        color: GSFColors.backgroundCard,
        borderRadius: BorderRadius.zero,
        border: Border.all(color: GSFColors.borderSubtle, width: 1),
      ),
      child: child,
    );
  }

  Widget _buildSystemStatus() {
    return _card(
      child: Row(
        children: [
          Container(
            width: 40,
            height: 40,
            decoration: BoxDecoration(
              color: GSFColors.accentCyan.withValues(alpha: 0.12),
              borderRadius: BorderRadius.zero,
              border: Border.all(color: GSFColors.accentCyan, width: 1),
            ),
            child: const Icon(Icons.settings,
                color: GSFColors.accentCyan, size: 20),
          ),
          const SizedBox(width: 14),
          Expanded(
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: const [
                Text(
                  'ENGINE ALPHA',
                  style: TextStyle(
                    fontFamily: kOscMonoFont,
                    color: GSFColors.textPrimary,
                    fontSize: 13,
                    fontWeight: FontWeight.w600,
                    letterSpacing: 1.5,
                  ),
                ),
                SizedBox(height: 3),
                Text(
                  '48KHZ / 24-BIT / ACTIVE',
                  style: TextStyle(
                    fontFamily: kOscMonoFont,
                    color: GSFColors.accentCyan,
                    fontSize: 9,
                    letterSpacing: 1.5,
                  ),
                ),
              ],
            ),
          ),
          const Text(
            'V 2.4.0',
            style: TextStyle(
              fontFamily: kOscMonoFont,
              color: GSFColors.textDim,
              fontSize: 9,
              letterSpacing: 1.5,
            ),
          ),
        ],
      ),
    );
  }

  Widget _buildAudioProcessing() {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        _sectionLabel('AUDIO PROCESSING'),
        _card(
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Row(
                children: [
                  const Expanded(
                    child: Text(
                      'Buffer Size',
                      style: TextStyle(
                        fontFamily: kOscMonoFont,
                        color: GSFColors.textPrimary,
                        fontSize: 13,
                        fontWeight: FontWeight.w600,
                      ),
                    ),
                  ),
                  Text(
                    _bufferIndex == 0
                        ? '32 samples'
                        : _bufferIndex == 1
                            ? '64 samples'
                            : '128 samples',
                    style: const TextStyle(
                      fontFamily: kOscMonoFont,
                      color: GSFColors.accentCyan,
                      fontSize: 11,
                    ),
                  ),
                ],
              ),
              const SizedBox(height: 12),
              Row(
                children: List.generate(3, (i) {
                  final selected = _bufferIndex == i;
                  return Expanded(
                    child: Padding(
                      padding: EdgeInsets.only(right: i < 2 ? 8 : 0),
                      child: GestureDetector(
                        onTap: () => setState(() => _bufferIndex = i),
                        child: Container(
                          height: 36,
                          alignment: Alignment.center,
                          decoration: BoxDecoration(
                            color: selected
                                ? GSFColors.accentCyan
                                : GSFColors.backgroundCard,
                            borderRadius: BorderRadius.zero,
                            border: Border.all(
                              color: selected
                                  ? GSFColors.accentCyan
                                  : GSFColors.borderSubtle,
                              width: 1,
                            ),
                          ),
                          child: Text(
                            _bufferLabels[i],
                            style: TextStyle(
                              fontFamily: kOscMonoFont,
                              color: selected
                                  ? GSFColors.backgroundDeep
                                  : GSFColors.textDim,
                              fontSize: 10,
                              fontWeight: FontWeight.w600,
                              letterSpacing: 1.5,
                            ),
                          ),
                        ),
                      ),
                    ),
                  );
                }),
              ),
              const SizedBox(height: 10),
              Text(
                _bufferNotes[_bufferIndex],
                style: const TextStyle(
                  fontFamily: kOscMonoFont,
                  color: GSFColors.textDim,
                  fontSize: 9,
                  fontStyle: FontStyle.italic,
                  height: 1.5,
                ),
              ),
            ],
          ),
        ),
      ],
    );
  }

  Widget _buildPresetSelector(GSFAppState appState) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        _sectionLabel('TRANSLATION PRESET'),
        _card(
          child: Column(
            children: PresetID.values.map((preset) {
              final selected = appState.currentPreset == preset;
              return GestureDetector(
                onTap: () {
                  appState.setPreset(preset);
                  setState(() {});
                },
                child: Container(
                  padding: const EdgeInsets.symmetric(vertical: 12),
                  decoration: BoxDecoration(
                    border: preset == PresetID.values.last
                        ? null
                        : const Border(
                            bottom: BorderSide(
                              color: GSFColors.borderSubtle,
                              width: 1,
                            ),
                          ),
                  ),
                  child: Row(
                    children: [
                      Container(
                        width: 10,
                        height: 10,
                        decoration: BoxDecoration(
                          shape: BoxShape.circle,
                          color: selected
                              ? GSFColors.accentCyan
                              : Colors.transparent,
                          border: Border.all(
                            color: selected
                                ? GSFColors.accentCyan
                                : GSFColors.textDim,
                            width: 1.5,
                          ),
                        ),
                      ),
                      const SizedBox(width: 12),
                      Expanded(
                        child: Text(
                          preset.displayName,
                          style: TextStyle(
                            fontFamily: kOscMonoFont,
                            color: selected
                                ? GSFColors.textPrimary
                                : GSFColors.textSecondary,
                            fontSize: 12,
                            fontWeight: selected
                                ? FontWeight.w700
                                : FontWeight.w400,
                          ),
                        ),
                      ),
                      if (selected)
                        Container(
                          padding: const EdgeInsets.symmetric(
                            horizontal: 6,
                            vertical: 2,
                          ),
                          decoration: BoxDecoration(
                            color: GSFColors.accentCyan,
                            borderRadius: BorderRadius.zero,
                          ),
                          child: const Text(
                            'ACTIVE',
                            style: TextStyle(
                              fontFamily: kOscMonoFont,
                              color: GSFColors.backgroundDeep,
                              fontSize: 8,
                              fontWeight: FontWeight.w600,
                              letterSpacing: 1.5,
                            ),
                          ),
                        ),
                    ],
                  ),
                ),
              );
            }).toList(),
          ),
        ),
      ],
    );
  }

  Widget _buildPersistence() {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        _sectionLabel('SYSTEM PERSISTENCE'),
        _card(
          child: Row(
            children: [
              const Expanded(
                child: Text(
                  'Stay Awake',
                  style: TextStyle(
                    fontFamily: kOscMonoFont,
                    color: GSFColors.textPrimary,
                    fontSize: 13,
                    fontWeight: FontWeight.w600,
                  ),
                ),
              ),
              Switch(
                value: _stayAwake,
                onChanged: (v) => setState(() => _stayAwake = v),
                activeThumbColor: GSFColors.accentCyan,
                activeTrackColor: GSFColors.accentCyan.withValues(alpha: 0.3),
                inactiveThumbColor: GSFColors.textDim,
                inactiveTrackColor: GSFColors.backgroundHover,
              ),
            ],
          ),
        ),
      ],
    );
  }

  Widget _buildAbout() {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        _sectionLabel('ABOUT OSCILLOSCOPE'),
        _card(
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              const Text(
                'Handcrafted in Berlin',
                style: TextStyle(
                  fontFamily: kOscMonoFont,
                  color: GSFColors.textPrimary,
                  fontSize: 13,
                  fontWeight: FontWeight.w600,
                ),
              ),
              const SizedBox(height: 6),
              const Text(
                'A precision monitoring tool for master-bus translation checks on mobile devices.',
                style: TextStyle(
                  fontFamily: kOscMonoFont,
                  color: GSFColors.textDim,
                  fontSize: 11,
                  height: 1.55,
                ),
              ),
              const SizedBox(height: 16),
              Row(
                children: [
                  Expanded(child: _aboutButton('LICENSES', Icons.description_outlined)),
                  const SizedBox(width: 10),
                  Expanded(child: _aboutButton('SUPPORT', Icons.help_outline)),
                ],
              ),
            ],
          ),
        ),
      ],
    );
  }

  Widget _aboutButton(String label, IconData icon) {
    return Container(
      height: 40,
      alignment: Alignment.center,
      decoration: BoxDecoration(
        color: GSFColors.backgroundCard,
        borderRadius: BorderRadius.zero,
        border: Border.all(color: GSFColors.borderSubtle, width: 1),
      ),
      child: Row(
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          Icon(icon, color: GSFColors.textSecondary, size: 14),
          const SizedBox(width: 8),
          Text(
            label,
            style: const TextStyle(
              fontFamily: kOscMonoFont,
              color: GSFColors.textSecondary,
              fontSize: 10,
              fontWeight: FontWeight.w600,
              letterSpacing: 1.5,
            ),
          ),
        ],
      ),
    );
  }
}
