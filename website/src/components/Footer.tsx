export default function Footer() {
  return (
    <footer className="py-12 px-6 border-t border-gsf-grey/20">
      <div className="max-w-4xl mx-auto text-center">
        {/* Logo small */}
        <div className="mb-6">
          <span className="text-gsf-red text-xl font-black tracking-wider">
            GSF{" "}
          </span>
          <span className="text-gsf-yellow text-xl font-black tracking-wider">
            FIGHTER
          </span>
        </div>

        {/* Nav links */}
        <div className="flex flex-wrap justify-center gap-8 mb-8">
          <a
            href="#features"
            className="text-gsf-dim text-xs tracking-wider hover:text-gsf-white transition-colors"
          >
            FEATURES
          </a>
          <a
            href="#presets"
            className="text-gsf-dim text-xs tracking-wider hover:text-gsf-white transition-colors"
          >
            PRESETS
          </a>
          <a
            href="#how"
            className="text-gsf-dim text-xs tracking-wider hover:text-gsf-white transition-colors"
          >
            HOW IT WORKS
          </a>
          <a
            href="#specs"
            className="text-gsf-dim text-xs tracking-wider hover:text-gsf-white transition-colors"
          >
            TECH SPECS
          </a>
          <a
            href="https://github.com/orenemmanuel-del/GSF-FIGHTER"
            target="_blank"
            rel="noopener noreferrer"
            className="text-gsf-dim text-xs tracking-wider hover:text-gsf-white transition-colors"
          >
            GITHUB
          </a>
        </div>

        {/* Credits */}
        <p className="text-gsf-dim/50 text-[10px] tracking-[0.3em] uppercase">
          Master Bus Streaming System x Gesaffelstein
        </p>
        <p className="text-gsf-dim/30 text-[9px] tracking-wider mt-2">
          JUCE 8 · Flutter · C++20 · Dark, powerful, professional.
        </p>
      </div>
    </footer>
  );
}
