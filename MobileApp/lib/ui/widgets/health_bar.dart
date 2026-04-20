import 'package:flutter/material.dart';
import '../theme/gsf_theme.dart';

class HealthBar extends StatelessWidget {
  final double value;
  final String label;
  final bool reversed;
  final double height;

  const HealthBar({
    super.key,
    required this.value,
    this.label = 'L',
    this.reversed = false,
    this.height = 28,
  });

  @override
  Widget build(BuildContext context) {
    final clamped = value.clamp(0.0, 1.0);
    final percent = (clamped * 100).toInt();

    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Row(
          mainAxisAlignment: MainAxisAlignment.spaceBetween,
          children: [
            Text(
              '${label.toUpperCase()} CHANNEL',
              style: const TextStyle(
                color: GSFColors.textSecondary,
                fontSize: 12,
                fontWeight: FontWeight.w400,
                letterSpacing: 0.32,
              ),
            ),
            Text(
              '$percent%',
              style: TextStyle(
                fontFamily: kOscMonoFont,
                color: GSFColors.accentCyan,
                fontSize: 12,
                fontWeight: FontWeight.w400,
              ),
            ),
          ],
        ),
        const SizedBox(height: 8),
        SizedBox(
          height: height,
          child: LayoutBuilder(
            builder: (context, constraints) {
              return CustomPaint(
                size: Size(constraints.maxWidth, constraints.maxHeight),
                painter: _VuMeterPainter(value: clamped),
              );
            },
          ),
        ),
      ],
    );
  }
}

class _VuMeterPainter extends CustomPainter {
  final double value;

  _VuMeterPainter({required this.value});

  @override
  void paint(Canvas canvas, Size size) {
    // Background track
    canvas.drawRect(
      Rect.fromLTWH(0, 0, size.width, size.height),
      Paint()..color = GSFColors.backgroundDeep,
    );

    // Border
    canvas.drawRect(
      Rect.fromLTWH(0, 0, size.width, size.height),
      Paint()
        ..color = GSFColors.borderSubtle
        ..style = PaintingStyle.stroke
        ..strokeWidth = 1,
    );

    final fillW = value * size.width;
    if (fillW <= 0) return;

    const clipThreshold = 0.9;
    final clipX = clipThreshold * size.width;

    // Blue body (0..90%)
    final blueW = fillW.clamp(0.0, clipX);
    if (blueW > 0) {
      canvas.drawRect(
        Rect.fromLTWH(0, 0, blueW, size.height),
        Paint()..color = GSFColors.accentCyan,
      );
    }

    // Red clip zone (90..100%)
    if (fillW > clipX) {
      final redW = fillW - clipX;
      canvas.drawRect(
        Rect.fromLTWH(clipX, 0, redW, size.height),
        Paint()..color = GSFColors.accentRed,
      );
    }

    // Clip threshold tick
    canvas.drawLine(
      Offset(clipX, 0),
      Offset(clipX, size.height),
      Paint()
        ..color = GSFColors.accentRed.withValues(alpha: 0.5)
        ..strokeWidth = 1,
    );
  }

  @override
  bool shouldRepaint(_VuMeterPainter old) => old.value != value;
}
