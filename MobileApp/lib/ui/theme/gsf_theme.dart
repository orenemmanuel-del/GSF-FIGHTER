/// GSF FIGHTER - Arcade Theme
/// Street Fighter inspired dark arcade aesthetic for Flutter.

import 'package:flutter/material.dart';

class GSFColors {
  static const Color black = Color(0xFF0A0A0A);
  static const Color darkGrey = Color(0xFF1A1A1A);
  static const Color midGrey = Color(0xFF2A2A2A);
  static const Color lightGrey = Color(0xFF444444);

  static const Color red = Color(0xFFFF1E1E);
  static const Color redDark = Color(0xFF8B0000);
  static const Color redGlow = Color(0x60FF1E1E);

  static const Color blue = Color(0xFF00C8FF);
  static const Color blueDark = Color(0xFF0066AA);
  static const Color blueGlow = Color(0x6000C8FF);

  static const Color yellow = Color(0xFFFFE500);
  static const Color yellowGlow = Color(0x60FFE500);

  static const Color green = Color(0xFF00FF66);
  static const Color greenDark = Color(0xFF00AA44);

  static const Color orange = Color(0xFFFF6600);
  static const Color purple = Color(0xFF8800FF);

  static const Color white = Color(0xFFEEEEEE);
  static const Color textDim = Color(0xFF888888);

  // Gradient backgrounds
  static const LinearGradient backgroundGradient = LinearGradient(
    begin: Alignment.topCenter,
    end: Alignment.bottomCenter,
    colors: [black, Color(0xFF111111)],
  );

  static const LinearGradient redGradient = LinearGradient(
    begin: Alignment.topCenter,
    end: Alignment.bottomCenter,
    colors: [red, redDark],
  );

  static const LinearGradient blueGradient = LinearGradient(
    begin: Alignment.topCenter,
    end: Alignment.bottomCenter,
    colors: [blue, blueDark],
  );
}

class GSFTheme {
  static ThemeData get darkTheme => ThemeData(
        brightness: Brightness.dark,
        scaffoldBackgroundColor: GSFColors.black,
        primaryColor: GSFColors.red,
        colorScheme: const ColorScheme.dark(
          primary: GSFColors.red,
          secondary: GSFColors.blue,
          surface: GSFColors.darkGrey,
          error: GSFColors.red,
        ),
        fontFamily: 'ArcadeFont',
        textTheme: const TextTheme(
          displayLarge: TextStyle(
            fontSize: 36,
            fontWeight: FontWeight.w900,
            color: GSFColors.yellow,
            letterSpacing: 4,
          ),
          displayMedium: TextStyle(
            fontSize: 28,
            fontWeight: FontWeight.w900,
            color: GSFColors.red,
            letterSpacing: 3,
          ),
          headlineMedium: TextStyle(
            fontSize: 20,
            fontWeight: FontWeight.w800,
            color: GSFColors.white,
            letterSpacing: 2,
          ),
          titleLarge: TextStyle(
            fontSize: 16,
            fontWeight: FontWeight.w700,
            color: GSFColors.white,
            letterSpacing: 1.5,
          ),
          bodyLarge: TextStyle(
            fontSize: 14,
            fontWeight: FontWeight.w600,
            color: GSFColors.white,
          ),
          bodyMedium: TextStyle(
            fontSize: 12,
            fontWeight: FontWeight.w500,
            color: GSFColors.textDim,
          ),
          labelLarge: TextStyle(
            fontSize: 14,
            fontWeight: FontWeight.w800,
            color: GSFColors.yellow,
            letterSpacing: 2,
          ),
        ),
        elevatedButtonTheme: ElevatedButtonThemeData(
          style: ElevatedButton.styleFrom(
            backgroundColor: GSFColors.red,
            foregroundColor: GSFColors.white,
            padding: const EdgeInsets.symmetric(horizontal: 32, vertical: 16),
            shape: RoundedRectangleBorder(
              borderRadius: BorderRadius.circular(8),
              side: const BorderSide(color: GSFColors.yellow, width: 2),
            ),
            textStyle: const TextStyle(
              fontSize: 16,
              fontWeight: FontWeight.w900,
              letterSpacing: 3,
            ),
          ),
        ),
        cardTheme: CardThemeData(
          color: GSFColors.darkGrey,
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(8),
            side: const BorderSide(color: GSFColors.lightGrey, width: 1),
          ),
          elevation: 8,
        ),
        appBarTheme: const AppBarTheme(
          backgroundColor: GSFColors.black,
          foregroundColor: GSFColors.yellow,
          elevation: 0,
          centerTitle: true,
          titleTextStyle: TextStyle(
            fontSize: 18,
            fontWeight: FontWeight.w900,
            color: GSFColors.yellow,
            letterSpacing: 3,
          ),
        ),
      );
}
