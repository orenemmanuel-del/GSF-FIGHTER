"use client";

import { useEffect, useRef, useState } from "react";

const specs = [
  { label: "Audio Quality", value: "24-bit / 96kHz", color: "text-gsf-red" },
  { label: "Latency", value: "< 50ms", color: "text-gsf-green" },
  { label: "Protocol", value: "TCP + UDP", color: "text-gsf-blue" },
  { label: "Jitter Buffer", value: "30ms", color: "text-gsf-yellow" },
  { label: "Max Clients", value: "8 devices", color: "text-gsf-orange" },
  { label: "Auth", value: "4-digit PIN", color: "text-gsf-purple" },
];

const formats = [
  { name: "VST3", mac: true, win: true },
  { name: "AU", mac: true, win: false },
  { name: "AAX", mac: true, win: true },
  { name: "Standalone", mac: true, win: true },
];

const daws = [
  "Logic Pro",
  "Ableton Live",
  "Pro Tools",
  "FL Studio",
  "Cubase",
  "REAPER",
  "Bitwig Studio",
  "Studio One",
];

export default function TechSpecs() {
  const ref = useRef<HTMLDivElement>(null);
  const [visible, setVisible] = useState(false);

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

  return (
    <section ref={ref} className="py-24 px-6" id="specs">
      <h2 className="text-center text-3xl md:text-5xl font-black tracking-wider text-gsf-blue glow-blue mb-4">
        TECH SPECS
      </h2>
      <p className="text-center text-gsf-dim text-sm mb-16 tracking-widest uppercase">
        Built for professional workflows
      </p>

      <div className="max-w-5xl mx-auto grid grid-cols-1 lg:grid-cols-3 gap-8">
        {/* Specs grid */}
        <div
          className={`rounded-xl p-6 bg-gsf-dark border border-gsf-grey/30 transition-all duration-700 ${
            visible ? "fade-up delay-1" : "opacity-0"
          }`}
        >
          <h3 className="text-gsf-yellow text-sm font-black tracking-wider mb-6">
            STREAMING
          </h3>
          <div className="space-y-4">
            {specs.map((s) => (
              <div key={s.label} className="flex justify-between items-center">
                <span className="text-gsf-dim text-xs tracking-wider">
                  {s.label}
                </span>
                <span className={`${s.color} text-sm font-bold`}>
                  {s.value}
                </span>
              </div>
            ))}
          </div>
        </div>

        {/* Plugin formats */}
        <div
          className={`rounded-xl p-6 bg-gsf-dark border border-gsf-grey/30 transition-all duration-700 ${
            visible ? "fade-up delay-2" : "opacity-0"
          }`}
        >
          <h3 className="text-gsf-yellow text-sm font-black tracking-wider mb-6">
            PLUGIN FORMATS
          </h3>
          <div className="space-y-3">
            {formats.map((f) => (
              <div
                key={f.name}
                className="flex items-center justify-between p-3 rounded-lg bg-gsf-black/50"
              >
                <span className="text-gsf-white text-sm font-bold">
                  {f.name}
                </span>
                <div className="flex gap-3">
                  <span
                    className={`text-xs font-bold ${
                      f.mac ? "text-gsf-green" : "text-gsf-grey"
                    }`}
                  >
                    macOS {f.mac ? "✓" : "—"}
                  </span>
                  <span
                    className={`text-xs font-bold ${
                      f.win ? "text-gsf-green" : "text-gsf-grey"
                    }`}
                  >
                    Win {f.win ? "✓" : "—"}
                  </span>
                </div>
              </div>
            ))}
          </div>
        </div>

        {/* DAW compatibility */}
        <div
          className={`rounded-xl p-6 bg-gsf-dark border border-gsf-grey/30 transition-all duration-700 ${
            visible ? "fade-up delay-3" : "opacity-0"
          }`}
        >
          <h3 className="text-gsf-yellow text-sm font-black tracking-wider mb-6">
            DAW SUPPORT
          </h3>
          <div className="grid grid-cols-2 gap-2">
            {daws.map((d) => (
              <div
                key={d}
                className="flex items-center gap-2 p-2 rounded-lg bg-gsf-black/50"
              >
                <span className="text-gsf-green text-xs">✓</span>
                <span className="text-gsf-white text-xs font-medium">{d}</span>
              </div>
            ))}
          </div>
          <div className="mt-4 pt-4 border-t border-gsf-grey/20">
            <p className="text-gsf-dim text-[10px] tracking-wider">
              MOBILE: iOS 16+ / Android 8+
            </p>
            <p className="text-gsf-dim text-[10px] tracking-wider mt-1">
              APP: Flutter (iOS + Android)
            </p>
          </div>
        </div>
      </div>
    </section>
  );
}
