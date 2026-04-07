/// GSF FIGHTER - Health Bar Widget
/// Street Fighter style health/energy bar for audio levels.

import 'package:flutter/material.dart';
import '../theme/gsf_theme.dart';

class HealthBar extends StatelessWidget {
  final double value; // 0.0 to 1.0
  final String label;
  final bool reversed;
  final double height;

  const HealthBar({
    super.key,
    required this.value,
    this.label = 'P1',
    this.reversed = false,
    this.height = 24,
  });

  @override
  Widget build(BuildContext context) {
    return SizedBox(
      height: height,
      child: Row(
        children: [
          if (!reversed) _buildLabel(),
          Expanded(child: _buildBar()),
          if (reversed) _buildLabel(),
        ],
      ),
    );
  }

  Widget _buildLabel() {
    return Container(
      width: 32,
      decoration: BoxDecoration(
        color: GSFColors.red,
        borderRadius: BorderRadius.circular(4),
      ),
      alignment: Alignment.center,
      child: Text(
        label,
        style: const TextStyle(
          color: GSFColors.yellow,
          fontSize: 10,
          fontWeight: FontWeight.w900,
        ),
      ),
    );
  }

  Widget _buildBar() {
    return Container(
      margin: const EdgeInsets.symmetric(horizontal: 4),
      decoration: BoxDecoration(
        color: GSFColors.black,
        borderRadius: BorderRadius.circular(3),
        border: Border.all(color: GSFColors.lightGrey, width: 1),
      ),
      child: ClipRRect(
        borderRadius: BorderRadius.circular(2),
        child: LayoutBuilder(
          builder: (context, constraints) {
            return CustomPaint(
              size: Size(constraints.maxWidth, constraints.maxHeight),
              painter: _HealthBarPainter(
                value: value.clamp(0.0, 1.0),
                reversed: reversed,
              ),
            );
          },
        ),
      ),
    );
  }
}

class _HealthBarPainter extends CustomPainter {
  final double value;
  final bool reversed;
  static const int segments = 32;

  _HealthBarPainter({required this.value, required this.reversed});

  @override
  void paint(Canvas canvas, Size size) {
    final segWidth = size.width / segments;
    final gap = 1.0;
    final filledSegments = (value * segments).round();

    for (int i = 0; i < segments; i++) {
      final segIndex = reversed ? segments - 1 - i : i;
      final x = segIndex * segWidth;
      final rect = Rect.fromLTWH(x + gap / 2, 0, segWidth - gap, size.height);

      Color color;
      final normalized = i / segments;
      if (normalized < 0.5) {
        color = GSFColors.green;
      } else if (normalized < 0.7) {
        color = GSFColors.yellow;
      } else if (normalized < 0.85) {
        color = GSFColors.orange;
      } else {
        color = GSFColors.red;
      }

      if (i < filledSegments) {
        canvas.drawRect(rect, Paint()..color = color);
      } else {
        canvas.drawRect(
          rect,
          Paint()..color = GSFColors.black.withValues(alpha: 0.5),
        );
      }
    }
  }

  @override
  bool shouldRepaint(_HealthBarPainter old) =>
      old.value != value || old.reversed != reversed;
}
