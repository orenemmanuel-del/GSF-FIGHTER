"use client";

import { useEffect, useState } from "react";

export default function Hero() {
  const [visible, setVisible] = useState(false);
  const [fightVisible, setFightVisible] = useState(false);

  useEffect(() => {
    setVisible(true);
    const t = setTimeout(() => setFightVisible(true), 800);
    return () => clearTimeout(t);
  }, []);

  return (
    <section className="relative min-h-screen flex flex-col items-center justify-center overflow-hidden px-6">
      {/* Background radial glow */}
      <div className="absolute inset-0 bg-[radial-gradient(ellipse_at_center,rgba(255,30,30,0.08)_0%,transparent_70%)]" />

      {/* Corner bolts */}
      {[
        "top-4 left-4",
        "top-4 right-4",
        "bottom-4 left-4",
        "bottom-4 right-4",
      ].map((pos, i) => (
        <div
          key={i}
          className={`absolute ${pos} w-3 h-3 rounded-full bg-gsf-grey border border-gsf-dark`}
        >
          <div className="absolute inset-0 flex items-center justify-center">
            <span className="block w-2 h-[1px] bg-gsf-black" />
          </div>
        </div>
      ))}

      {/* Logo */}
      <div
        className={`transition-all duration-1000 ${
          visible ? "opacity-100 translate-y-0" : "opacity-0 translate-y-8"
        }`}
      >
        <h1 className="text-center select-none">
          <span className="block text-7xl md:text-9xl font-black tracking-wider text-gsf-red glow-red">
            GSF
          </span>
          <span className="block text-6xl md:text-8xl font-black tracking-[0.3em] text-gsf-yellow glow-yellow -mt-2">
            FIGHTER
          </span>
        </h1>

        <p className="text-center text-gsf-dim text-xs tracking-[0.5em] mt-4 uppercase">
          x Gesaffelstein
        </p>
      </div>

      {/* Tagline */}
      <div
        className={`mt-10 transition-all duration-1000 delay-300 ${
          visible ? "opacity-100 translate-y-0" : "opacity-0 translate-y-8"
        }`}
      >
        <p className="text-center text-gsf-blue text-lg md:text-xl font-bold tracking-widest uppercase glow-blue">
          Master Bus Streaming System
        </p>
        <p className="text-center text-gsf-dim text-sm mt-3 max-w-lg mx-auto leading-relaxed">
          Stream your DAW mix to any phone in real-time over WiFi.
          <br />6 Translation presets by Gesaffelstein. Arcade UI.
        </p>
      </div>

      {/* FIGHT button */}
      <div
        className={`mt-12 transition-all duration-700 ${
          fightVisible
            ? "opacity-100 scale-100"
            : "opacity-0 scale-150"
        }`}
      >
        <a
          href="#download"
          className="relative inline-block px-16 py-5 bg-gradient-to-b from-gsf-red to-gsf-red-dark text-gsf-white text-2xl font-black tracking-[0.3em] rounded-lg border-2 border-gsf-yellow box-glow-red hover:scale-105 hover:brightness-110 transition-all duration-200 active:scale-95"
        >
          FIGHT!
          <span className="absolute inset-0 rounded-lg bg-gradient-to-b from-white/10 to-transparent pointer-events-none" />
        </a>
      </div>

      {/* Scroll indicator */}
      <div className="absolute bottom-8 flex flex-col items-center gap-2 pulse-glow">
        <span className="text-gsf-dim text-[10px] tracking-widest uppercase">
          Scroll down
        </span>
        <svg
          className="w-4 h-4 text-gsf-dim"
          fill="none"
          viewBox="0 0 24 24"
          stroke="currentColor"
        >
          <path
            strokeLinecap="round"
            strokeLinejoin="round"
            strokeWidth={2}
            d="M19 14l-7 7m0 0l-7-7"
          />
        </svg>
      </div>

      {/* Red accent line bottom */}
      <div className="absolute bottom-0 left-0 right-0 h-[3px] bg-gradient-to-r from-transparent via-gsf-red to-transparent" />
    </section>
  );
}
