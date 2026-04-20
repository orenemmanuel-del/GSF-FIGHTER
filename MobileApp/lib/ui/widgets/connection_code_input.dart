/// GSF FIGHTER — Connection Code Input (Oscilloscope Design)
/// 4-digit code entry with cyan accent.

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
    final code = _controllers.map((c) => c.text).join();
    if (code.length == 4) {
      widget.onCodeEntered?.call(code);
    }
    setState(() {});
  }

  @override
  Widget build(BuildContext context) {
    return Row(
      mainAxisAlignment: MainAxisAlignment.center,
      children: List.generate(4, (i) {
        final filled = _controllers[i].text.isNotEmpty;
        return Container(
          width: 52,
          height: 64,
          margin: const EdgeInsets.symmetric(horizontal: 5),
          decoration: BoxDecoration(
            color: GSFColors.backgroundDeep,
            borderRadius: BorderRadius.zero,
            border: Border.all(
              color: filled ? GSFColors.accentCyan : GSFColors.borderSubtle,
              width: 1,
            ),
          ),
          child: TextField(
            controller: _controllers[i],
            focusNode: _focusNodes[i],
            textAlign: TextAlign.center,
            keyboardType: TextInputType.number,
            maxLength: 1,
            style: const TextStyle(
              fontFamily: kOscMonoFont,
              color: GSFColors.accentCyan,
              fontSize: 28,
              fontWeight: FontWeight.w700,
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
