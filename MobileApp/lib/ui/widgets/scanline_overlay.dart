/// GSF FIGHTER - Scanline Overlay
/// CRT scanline effect over the entire screen for retro arcade feel.

import 'package:flutter/material.dart';

class ScanlineOverlay extends StatelessWidget {
  final double opacity;
  final int lineSpacing;

  const ScanlineOverlay({
    super.key,
    this.opacity = 0.03,
    this.lineSpacing = 3,
  });

  @override
  Widget build(BuildContext context) {
    return IgnorePointer(
      child: CustomPaint(
        size: MediaQuery.of(context).size,
        painter: _ScanlinePainter(
          opacity: opacity,
          lineSpacing: lineSpacing,
        ),
      ),
    );
  }
}

class _ScanlinePainter extends CustomPainter {
  final double opacity;
  final int lineSpacing;

  _ScanlinePainter({required this.opacity, required this.lineSpacing});

  @override
  void paint(Canvas canvas, Size size) {
    final paint = Paint()
      ..color = Colors.white.withValues(alpha: opacity)
      ..strokeWidth = 0.5;

    for (double y = 0; y < size.height; y += lineSpacing) {
      canvas.drawLine(Offset(0, y), Offset(size.width, y), paint);
    }
  }

  @override
  bool shouldRepaint(_ScanlinePainter old) =>
      old.opacity != opacity || old.lineSpacing != lineSpacing;
}
