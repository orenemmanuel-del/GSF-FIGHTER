import Link from "next/link";

export default function Footer() {
  return (
    <footer className="py-12 px-8 bg-cds-gray-100">
      <div className="max-w-[1200px] mx-auto">
        <div className="flex flex-col md:flex-row justify-between items-start gap-8">
          <div>
            <span
              className="text-base text-cds-white"
              style={{ fontWeight: 600 }}
            >
              GSF Fighter
            </span>
            <p className="mt-2 text-sm text-cds-gray-30">
              Master Bus Streaming System
            </p>
          </div>

          <div className="flex flex-wrap gap-8">
            <a
              href="#features"
              className="text-sm text-cds-gray-30 hover:text-cds-white transition-colors"
            >
              Features
            </a>
            <a
              href="#how"
              className="text-sm text-cds-gray-30 hover:text-cds-white transition-colors"
            >
              How it works
            </a>
            <a
              href="#specs"
              className="text-sm text-cds-gray-30 hover:text-cds-white transition-colors"
            >
              Tech specs
            </a>
            <a
              href="https://github.com/orenemmanuel-del/GSF-FIGHTER"
              target="_blank"
              rel="noopener noreferrer"
              className="text-sm text-cds-gray-30 hover:text-cds-white transition-colors"
            >
              GitHub
            </a>
            <Link
              href="/privacy"
              className="text-sm text-cds-gray-30 hover:text-cds-white transition-colors"
            >
              Privacy
            </Link>
            <Link
              href="/support"
              className="text-sm text-cds-gray-30 hover:text-cds-white transition-colors"
            >
              Support
            </Link>
          </div>
        </div>

        <div className="mt-12 pt-8 border-t border-cds-gray-80">
          <p className="text-xs text-cds-gray-50 tracking-[0.32px]">
            JUCE 8 · Flutter · C++20
          </p>
        </div>
      </div>
    </footer>
  );
}
