/// GSF FIGHTER - Home Screen
/// Main entry point: "Hi there! Start listening to your mix."
/// Shows discovered hosts + manual connect option.

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../../core/app_state.dart';
import '../../core/gsf_protocol.dart';
import '../../network/discovery_client.dart';
import '../theme/gsf_theme.dart';
import '../widgets/arcade_button.dart';
import '../widgets/scanline_overlay.dart';
import '../widgets/connection_code_input.dart';
import 'listening_screen.dart';

class HomeScreen extends StatefulWidget {
  const HomeScreen({super.key});

  @override
  State<HomeScreen> createState() => _HomeScreenState();
}

class _HomeScreenState extends State<HomeScreen> with TickerProviderStateMixin {
  late AnimationController _titleController;
  late AnimationController _pulseController;
  bool _showManualConnect = false;
  final TextEditingController _ipController = TextEditingController();

  @override
  void initState() {
    super.initState();
    _titleController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 800),
    )..forward();

    _pulseController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 2000),
    )..repeat(reverse: true);

    // Auto-start discovery
    WidgetsBinding.instance.addPostFrameCallback((_) {
      context.read<GSFAppState>().startDiscovery();
    });
  }

  @override
  void dispose() {
    _titleController.dispose();
    _pulseController.dispose();
    _ipController.dispose();
    super.dispose();
  }

  void _connectToHost(DiscoveredHost host) async {
    final appState = context.read<GSFAppState>();
    final connected = await appState.connectToHost(host);

    if (connected && mounted) {
      Navigator.of(context).push(
        MaterialPageRoute(builder: (_) => const ListeningScreen()),
      );
    }
  }

  void _connectManual(String code) async {
    final ip = _ipController.text.trim();
    if (ip.isEmpty || code.length != 4) return;

    final appState = context.read<GSFAppState>();
    final connected = await appState.connectManual(ip, code);

    if (connected && mounted) {
      Navigator.of(context).push(
        MaterialPageRoute(builder: (_) => const ListeningScreen()),
      );
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Stack(
        children: [
          // Background
          Container(decoration: const BoxDecoration(gradient: GSFColors.backgroundGradient)),

          // Grid pattern
          CustomPaint(
            size: MediaQuery.of(context).size,
            painter: _GridPainter(),
          ),

          // Main content
          SafeArea(
            child: Consumer<GSFAppState>(
              builder: (context, appState, _) {
                return Column(
                  children: [
                    const SizedBox(height: 40),

                    // Logo
                    _buildLogo(),
                    const SizedBox(height: 12),

                    // Subtitle
                    AnimatedBuilder(
                      animation: _pulseController,
                      builder: (context, _) {
                        return Text(
                          'START LISTENING TO YOUR MIX',
                          style: TextStyle(
                            color: GSFColors.blue.withValues(
                              alpha: 0.6 + 0.4 * _pulseController.value,
                            ),
                            fontSize: 13,
                            fontWeight: FontWeight.w700,
                            letterSpacing: 2,
                          ),
                        );
                      },
                    ),

                    const SizedBox(height: 32),

                    // Discovered hosts
                    Expanded(
                      child: _buildHostList(appState),
                    ),

                    // Manual connect toggle
                    if (_showManualConnect) _buildManualConnect(),

                    // Bottom actions
                    Padding(
                      padding: const EdgeInsets.all(16),
                      child: Row(
                        children: [
                          Expanded(
                            child: ArcadeButton(
                              text: _showManualConnect ? 'AUTO DETECT' : 'MANUAL CODE',
                              style: ArcadeButtonStyle.shoryuken,
                              height: 44,
                              onPressed: () {
                                setState(() => _showManualConnect = !_showManualConnect);
                              },
                            ),
                          ),
                        ],
                      ),
                    ),

                    const SizedBox(height: 8),
                  ],
                );
              },
            ),
          ),

          // Scanlines
          const ScanlineOverlay(),
        ],
      ),
    );
  }

  Widget _buildLogo() {
    return FadeTransition(
      opacity: _titleController,
      child: Column(
        children: [
          Row(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              Text(
                'GSF ',
                style: TextStyle(
                  color: GSFColors.red,
                  fontSize: 42,
                  fontWeight: FontWeight.w900,
                  letterSpacing: 4,
                  shadows: [
                    Shadow(
                      color: GSFColors.red.withValues(alpha: 0.5),
                      blurRadius: 20,
                    ),
                  ],
                ),
              ),
              Text(
                'FIGHTER',
                style: TextStyle(
                  color: GSFColors.yellow,
                  fontSize: 42,
                  fontWeight: FontWeight.w900,
                  letterSpacing: 4,
                  shadows: [
                    Shadow(
                      color: GSFColors.yellow.withValues(alpha: 0.5),
                      blurRadius: 20,
                    ),
                  ],
                ),
              ),
            ],
          ),
          const SizedBox(height: 4),
          Text(
            'x GESAFFELSTEIN',
            style: TextStyle(
              color: GSFColors.textDim.withValues(alpha: 0.6),
              fontSize: 10,
              fontWeight: FontWeight.w600,
              letterSpacing: 6,
            ),
          ),
        ],
      ),
    );
  }

  Widget _buildHostList(GSFAppState appState) {
    final hosts = appState.availableHosts;

    if (hosts.isEmpty) {
      return Center(
        child: Column(
          mainAxisSize: MainAxisSize.min,
          children: [
            AnimatedBuilder(
              animation: _pulseController,
              builder: (context, _) {
                return Icon(
                  Icons.wifi_find,
                  size: 48,
                  color: GSFColors.blue.withValues(
                    alpha: 0.3 + 0.3 * _pulseController.value,
                  ),
                );
              },
            ),
            const SizedBox(height: 16),
            const Text(
              'SEARCHING FOR FIGHTERS...',
              style: TextStyle(
                color: GSFColors.textDim,
                fontSize: 13,
                fontWeight: FontWeight.w700,
                letterSpacing: 2,
              ),
            ),
            const SizedBox(height: 8),
            const Text(
              'Make sure the GSF FIGHTER plugin\nis inserted on your master bus',
              textAlign: TextAlign.center,
              style: TextStyle(
                color: GSFColors.textDim,
                fontSize: 11,
                height: 1.6,
              ),
            ),
          ],
        ),
      );
    }

    return ListView.builder(
      padding: const EdgeInsets.symmetric(horizontal: 16),
      itemCount: hosts.length,
      itemBuilder: (context, index) {
        return _buildHostCard(hosts[index]);
      },
    );
  }

  Widget _buildHostCard(DiscoveredHost host) {
    return Container(
      margin: const EdgeInsets.only(bottom: 12),
      decoration: BoxDecoration(
        color: GSFColors.darkGrey,
        borderRadius: BorderRadius.circular(10),
        border: Border.all(color: GSFColors.lightGrey, width: 1),
        boxShadow: const [
          BoxShadow(
            color: Colors.black26,
            blurRadius: 8,
            offset: Offset(0, 4),
          ),
        ],
      ),
      child: Material(
        color: Colors.transparent,
        child: InkWell(
          borderRadius: BorderRadius.circular(10),
          onTap: host.hasSlots ? () => _connectToHost(host) : null,
          child: Padding(
            padding: const EdgeInsets.all(16),
            child: Row(
              children: [
                // DAW icon
                Container(
                  width: 48,
                  height: 48,
                  decoration: BoxDecoration(
                    color: GSFColors.midGrey,
                    borderRadius: BorderRadius.circular(8),
                    border: Border.all(color: GSFColors.blue, width: 1),
                  ),
                  child: const Icon(
                    Icons.speaker_group,
                    color: GSFColors.blue,
                    size: 24,
                  ),
                ),
                const SizedBox(width: 14),

                // Info
                Expanded(
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Text(
                        host.displayName,
                        style: const TextStyle(
                          color: GSFColors.white,
                          fontSize: 14,
                          fontWeight: FontWeight.w700,
                        ),
                      ),
                      const SizedBox(height: 4),
                      Text(
                        host.payload.displayInfo,
                        style: const TextStyle(
                          color: GSFColors.textDim,
                          fontSize: 10,
                        ),
                      ),
                      const SizedBox(height: 4),
                      Row(
                        children: [
                          Container(
                            width: 8,
                            height: 8,
                            decoration: BoxDecoration(
                              color: host.hasSlots ? GSFColors.green : GSFColors.red,
                              shape: BoxShape.circle,
                            ),
                          ),
                          const SizedBox(width: 6),
                          Text(
                            '${host.payload.currentClients}/${host.payload.maxClients} connected',
                            style: const TextStyle(
                              color: GSFColors.textDim,
                              fontSize: 10,
                            ),
                          ),
                        ],
                      ),
                    ],
                  ),
                ),

                // Connect button
                ArcadeButton(
                  text: 'FIGHT!',
                  style: ArcadeButtonStyle.hadouken,
                  width: 80,
                  height: 40,
                  onPressed: host.hasSlots ? () => _connectToHost(host) : null,
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }

  Widget _buildManualConnect() {
    return Container(
      margin: const EdgeInsets.symmetric(horizontal: 16),
      padding: const EdgeInsets.all(16),
      decoration: BoxDecoration(
        color: GSFColors.darkGrey,
        borderRadius: BorderRadius.circular(10),
        border: Border.all(color: GSFColors.blue, width: 1),
      ),
      child: Column(
        children: [
          const Text(
            'ENTER CONNECTION CODE',
            style: TextStyle(
              color: GSFColors.yellow,
              fontSize: 13,
              fontWeight: FontWeight.w800,
              letterSpacing: 2,
            ),
          ),
          const SizedBox(height: 12),

          // IP address input
          TextField(
            controller: _ipController,
            style: const TextStyle(color: GSFColors.white, fontSize: 16),
            decoration: InputDecoration(
              hintText: '192.168.1.xxx',
              hintStyle: TextStyle(color: GSFColors.textDim.withValues(alpha: 0.5)),
              filled: true,
              fillColor: GSFColors.black,
              border: OutlineInputBorder(
                borderRadius: BorderRadius.circular(8),
                borderSide: const BorderSide(color: GSFColors.lightGrey),
              ),
              contentPadding: const EdgeInsets.symmetric(horizontal: 16, vertical: 12),
              prefixText: 'IP: ',
              prefixStyle: const TextStyle(
                color: GSFColors.textDim,
                fontWeight: FontWeight.w600,
              ),
            ),
            keyboardType: const TextInputType.numberWithOptions(decimal: true),
          ),
          const SizedBox(height: 16),

          // 4-digit code input
          ConnectionCodeInput(
            onCodeEntered: _connectManual,
          ),
        ],
      ),
    );
  }
}

class _GridPainter extends CustomPainter {
  @override
  void paint(Canvas canvas, Size size) {
    final paint = Paint()
      ..color = GSFColors.midGrey.withValues(alpha: 0.06)
      ..strokeWidth = 0.5;

    for (double x = 0; x < size.width; x += 20) {
      canvas.drawLine(Offset(x, 0), Offset(x, size.height), paint);
    }
    for (double y = 0; y < size.height; y += 20) {
      canvas.drawLine(Offset(0, y), Offset(size.width, y), paint);
    }
  }

  @override
  bool shouldRepaint(covariant CustomPainter oldDelegate) => false;
}
