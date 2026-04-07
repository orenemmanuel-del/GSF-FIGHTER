/// GSF FIGHTER - Fight Overlay Animation
/// "READY?" → "FIGHT!" full-screen overlay when stream connects.

import 'package:flutter/material.dart';
import '../theme/gsf_theme.dart';

class FightOverlay extends StatefulWidget {
  final VoidCallback? onComplete;

  const FightOverlay({super.key, this.onComplete});

  @override
  State<FightOverlay> createState() => _FightOverlayState();
}

class _FightOverlayState extends State<FightOverlay>
    with TickerProviderStateMixin {
  late AnimationController _readyController;
  late AnimationController _fightController;
  late AnimationController _fadeController;
  _Phase _phase = _Phase.ready;

  @override
  void initState() {
    super.initState();

    // Phase 1: "READY?" (1 second)
    _readyController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 1000),
    );

    // Phase 2: "FIGHT!" (0.8 seconds)
    _fightController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 800),
    );

    // Phase 3: Fade out
    _fadeController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 400),
    );

    _startSequence();
  }

  void _startSequence() async {
    // Phase 1: READY?
    setState(() => _phase = _Phase.ready);
    _readyController.forward();
    await Future.delayed(const Duration(milliseconds: 1200));

    // Phase 2: FIGHT!
    if (!mounted) return;
    setState(() => _phase = _Phase.fight);
    _fightController.forward();
    await Future.delayed(const Duration(milliseconds: 1000));

    // Phase 3: Fade out
    if (!mounted) return;
    setState(() => _phase = _Phase.fadeOut);
    _fadeController.forward();
    await Future.delayed(const Duration(milliseconds: 500));

    widget.onComplete?.call();
  }

  @override
  void dispose() {
    _readyController.dispose();
    _fightController.dispose();
    _fadeController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return AnimatedBuilder(
      animation: _fadeController,
      builder: (context, _) {
        final fadeOpacity = _phase == _Phase.fadeOut
            ? 1.0 - _fadeController.value
            : 1.0;

        return IgnorePointer(
          ignoring: _phase == _Phase.fadeOut,
          child: Opacity(
            opacity: fadeOpacity,
            child: Container(
              color: GSFColors.black.withValues(alpha: 0.85),
              alignment: Alignment.center,
              child: _buildText(),
            ),
          ),
        );
      },
    );
  }

  Widget _buildText() {
    switch (_phase) {
      case _Phase.ready:
        return AnimatedBuilder(
          animation: _readyController,
          builder: (context, _) {
            final scale = 1.0 + 2.0 * (1.0 - _readyController.value);
            final opacity = _readyController.value.clamp(0.0, 1.0);

            return Transform.scale(
              scale: scale,
              child: Opacity(
                opacity: opacity,
                child: Text(
                  'READY?',
                  style: TextStyle(
                    color: GSFColors.yellow,
                    fontSize: 56,
                    fontWeight: FontWeight.w900,
                    letterSpacing: 8,
                    shadows: [
                      Shadow(
                        color: GSFColors.yellow.withValues(alpha: 0.6),
                        blurRadius: 30,
                      ),
                      const Shadow(
                        color: Colors.black,
                        blurRadius: 4,
                        offset: Offset(3, 3),
                      ),
                    ],
                  ),
                ),
              ),
            );
          },
        );

      case _Phase.fight:
        return AnimatedBuilder(
          animation: _fightController,
          builder: (context, _) {
            final scale = 1.0 + 3.0 * (1.0 - _fightController.value);
            final opacity = _fightController.value.clamp(0.0, 1.0);

            return Transform.scale(
              scale: scale,
              child: Opacity(
                opacity: opacity,
                child: Text(
                  'FIGHT!',
                  style: TextStyle(
                    color: GSFColors.red,
                    fontSize: 64,
                    fontWeight: FontWeight.w900,
                    letterSpacing: 10,
                    shadows: [
                      Shadow(
                        color: GSFColors.red.withValues(alpha: 0.7),
                        blurRadius: 40,
                      ),
                      const Shadow(
                        color: Colors.black,
                        blurRadius: 6,
                        offset: Offset(4, 4),
                      ),
                    ],
                  ),
                ),
              ),
            );
          },
        );

      case _Phase.fadeOut:
        return Text(
          'FIGHT!',
          style: TextStyle(
            color: GSFColors.red,
            fontSize: 64,
            fontWeight: FontWeight.w900,
            letterSpacing: 10,
            shadows: [
              Shadow(
                color: GSFColors.red.withValues(alpha: 0.7),
                blurRadius: 40,
              ),
            ],
          ),
        );
    }
  }
}

enum _Phase { ready, fight, fadeOut }
