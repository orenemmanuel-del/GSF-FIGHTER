/// GSF FIGHTER - Mobile App Entry Point
/// Master Bus Streaming Receiver x Gesaffelstein

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:provider/provider.dart';
import 'core/app_state.dart';
import 'ui/theme/gsf_theme.dart';
import 'ui/screens/home_screen.dart';

void main() {
  WidgetsFlutterBinding.ensureInitialized();

  // Lock to portrait + force dark status bar
  SystemChrome.setPreferredOrientations([
    DeviceOrientation.portraitUp,
  ]);
  SystemChrome.setSystemUIOverlayStyle(const SystemUiOverlayStyle(
    statusBarColor: Colors.transparent,
    statusBarIconBrightness: Brightness.light,
    systemNavigationBarColor: GSFColors.black,
    systemNavigationBarIconBrightness: Brightness.light,
  ));

  runApp(const GSFFighterApp());
}

class GSFFighterApp extends StatelessWidget {
  const GSFFighterApp({super.key});

  @override
  Widget build(BuildContext context) {
    return ChangeNotifierProvider(
      create: (_) => GSFAppState(),
      child: MaterialApp(
        title: 'GSF FIGHTER',
        theme: GSFTheme.darkTheme,
        debugShowCheckedModeBanner: false,
        home: const HomeScreen(),
      ),
    );
  }
}
