import 'package:flutter/material.dart';
import '../theme/gsf_theme.dart';

enum ArcadeButtonStyle {
  hadouken,  // primary (blue filled)
  shoryuken, // secondary (gray filled)
  tatsumaki, // ghost (transparent with blue border)
  ko,        // danger (red)
  round,     // round icon button
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

class _ArcadeButtonState extends State<ArcadeButton> {
  bool _hovered = false;

  @override
  Widget build(BuildContext context) {
    final palette = _paletteFor(widget.style, widget.isActive, _hovered);

    return MouseRegion(
      onEnter: (_) => setState(() => _hovered = true),
      onExit: (_) => setState(() => _hovered = false),
      cursor: widget.onPressed != null
          ? SystemMouseCursors.click
          : SystemMouseCursors.basic,
      child: GestureDetector(
        onTap: widget.onPressed,
        child: Container(
          width: widget.width,
          height: widget.height ?? 48,
          alignment: Alignment.center,
          decoration: BoxDecoration(
            color: palette.fill,
            borderRadius: widget.style == ArcadeButtonStyle.round
                ? BorderRadius.circular(100)
                : BorderRadius.zero,
            border: Border.all(color: palette.border, width: 1),
          ),
          child: Text(
            widget.text,
            style: TextStyle(
              color: palette.text,
              fontSize: 14,
              fontWeight: FontWeight.w600,
              letterSpacing: 0.16,
            ),
            textAlign: TextAlign.center,
          ),
        ),
      ),
    );
  }

  _ButtonPalette _paletteFor(ArcadeButtonStyle style, bool active, bool hover) {
    switch (style) {
      case ArcadeButtonStyle.hadouken:
        return _ButtonPalette(
          fill: hover ? const Color(0xFF0353E9) : GSFColors.borderAccent,
          border: Colors.transparent,
          text: Colors.white,
        );
      case ArcadeButtonStyle.shoryuken:
        return _ButtonPalette(
          fill: hover ? GSFColors.backgroundHover : GSFColors.backgroundCard,
          border: Colors.transparent,
          text: GSFColors.textPrimary,
        );
      case ArcadeButtonStyle.tatsumaki:
        return _ButtonPalette(
          fill: hover ? const Color(0x1978A9FF) : Colors.transparent,
          border: GSFColors.accentCyan,
          text: GSFColors.accentCyan,
        );
      case ArcadeButtonStyle.ko:
        return _ButtonPalette(
          fill: hover ? const Color(0xFFB81921) : GSFColors.accentRed,
          border: Colors.transparent,
          text: Colors.white,
        );
      case ArcadeButtonStyle.round:
        return _ButtonPalette(
          fill: active ? GSFColors.accentCyan : GSFColors.backgroundCard,
          border: active ? GSFColors.accentCyan : GSFColors.borderSubtle,
          text: active ? GSFColors.backgroundDeep : GSFColors.textSecondary,
        );
    }
  }
}

class _ButtonPalette {
  final Color fill;
  final Color border;
  final Color text;
  _ButtonPalette({required this.fill, required this.border, required this.text});
}
