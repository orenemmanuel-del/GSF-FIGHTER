"use client";

import { useEffect, useRef, useState } from "react";

const features = [
  {
    title: "Phone speaker",
    desc: "Hear your mix through your phone's built-in speaker in real-time. No headphones, no external speakers — just the phone.",
  },
  {
    title: "Real-time stream",
    desc: "24-bit lossless audio from your DAW master bus. Under 50ms latency over WiFi.",
  },
  {
    title: "Auto-detect",
    desc: "The app finds your plugin automatically on the network. No IP address, no setup.",
  },
  {
    title: "One tap",
    desc: "Open the app, tap Start Listening. That's it. Your mix plays on the phone speaker instantly.",
  },
  {
    title: "All DAWs",
    desc: "VST3, AU, AAX and Standalone. Logic, Ableton, Pro Tools, FL Studio, Cubase, REAPER, Bitwig.",
  },
  {
    title: "Multi-device",
    desc: "Up to 8 phones connected at the same time. Everyone in the room checks the mix on their phone.",
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
      { threshold: 0.1 }
    );
    if (ref.current) observer.observe(ref.current);
    return () => observer.disconnect();
  }, []);

  return (
    <section ref={ref} className="py-24 px-8 bg-cds-gray-10" id="features">
      <div className="max-w-[1200px] mx-auto">
        <h2
          className="text-[42px] leading-[1.19] text-cds-gray-100 mb-2"
          style={{ fontWeight: 300 }}
        >
          Why GSF Fighter
        </h2>
        <p className="text-sm text-cds-gray-70 tracking-[0.16px] mb-16">
          The fastest way to check your mix on a phone
        </p>

        <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-px bg-cds-gray-20">
          {features.map((f, i) => (
            <div
              key={i}
              className={`bg-cds-white p-8 hover:bg-cds-gray-10-hover transition-colors ${
                visible ? "fade-up" : "opacity-0"
              } delay-${Math.min(i + 1, 4)}`}
            >
              <h3
                className="text-xl leading-[1.4] text-cds-gray-100 mb-3"
                style={{ fontWeight: 600 }}
              >
                {f.title}
              </h3>
              <p className="text-sm leading-[1.29] text-cds-gray-70 tracking-[0.16px]">
                {f.desc}
              </p>
            </div>
          ))}
        </div>
      </div>
    </section>
  );
}
