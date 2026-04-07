/// GSF FIGHTER - Arcade Button Widget
/// Big chunky buttons styled like Street Fighter arcade pads.

import 'package:flutter/material.dart';
import '../theme/gsf_theme.dart';

enum ArcadeButtonStyle {
  hadouken, // Red - primary action
  shoryuken, // Blue - secondary action
  tatsumaki, // Yellow - tertiary
  ko, // Red pulse - danger
  round, // Circular pad
}

class ArcadeButton extends StatefulWidget {
  final String text;
  final ArcadeButtonStyle style;
  final VoidCallback? onPressed;
  final bool isActive;
  final double? width;
  final double? height;

  const ArcadeButton({
    super.key,
    required this.text,
    this.style = ArcadeButtonStyle.hadouken,
    this.onPressed,
    this.isActive = false,
    this.width,
    this.height,
  });

  @override
  State<ArcadeButton> createState() => _ArcadeButtonState();
}

class _ArcadeButtonState extends State<ArcadeButton>
    with SingleTickerProviderStateMixin {
  late AnimationController _glowController;
  bool _isPressed = false;

  @override
  void initState() {
    super.initState();
    _glowController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 1500),
    )..repeat(reverse: true);
  }

  @override
  void dispose() {
    _glowController.dispose();
    super.dispose();
  }

  Color get _baseColor {
    switch (widget.style) {
      case ArcadeButtonStyle.hadouken:
        return GSFColors.red;
      case ArcadeButtonStyle.shoryuken:
        return GSFColors.blue;
      case ArcadeButtonStyle.tatsumaki:
        return GSFColors.yellow;
      case ArcadeButtonStyle.ko:
        return GSFColors.red;
      case ArcadeButtonStyle.round:
        return GSFColors.midGrey;
    }
  }

  Color get _glowColor {
    switch (widget.style) {
      case ArcadeButtonStyle.hadouken:
        return GSFColors.redGlow;
      case ArcadeButtonStyle.shoryuken:
        return GSFColors.blueGlow;
      case ArcadeButtonStyle.tatsumaki:
        return GSFColors.yellowGlow;
      case ArcadeButtonStyle.ko:
        return GSFColors.redGlow;
      case ArcadeButtonStyle.round:
        return GSFColors.blueGlow;
    }
  }

  @override
  Widget build(BuildContext context) {
    return AnimatedBuilder(
      animation: _glowController,
      builder: (context, child) {
        final glowIntensity = widget.isActive
            ? 0.3 + 0.2 * _glowController.value
            : 0.0;

        return GestureDetector(
          onTapDown: (_) => setState(() => _isPressed = true),
          onTapUp: (_) {
            setState(() => _isPressed = false);
            widget.onPressed?.call();
          },
          onTapCancel: () => setState(() => _isPressed = false),
          child: AnimatedContainer(
            duration: const Duration(milliseconds: 80),
            width: widget.width,
            height: widget.height ?? 56,
            transform: Matrix4.translationValues(
              0,
              _isPressed ? 2 : 0,
              0,
            ),
            decoration: BoxDecoration(
              gradient: LinearGradient(
                begin: Alignment.topCenter,
                end: Alignment.bottomCenter,
                colors: [
                  _baseColor.withValues(alpha: _isPressed ? 0.7 : 1.0),
                  _baseColor
                      .withValues(
                        red: _baseColor.r * 0.7,
                        green: _baseColor.g * 0.7,
                        blue: _baseColor.b * 0.7,
                      ),
                ],
              ),
              borderRadius: widget.style == ArcadeButtonStyle.round
                  ? BorderRadius.circular(100)
                  : BorderRadius.circular(8),
              border: Border.all(
                color: widget.isActive
                    ? GSFColors.yellow
                    : _baseColor.withValues(alpha: 0.7),
                width: widget.isActive ? 2.5 : 1.5,
              ),
              boxShadow: [
                // Drop shadow
                BoxShadow(
                  color: Colors.black.withValues(alpha: 0.5),
                  offset: Offset(0, _isPressed ? 1 : 3),
                  blurRadius: _isPressed ? 2 : 6,
                ),
                // Glow
                if (widget.isActive || glowIntensity > 0)
                  BoxShadow(
                    color: _glowColor.withValues(alpha: glowIntensity),
                    blurRadius: 16,
                    spreadRadius: 2,
                  ),
              ],
            ),
            alignment: Alignment.center,
            child: Text(
              widget.text,
              style: TextStyle(
                color: widget.isActive ? GSFColors.yellow : GSFColors.white,
                fontSize: widget.style == ArcadeButtonStyle.round ? 11 : 15,
                fontWeight: FontWeight.w900,
                letterSpacing: 2,
              ),
              textAlign: TextAlign.center,
            ),
          ),
        );
      },
    );
  }
}
