"use client";

import { useEffect, useRef, useState } from "react";

const specs = [
  { label: "Audio Quality", value: "24-bit / 96kHz" },
  { label: "Latency", value: "< 50ms" },
  { label: "Protocol", value: "TCP + UDP" },
  { label: "Jitter Buffer", value: "30ms" },
  { label: "Max Devices", value: "8 phones" },
  { label: "Auth", value: "4-digit PIN" },
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
    <section ref={ref} className="py-24 px-8 bg-cds-gray-10" id="specs">
      <div className="max-w-[1200px] mx-auto">
        <h2
          className="text-[42px] leading-[1.19] text-cds-gray-100 mb-2"
          style={{ fontWeight: 300 }}
        >
          Tech specs
        </h2>
        <p className="text-sm text-cds-gray-70 tracking-[0.16px] mb-16">
          Built for professional workflows
        </p>

        <div className="grid grid-cols-1 lg:grid-cols-3 gap-px bg-cds-gray-20">
          {/* Streaming specs */}
          <div
            className={`bg-cds-white p-8 transition-all duration-700 ${
              visible ? "fade-up delay-1" : "opacity-0"
            }`}
          >
            <h3
              className="text-xs text-cds-gray-70 tracking-[0.32px] mb-8"
              style={{ fontWeight: 400 }}
            >
              STREAMING
            </h3>
            <div className="space-y-5">
              {specs.map((s) => (
                <div key={s.label} className="flex justify-between items-center">
                  <span className="text-sm text-cds-gray-70 tracking-[0.16px]">
                    {s.label}
                  </span>
                  <span
                    className="text-sm text-cds-gray-100"
                    style={{ fontWeight: 600, fontFamily: "var(--font-plex-mono), monospace" }}
                  >
                    {s.value}
                  </span>
                </div>
              ))}
            </div>
          </div>

          {/* Plugin formats */}
          <div
            className={`bg-cds-white p-8 transition-all duration-700 ${
              visible ? "fade-up delay-2" : "opacity-0"
            }`}
          >
            <h3
              className="text-xs text-cds-gray-70 tracking-[0.32px] mb-8"
              style={{ fontWeight: 400 }}
            >
              PLUGIN FORMATS
            </h3>
            <div className="space-y-3">
              {formats.map((f) => (
                <div
                  key={f.name}
                  className="flex items-center justify-between py-3 border-b border-cds-gray-20 last:border-b-0"
                >
                  <span
                    className="text-sm text-cds-gray-100"
                    style={{ fontWeight: 600 }}
                  >
                    {f.name}
                  </span>
                  <div className="flex gap-4">
                    <span className={`text-xs ${f.mac ? "text-cds-green-50" : "text-cds-gray-50"}`}>
                      macOS {f.mac ? "✓" : "—"}
                    </span>
                    <span className={`text-xs ${f.win ? "text-cds-green-50" : "text-cds-gray-50"}`}>
                      Win {f.win ? "✓" : "—"}
                    </span>
                  </div>
                </div>
              ))}
            </div>
          </div>

          {/* DAW support */}
          <div
            className={`bg-cds-white p-8 transition-all duration-700 ${
              visible ? "fade-up delay-3" : "opacity-0"
            }`}
          >
            <h3
              className="text-xs text-cds-gray-70 tracking-[0.32px] mb-8"
              style={{ fontWeight: 400 }}
            >
              DAW SUPPORT
            </h3>
            <div className="grid grid-cols-2 gap-3">
              {daws.map((d) => (
                <div key={d} className="flex items-center gap-2">
                  <span className="text-xs text-cds-green-50">✓</span>
                  <span className="text-sm text-cds-gray-100">{d}</span>
                </div>
              ))}
            </div>
            <div className="mt-8 pt-6 border-t border-cds-gray-20">
              <p className="text-xs text-cds-gray-60 tracking-[0.32px]">
                MOBILE: iOS 16+ / Android 8+
              </p>
              <p className="text-xs text-cds-gray-60 tracking-[0.32px] mt-1">
                OUTPUT: Phone speaker only
              </p>
            </div>
          </div>
        </div>
      </div>
    </section>
  );
}
