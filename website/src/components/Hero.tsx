"use client";

import { useEffect, useState } from "react";

export default function Hero() {
  const [visible, setVisible] = useState(false);

  useEffect(() => {
    setVisible(true);
  }, []);

  return (
    <section className="relative min-h-screen flex flex-col items-center justify-center px-8 bg-cds-white">
      <div
        className={`max-w-[640px] text-center transition-all duration-700 ${
          visible ? "opacity-100 translate-y-0" : "opacity-0 translate-y-6"
        }`}
      >
        <h1
          className="text-[60px] md:text-[72px] leading-[1.17] text-cds-gray-100"
          style={{ fontWeight: 300 }}
        >
          GSF Fighter
        </h1>

        <p className="mt-6 text-base leading-[1.5] text-cds-gray-70 max-w-[480px] mx-auto">
          Stream your DAW master bus to your phone in real-time over WiFi.
          Check how your mix sounds on a phone speaker. Instantly.
        </p>

        <div className="mt-10 flex flex-col sm:flex-row gap-4 justify-center">
          <a
            href="#download"
            className="inline-flex items-center justify-center h-12 px-16 bg-cds-blue-60 text-white text-sm tracking-[0.16px] hover:bg-[#0353e9] active:bg-cds-blue-80 transition-colors"
            style={{ fontWeight: 600 }}
          >
            Get started
          </a>
          <a
            href="#features"
            className="inline-flex items-center justify-center h-12 px-16 border border-cds-blue-60 text-cds-blue-60 text-sm tracking-[0.16px] hover:bg-cds-blue-10 hover:text-cds-blue-70 transition-colors"
            style={{ fontWeight: 600 }}
          >
            Learn more
          </a>
        </div>
      </div>

      <div className="absolute bottom-0 left-0 right-0 h-px bg-cds-gray-20" />
    </section>
  );
}
