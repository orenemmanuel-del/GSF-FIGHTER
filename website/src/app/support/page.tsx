import Link from "next/link";

export const metadata = {
  title: "Support · GSF Fighter",
  description: "Support and troubleshooting for GSF Fighter — Master Bus Streaming Receiver.",
};

export default function Support() {
  return (
    <main className="min-h-screen px-8 py-16 md:py-24 bg-cds-white">
      <div className="max-w-[640px] mx-auto">
        <Link
          href="/"
          className="text-sm text-cds-blue-60 hover:text-cds-blue-70 hover:underline transition-colors"
        >
          ← Back to GSF Fighter
        </Link>

        <h1
          className="mt-8 text-[42px] leading-[1.19] text-cds-gray-100"
          style={{ fontWeight: 300 }}
        >
          Support
        </h1>
        <p className="mt-2 text-sm text-cds-gray-70 tracking-[0.16px]">
          Need help? You&apos;re in the right place.
        </p>

        <div className="mt-12 space-y-10 text-base leading-[1.5] text-cds-gray-70">
          <section>
            <h2 className="text-2xl text-cds-gray-100 mb-4" style={{ fontWeight: 400 }}>
              Quick Start
            </h2>
            <ol className="list-decimal pl-6 space-y-3">
              <li>
                Install the GSF Fighter plugin on the master bus of your DAW
                (VST3 / AU / AAX).
              </li>
              <li>
                Make sure your iPhone and your studio computer are on the{" "}
                <strong className="text-cds-gray-100">same Wi-Fi network</strong>.
              </li>
              <li>
                Open the GSF Fighter app on your iPhone. Grant it Local
                Network access when prompted.
              </li>
              <li>
                The app auto-discovers the plugin via Bonjour and starts
                streaming within a few seconds.
              </li>
            </ol>
          </section>

          <section>
            <h2 className="text-2xl text-cds-gray-100 mb-4" style={{ fontWeight: 400 }}>
              Troubleshooting
            </h2>

            <div className="space-y-8">
              <div>
                <h3 className="text-base text-cds-gray-100 mb-2" style={{ fontWeight: 600 }}>
                  The app says &quot;searching&quot; forever
                </h3>
                <ul className="list-disc pl-6 space-y-2">
                  <li>Confirm both devices are on the same Wi-Fi network.</li>
                  <li>Check that the plugin is loaded and playing audio in your DAW.</li>
                  <li>Some routers block Bonjour/mDNS — try disabling AP isolation or guest network.</li>
                  <li>In iOS Settings → GSF Fighter → Local Network → make sure it&apos;s enabled.</li>
                </ul>
              </div>

              <div>
                <h3 className="text-base text-cds-gray-100 mb-2" style={{ fontWeight: 600 }}>
                  Audio stutters or drops out
                </h3>
                <ul className="list-disc pl-6 space-y-2">
                  <li>Move closer to your Wi-Fi router or use a 5 GHz network for lower latency.</li>
                  <li>Other heavy Wi-Fi traffic (video streaming, downloads) can saturate the network.</li>
                  <li>Try restarting the plugin and the app if the connection degrades.</li>
                </ul>
              </div>

              <div>
                <h3 className="text-base text-cds-gray-100 mb-2" style={{ fontWeight: 600 }}>
                  Plugin not showing in my DAW
                </h3>
                <ul className="list-disc pl-6 space-y-2">
                  <li>Rescan plugins in your DAW&apos;s plugin manager after installation.</li>
                  <li>Confirm you installed the correct format (VST3 / AU / AAX) for your DAW.</li>
                </ul>
              </div>
            </div>
          </section>

          <section>
            <h2 className="text-2xl text-cds-gray-100 mb-4" style={{ fontWeight: 400 }}>
              Still stuck?
            </h2>
            <p>
              Reach out directly — response within 48 hours:{" "}
              <a
                href="mailto:oren.emmanuel@gmail.com"
                className="text-cds-blue-60 hover:text-cds-blue-70 hover:underline"
              >
                oren.emmanuel@gmail.com
              </a>
            </p>
            <p className="mt-4">When you email, include:</p>
            <ul className="mt-2 list-disc pl-6 space-y-2">
              <li>Your iPhone model and iOS version</li>
              <li>Your DAW name and version</li>
              <li>A short description of what you&apos;re seeing</li>
            </ul>
          </section>
        </div>
      </div>
    </main>
  );
}
