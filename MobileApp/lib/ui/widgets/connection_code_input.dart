/// GSF FIGHTER - Connection Code Input
/// 4-digit PIN entry styled like arcade coin slot display.

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import '../theme/gsf_theme.dart';

class ConnectionCodeInput extends StatefulWidget {
  final void Function(String code)? onCodeEntered;

  const ConnectionCodeInput({super.key, this.onCodeEntered});

  @override
  State<ConnectionCodeInput> createState() => _ConnectionCodeInputState();
}

class _ConnectionCodeInputState extends State<ConnectionCodeInput> {
  final List<TextEditingController> _controllers =
      List.generate(4, (_) => TextEditingController());
  final List<FocusNode> _focusNodes =
      List.generate(4, (_) => FocusNode());

  @override
  void dispose() {
    for (final c in _controllers) {
      c.dispose();
    }
    for (final f in _focusNodes) {
      f.dispose();
    }
    super.dispose();
  }

  void _onChanged(int index, String value) {
    if (value.length == 1 && index < 3) {
      _focusNodes[index + 1].requestFocus();
    }

    // Check if all digits entered
    final code = _controllers.map((c) => c.text).join();
    if (code.length == 4) {
      widget.onCodeEntered?.call(code);
    }
  }

  String get currentCode => _controllers.map((c) => c.text).join();

  @override
  Widget build(BuildContext context) {
    return Row(
      mainAxisAlignment: MainAxisAlignment.center,
      children: List.generate(4, (i) {
        return Container(
          width: 56,
          height: 72,
          margin: const EdgeInsets.symmetric(horizontal: 6),
          decoration: BoxDecoration(
            color: GSFColors.black,
            borderRadius: BorderRadius.circular(8),
            border: Border.all(
              color: _controllers[i].text.isNotEmpty
                  ? GSFColors.yellow
                  : GSFColors.lightGrey,
              width: 2,
            ),
            boxShadow: [
              if (_controllers[i].text.isNotEmpty)
                const BoxShadow(
                  color: GSFColors.yellowGlow,
                  blurRadius: 8,
                  spreadRadius: 1,
                ),
            ],
          ),
          child: TextField(
            controller: _controllers[i],
            focusNode: _focusNodes[i],
            textAlign: TextAlign.center,
            keyboardType: TextInputType.number,
            maxLength: 1,
            style: const TextStyle(
              color: GSFColors.yellow,
              fontSize: 32,
              fontWeight: FontWeight.w900,
            ),
            decoration: const InputDecoration(
              counterText: '',
              border: InputBorder.none,
            ),
            inputFormatters: [FilteringTextInputFormatter.digitsOnly],
            onChanged: (value) => _onChanged(i, value),
          ),
        );
      }),
    );
  }
}
