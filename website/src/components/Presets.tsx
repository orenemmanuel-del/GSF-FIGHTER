"use client";

import { useEffect, useRef, useState } from "react";

const presets = [
  {
    id: 1,
    name: "RYU",
    preset: "GSF iPHONE FIGHTER",
    desc: "Tiny speaker: no sub bass, harsh mids, narrow stereo. 200Hz high-pass.",
    color: "#00C8FF",
    eq: [10, 30, 70, 85, 40],
  },
  {
    id: 2,
    name: "CHUN-LI",
    preset: "GSF AIRPODS FIGHTER",
    desc: "ANC seal, slight bass boost, intimate stereo. Full range.",
    color: "#8800FF",
    eq: [55, 45, 50, 60, 55],
  },
  {
    id: 3,
    name: "GUILE",
    preset: "GSF VOITURE FIGHTER",
    desc: "Car cabin: sub emphasis +6dB, road noise masking, cabin resonance.",
    color: "#00FF66",
    eq: [80, 65, 40, 60, 35],
  },
  {
    id: 4,
    name: "AKUMA",
    preset: "GSF CLUB FIGHTER",
    desc: "PA system: massive sub +8dB, scooped mids, wide stereo, PA saturation.",
    color: "#FF1E1E",
    eq: [95, 70, 35, 60, 45],
  },
  {
    id: 5,
    name: "BLANKA",
    preset: "GSF CHEAP FIGHTER",
    desc: "Cheap earbuds: no bass, harsh highs, slight distortion. Worst case scenario.",
    color: "#FFE500",
    eq: [10, 25, 65, 80, 70],
  },
  {
    id: 6,
    name: "SAGAT",
    preset: "GSF STUDIO FIGHTER",
    desc: "Near-field monitors with room interaction. Almost flat reference.",
    color: "#FF6600",
    eq: [52, 48, 50, 52, 50],
  },
];

export default function Presets() {
  const ref = useRef<HTMLDivElement>(null);
  const [visible, setVisible] = useState(false);
  const [selected, setSelected] = useState(3); // Default to AKUMA

  useEffect(() => {
    const observer = new IntersectionObserver(
      ([entry]) => {
        if (entry.isIntersecting) setVisible(true);
      },
      { threshold: 0.1 }
    );
    if (ref.current) observer.observe(ref.current);
    return () => observer.disconnect();
  }, []);

  const active = presets.find((p) => p.id === selected)!;

  return (
    <section ref={ref} className="py-24 px-6" id="presets">
      <h2 className="text-center text-3xl md:text-5xl font-black tracking-wider text-gsf-yellow glow-yellow mb-4">
        SELECT YOUR FIGHTER
      </h2>
      <p className="text-center text-gsf-dim text-sm mb-16 tracking-widest uppercase">
        6 translation presets by Gesaffelstein
      </p>

      {/* Fighter grid */}
      <div className="max-w-4xl mx-auto grid grid-cols-2 md:grid-cols-3 lg:grid-cols-6 gap-4 mb-12">
        {presets.map((p, i) => (
          <button
            key={p.id}
            onClick={() => setSelected(p.id)}
            className={`relative flex flex-col items-center p-4 rounded-xl border-2 transition-all duration-300 hover:scale-105 ${
              visible ? "fade-up" : "opacity-0"
            } delay-${i + 1} ${
              selected === p.id
                ? "border-current bg-white/5"
                : "border-gsf-grey/50 bg-gsf-dark"
            }`}
            style={{
              color: p.color,
              boxShadow:
                selected === p.id
                  ? `0 0 20px ${p.color}30, inset 0 0 20px ${p.color}10`
                  : "none",
            }}
          >
            {/* Fighter initial */}
            <div
              className="w-14 h-14 rounded-full border-2 flex items-center justify-center text-2xl font-black mb-2"
              style={{
                borderColor: p.color,
                backgroundColor: `${p.color}15`,
              }}
            >
              {p.name[0]}
            </div>
            <span className="text-xs font-black tracking-wider">
              {p.name}
            </span>

            {/* Active badge */}
            {selected === p.id && (
              <span
                className="absolute -top-1 -right-1 text-[8px] font-black px-2 py-0.5 rounded-full text-gsf-black"
                style={{ backgroundColor: p.color }}
              >
                ACTIVE
              </span>
            )}
          </button>
        ))}
      </div>

      {/* Active preset detail */}
      <div
        className={`max-w-2xl mx-auto rounded-xl p-8 bg-gsf-dark border-2 transition-all duration-500 ${
          visible ? "fade-up delay-7" : "opacity-0"
        }`}
        style={{
          borderColor: `${active.color}50`,
          boxShadow: `0 0 30px ${active.color}20`,
        }}
      >
        <div className="flex items-center gap-4 mb-6">
          <div
            className="w-16 h-16 rounded-full border-2 flex items-center justify-center text-3xl font-black"
            style={{
              borderColor: active.color,
              color: active.color,
              backgroundColor: `${active.color}15`,
            }}
          >
            {active.name[0]}
          </div>
          <div>
            <h3
              className="text-2xl font-black tracking-wider"
              style={{ color: active.color }}
            >
              {active.name}
            </h3>
            <p className="text-gsf-dim text-xs tracking-wider">
              {active.preset}
            </p>
          </div>
        </div>

        <p className="text-gsf-white/80 text-sm mb-6 leading-relaxed">
          {active.desc}
        </p>

        {/* EQ visualization */}
        <div className="flex items-end gap-2 h-24">
          {["SUB", "LOW", "MID", "HIGH", "AIR"].map((band, i) => (
            <div key={band} className="flex-1 flex flex-col items-center gap-1">
              <div className="w-full bg-gsf-black rounded-sm overflow-hidden h-20 flex items-end">
                <div
                  className="w-full rounded-sm transition-all duration-500 health-animate"
                  style={{
                    height: `${active.eq[i]}%`,
                    backgroundColor: active.color,
                    opacity: 0.8,
                  }}
                />
              </div>
              <span className="text-[9px] text-gsf-dim font-bold tracking-wider">
                {band}
              </span>
            </div>
          ))}
        </div>
      </div>
    </section>
  );
}
