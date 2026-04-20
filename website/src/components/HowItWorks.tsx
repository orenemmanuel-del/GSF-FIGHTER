"use client";

import { useEffect, useRef, useState } from "react";

const steps = [
  {
    num: "01",
    title: "Insert",
    desc: "Drop GSF Fighter on your DAW master bus. Last insert in the chain.",
  },
  {
    num: "02",
    title: "Open app",
    desc: "Open the app on your phone. It auto-detects the plugin on your WiFi.",
  },
  {
    num: "03",
    title: "Start listening",
    desc: "Tap Start Listening — your mix streams instantly to the phone speaker in 24-bit.",
  },
  {
    num: "04",
    title: "Check your mix",
    desc: "Walk around, hold the phone at arm's length, check the balance. Done.",
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
    <section ref={ref} className="py-24 px-8 bg-cds-white" id="how">
      <div className="max-w-[1200px] mx-auto">
        <h2
          className="text-[42px] leading-[1.19] text-cds-gray-100 mb-2"
          style={{ fontWeight: 300 }}
        >
          How it works
        </h2>
        <p className="text-sm text-cds-gray-70 tracking-[0.16px] mb-16">
          From DAW to phone speaker in 4 steps
        </p>

        <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-4 gap-8">
          {steps.map((step, i) => (
            <div
              key={i}
              className={`transition-all duration-600 ${
                visible ? "fade-up" : "opacity-0"
              } delay-${i + 1}`}
            >
              <div
                className="text-[48px] leading-none text-cds-blue-60 mb-4"
                style={{ fontWeight: 300, fontFamily: "var(--font-plex-mono), monospace" }}
              >
                {step.num}
              </div>

              <div className="w-8 h-[2px] bg-cds-blue-60 mb-6" />

              <h3
                className="text-xl leading-[1.4] text-cds-gray-100 mb-2"
                style={{ fontWeight: 600 }}
              >
                {step.title}
              </h3>

              <p className="text-sm leading-[1.29] text-cds-gray-70 tracking-[0.16px]">
                {step.desc}
              </p>
            </div>
          ))}
        </div>
      </div>
    </section>
  );
}
