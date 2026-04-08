"use client";

import { useEffect, useRef, useState } from "react";

export default function Download() {
  const ref = useRef<HTMLDivElement>(null);
  const [visible, setVisible] = useState(false);

  useEffect(() => {
    const observer = new IntersectionObserver(
      ([entry]) => {
        if (entry.isIntersecting) setVisible(true);
      },
      { threshold: 0.2 }
    );
    if (ref.current) observer.observe(ref.current);
    return () => observer.disconnect();
  }, []);

  return (
    <section
      ref={ref}
      className="py-24 px-6 relative overflow-hidden"
      id="download"
    >
      {/* Background glow */}
      <div className="absolute inset-0 bg-[radial-gradient(ellipse_at_center,rgba(255,30,30,0.06)_0%,transparent_60%)]" />

      <div
        className={`max-w-3xl mx-auto text-center relative z-10 transition-all duration-1000 ${
          visible ? "opacity-100 translate-y-0" : "opacity-0 translate-y-12"
        }`}
      >
        {/* READY? text */}
        <p className="text-gsf-blue text-lg font-black tracking-[0.5em] mb-4 glow-blue">
          READY?
        </p>

        <h2 className="text-4xl md:text-6xl font-black tracking-wider text-gsf-red glow-red mb-6">
          GET GSF FIGHTER
        </h2>

        <p className="text-gsf-dim text-sm mb-12 max-w-md mx-auto leading-relaxed">
          Download the plugin for your DAW and the mobile app for your phone.
          Start checking your mix everywhere.
        </p>

        {/* Download buttons */}
        <div className="flex flex-col sm:flex-row gap-4 justify-center mb-8">
          <a
            href="https://github.com/orenemmanuel-del/GSF-FIGHTER"
            target="_blank"
            rel="noopener noreferrer"
            className="inline-flex items-center justify-center gap-3 px-10 py-4 bg-gradient-to-b from-gsf-red to-gsf-red-dark text-gsf-white text-lg font-black tracking-wider rounded-lg border-2 border-gsf-yellow box-glow-red hover:scale-105 hover:brightness-110 transition-all duration-200 active:scale-95"
          >
            <svg className="w-6 h-6" fill="currentColor" viewBox="0 0 24 24">
              <path d="M12 0C5.37 0 0 5.37 0 12c0 5.31 3.435 9.795 8.205 11.385.6.105.825-.255.825-.57 0-.285-.015-1.23-.015-2.235-3.015.555-3.795-.735-4.035-1.41-.135-.345-.72-1.41-1.23-1.695-.42-.225-1.02-.78-.015-.795.945-.015 1.62.87 1.845 1.23 1.08 1.815 2.805 1.305 3.495.99.105-.78.42-1.305.765-1.605-2.67-.3-5.46-1.335-5.46-5.925 0-1.305.465-2.385 1.23-3.225-.12-.3-.54-1.53.12-3.18 0 0 1.005-.315 3.3 1.23.96-.27 1.98-.405 3-.405s2.04.135 3 .405c2.295-1.56 3.3-1.23 3.3-1.23.66 1.65.24 2.88.12 3.18.765.84 1.23 1.905 1.23 3.225 0 4.605-2.805 5.625-5.475 5.925.435.375.81 1.095.81 2.22 0 1.605-.015 2.895-.015 3.3 0 .315.225.69.825.57A12.02 12.02 0 0024 12c0-6.63-5.37-12-12-12z" />
            </svg>
            DOWNLOAD PLUGIN
          </a>

          <a
            href="https://github.com/orenemmanuel-del/GSF-FIGHTER"
            target="_blank"
            rel="noopener noreferrer"
            className="inline-flex items-center justify-center gap-3 px-10 py-4 bg-gradient-to-b from-gsf-blue to-gsf-blue-dark text-gsf-white text-lg font-black tracking-wider rounded-lg border-2 border-gsf-blue/50 box-glow-blue hover:scale-105 hover:brightness-110 transition-all duration-200 active:scale-95"
          >
            <svg className="w-6 h-6" fill="currentColor" viewBox="0 0 24 24">
              <path d="M17.05 20.28c-.98.95-2.05.8-3.08.35-1.09-.46-2.09-.48-3.24 0-1.44.62-2.2.44-3.06-.35C2.79 15.25 3.51 7.59 9.05 7.31c1.35.07 2.29.74 3.08.8 1.18-.24 2.31-.93 3.57-.84 1.51.12 2.65.72 3.4 1.8-3.12 1.87-2.38 5.98.48 7.13-.57 1.5-1.31 2.99-2.54 4.09zM12.03 7.25c-.15-2.23 1.66-4.07 3.74-4.25.29 2.58-2.34 4.5-3.74 4.25z" />
            </svg>
            GET MOBILE APP
          </a>
        </div>

        {/* Free label */}
        <p className="text-gsf-green text-xs font-bold tracking-[0.4em] glow-green">
          FREE & OPEN SOURCE
        </p>
      </div>

      {/* Bottom accent */}
      <div className="absolute bottom-0 left-0 right-0 h-[2px] bg-gradient-to-r from-transparent via-gsf-red to-transparent" />
    </section>
  );
}
