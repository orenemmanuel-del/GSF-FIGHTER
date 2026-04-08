"use client";

import { useEffect, useRef, useState } from "react";

const features = [
  {
    icon: "📡",
    title: "REAL-TIME STREAM",
    desc: "24-bit lossless audio from your DAW master bus to any phone. Under 50ms latency over WiFi.",
    color: "text-gsf-red",
    glow: "box-glow-red",
    border: "border-gsf-red/30",
  },
  {
    icon: "📱",
    title: "MULTI-DEVICE",
    desc: "Up to 8 phones connected simultaneously. Everyone hears the mix in real-time.",
    color: "text-gsf-blue",
    glow: "box-glow-blue",
    border: "border-gsf-blue/30",
  },
  {
    icon: "🔍",
    title: "AUTO-DETECT",
    desc: "Plugin broadcasts on your WiFi. The app finds it instantly — no IP address needed.",
    color: "text-gsf-yellow",
    glow: "box-glow-yellow",
    border: "border-gsf-yellow/30",
  },
  {
    icon: "🎛️",
    title: "6 GSF PRESETS",
    desc: "Translation presets by Gesaffelstein: hear how your mix sounds on iPhone, AirPods, car, club, cheap earbuds, studio monitors.",
    color: "text-gsf-green",
    glow: "box-glow-blue",
    border: "border-gsf-green/30",
  },
  {
    icon: "🔌",
    title: "ALL DAWs",
    desc: "VST3, AU, AAX + Standalone. Logic, Ableton, Pro Tools, FL Studio, Cubase, REAPER, Bitwig.",
    color: "text-gsf-orange",
    glow: "box-glow-red",
    border: "border-gsf-orange/30",
  },
  {
    icon: "🎮",
    title: "ARCADE UI",
    desc: "Street Fighter aesthetic. Health bar meters, FIGHT! animations, fighter character presets. Dark. Powerful.",
    color: "text-gsf-purple",
    glow: "box-glow-blue",
    border: "border-gsf-purple/30",
  },
];

export default function Features() {
  const ref = useRef<HTMLDivElement>(null);
  const [visible, setVisible] = useState(false);

  useEffect(() => {
    const observer = new IntersectionObserver(
      ([entry]) => {
        if (entry.isIntersecting) setVisible(true);
      },
      { threshold: 0.15 }
    );
    if (ref.current) observer.observe(ref.current);
    return () => observer.disconnect();
  }, []);

  return (
    <section ref={ref} className="py-24 px-6" id="features">
      <h2 className="text-center text-3xl md:text-5xl font-black tracking-wider text-gsf-yellow glow-yellow mb-4">
        CHOOSE YOUR WEAPON
      </h2>
      <p className="text-center text-gsf-dim text-sm mb-16 tracking-widest uppercase">
        Everything you need to check your mix anywhere
      </p>

      <div className="max-w-6xl mx-auto grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
        {features.map((f, i) => (
          <div
            key={i}
            className={`relative p-6 rounded-xl bg-gsf-dark border ${f.border} ${f.glow} transition-all duration-700 hover:scale-[1.02] hover:brightness-110 ${
              visible ? "fade-up" : "opacity-0"
            } delay-${i + 1}`}
          >
            <div className="text-4xl mb-4">{f.icon}</div>
            <h3
              className={`text-lg font-black tracking-wider ${f.color} mb-2`}
            >
              {f.title}
            </h3>
            <p className="text-gsf-dim text-sm leading-relaxed">{f.desc}</p>

            {/* Corner accent */}
            <div
              className={`absolute top-0 right-0 w-8 h-8 border-t-2 border-r-2 ${f.border} rounded-tr-xl`}
            />
          </div>
        ))}
      </div>
    </section>
  );
}
