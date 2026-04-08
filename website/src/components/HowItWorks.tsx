"use client";

import { useEffect, useRef, useState } from "react";

const steps = [
  {
    num: "01",
    title: "INSERT",
    desc: "Drop GSF FIGHTER on your DAW master bus. Last insert in the chain.",
    color: "text-gsf-red",
    accent: "bg-gsf-red",
  },
  {
    num: "02",
    title: "CONNECT",
    desc: "Open the app on your phone. It auto-detects the plugin on your WiFi.",
    color: "text-gsf-blue",
    accent: "bg-gsf-blue",
  },
  {
    num: "03",
    title: "FIGHT!",
    desc: "Press FIGHT! — your mix streams instantly to the phone in 24-bit lossless.",
    color: "text-gsf-yellow",
    accent: "bg-gsf-yellow",
  },
  {
    num: "04",
    title: "TRANSLATE",
    desc: "Select a Fighter preset to hear how your mix translates on different systems.",
    color: "text-gsf-green",
    accent: "bg-gsf-green",
  },
];

export default function HowItWorks() {
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
    <section ref={ref} className="py-24 px-6" id="how">
      {/* Top divider */}
      <div className="max-w-4xl mx-auto h-[1px] bg-gradient-to-r from-transparent via-gsf-grey to-transparent mb-20" />

      <h2 className="text-center text-3xl md:text-5xl font-black tracking-wider text-gsf-red glow-red mb-4">
        HOW IT WORKS
      </h2>
      <p className="text-center text-gsf-dim text-sm mb-16 tracking-widest uppercase">
        From DAW to phone in 4 steps
      </p>

      <div className="max-w-4xl mx-auto grid grid-cols-1 md:grid-cols-2 lg:grid-cols-4 gap-8">
        {steps.map((step, i) => (
          <div
            key={i}
            className={`relative transition-all duration-700 ${
              visible ? "fade-up" : "opacity-0"
            } delay-${i + 1}`}
          >
            {/* Step number */}
            <div
              className={`text-6xl font-black ${step.color} opacity-20 mb-2`}
            >
              {step.num}
            </div>

            {/* Accent bar */}
            <div className={`w-12 h-1 ${step.accent} rounded-full mb-4`} />

            {/* Title */}
            <h3
              className={`text-xl font-black tracking-wider ${step.color} mb-2`}
            >
              {step.title}
            </h3>

            {/* Description */}
            <p className="text-gsf-dim text-sm leading-relaxed">{step.desc}</p>

            {/* Arrow connector (not on last) */}
            {i < 3 && (
              <div className="hidden lg:block absolute top-8 -right-4 text-gsf-grey text-2xl">
                →
              </div>
            )}
          </div>
        ))}
      </div>
    </section>
  );
}
