import 'package:flutter/material.dart';

class GSFColors {
  // Carbon Gray 100 Dark Theme
  static const Color backgroundDeep  = Color(0xFF161616);
  static const Color backgroundPanel = Color(0xFF262626);
  static const Color backgroundCard  = Color(0xFF393939);
  static const Color backgroundHover = Color(0xFF4C4C4C);

  // Interactive — Blue
  static const Color accentCyan    = Color(0xFF78A9FF); // Blue 40 (dark theme interactive)
  static const Color accentCyanDim = Color(0xFF4589FF); // Blue 50
  static const Color accentRed     = Color(0xFFFA4D56); // Red 50 (dark theme error)
  static const Color accentRedDark = Color(0xFF620000); // Red 90

  // Text
  static const Color textPrimary   = Color(0xFFF4F4F4); // Gray 10
  static const Color textSecondary = Color(0xFFC6C6C6); // Gray 30
  static const Color textDim       = Color(0xFF6F6F6F); // Gray 60

  // Borders
  static const Color borderSubtle  = Color(0xFF393939); // Gray 80
  static const Color borderAccent  = Color(0xFF0F62FE); // Blue 60

  // Support
  static const Color supportSuccess = Color(0xFF42BE65); // Green 40
  static const Color supportWarning = Color(0xFFF1C21B); // Yellow 30

  // Legacy aliases
  static const Color black     = backgroundDeep;
  static const Color darkGrey  = backgroundPanel;
  static const Color midGrey   = backgroundCard;
  static const Color lightGrey = backgroundHover;
  static const Color white     = textPrimary;
  static const Color textDimLegacy = textSecondary;
  static const Color red    = accentRed;
  static const Color redDark = accentRedDark;
  static const Color redGlow = Color(0x40FA4D56);
  static const Color blue   = accentCyan;
  static const Color blueDark = accentCyanDim;
  static const Color blueGlow = Color(0x2278A9FF);
  static const Color yellow = supportWarning;
  static const Color yellowGlow = Color(0x22F1C21B);
  static const Color green  = supportSuccess;
  static const Color greenDark = Color(0xFF24A148);
  static const Color orange = accentRed;
  static const Color purple = accentCyan;

  static const LinearGradient backgroundGradient = LinearGradient(
    begin: Alignment.topCenter,
    end: Alignment.bottomCenter,
    colors: [backgroundDeep, Color(0xFF0D0D0D)],
  );
}

const String kOscMonoFont = 'IBMPlexMono';
const String kPlexSansFont = 'IBMPlexSans';

class GSFTheme {
  static ThemeData get darkTheme => ThemeData(
        brightness: Brightness.dark,
        scaffoldBackgroundColor: GSFColors.backgroundDeep,
        primaryColor: GSFColors.accentCyan,
        colorScheme: const ColorScheme.dark(
          primary: GSFColors.accentCyan,
          secondary: GSFColors.accentCyanDim,
          surface: GSFColors.backgroundPanel,
          error: GSFColors.accentRed,
        ),
        fontFamily: kPlexSansFont,
        textTheme: const TextTheme(
          displayLarge: TextStyle(
            fontSize: 32,
            fontWeight: FontWeight.w300,
            color: GSFColors.textPrimary,
            letterSpacing: 0,
          ),
          displayMedium: TextStyle(
            fontSize: 24,
            fontWeight: FontWeight.w300,
            color: GSFColors.textPrimary,
            letterSpacing: 0,
          ),
          headlineMedium: TextStyle(
            fontSize: 16,
            fontWeight: FontWeight.w600,
            color: GSFColors.textPrimary,
            letterSpacing: 0,
          ),
          titleLarge: TextStyle(
            fontSize: 14,
            fontWeight: FontWeight.w600,
            color: GSFColors.textPrimary,
            letterSpacing: 0,
          ),
          bodyLarge: TextStyle(
            fontSize: 16,
            fontWeight: FontWeight.w400,
            color: GSFColors.textPrimary,
            height: 1.5,
          ),
          bodyMedium: TextStyle(
            fontSize: 14,
            fontWeight: FontWeight.w400,
            color: GSFColors.textSecondary,
            letterSpacing: 0.16,
            height: 1.29,
          ),
          labelLarge: TextStyle(
            fontSize: 12,
            fontWeight: FontWeight.w400,
            color: GSFColors.textSecondary,
            letterSpacing: 0.32,
          ),
        ),
        cardTheme: const CardThemeData(
          color: GSFColors.backgroundPanel,
          elevation: 0,
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.zero,
            side: BorderSide(color: GSFColors.borderSubtle, width: 1),
          ),
        ),
        appBarTheme: const AppBarTheme(
          backgroundColor: GSFColors.backgroundDeep,
          foregroundColor: GSFColors.textPrimary,
          elevation: 0,
          centerTitle: false,
          titleTextStyle: TextStyle(
            fontSize: 14,
            fontWeight: FontWeight.w600,
            color: GSFColors.textPrimary,
            letterSpacing: 0.16,
          ),
        ),
      );
}
