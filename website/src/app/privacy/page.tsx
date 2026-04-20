import Link from "next/link";

export const metadata = {
  title: "Privacy Policy · GSF Fighter",
  description: "Privacy Policy for GSF Fighter — Master Bus Streaming Receiver.",
};

export default function PrivacyPolicy() {
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
          Privacy Policy
        </h1>
        <p className="mt-2 text-xs text-cds-gray-60 tracking-[0.32px]">
          Last updated: April 14, 2026
        </p>

        <div className="mt-12 space-y-8 text-base leading-[1.5] text-cds-gray-70">
          <p>
            GSF Fighter (&quot;the App&quot;) is a local audio streaming receiver developed by
            Emmanuel Oren. This policy explains how the App handles your data. The
            short version: <strong className="text-cds-gray-100">it doesn&apos;t collect any.</strong>
          </p>

          <section>
            <h2 className="text-2xl text-cds-gray-100 mb-3" style={{ fontWeight: 400 }}>
              1. Data Collection
            </h2>
            <p>
              GSF Fighter does <strong className="text-cds-gray-100">not collect, store, or transmit</strong> any
              personal data, usage statistics, crash reports, analytics, or
              identifiers. No account is required. No tracking SDKs are embedded in
              the App.
            </p>
          </section>

          <section>
            <h2 className="text-2xl text-cds-gray-100 mb-3" style={{ fontWeight: 400 }}>
              2. Network Access
            </h2>
            <p>
              The App uses your device&apos;s local Wi-Fi network solely to discover
              and receive audio from the GSF Fighter DAW plugin running on your
              computer. This communication happens entirely on your local network
              (LAN). Audio streams are <strong className="text-cds-gray-100">never sent over the internet</strong>{" "}
              and never leave your home or studio network. No data is sent to any
              third-party server.
            </p>
          </section>

          <section>
            <h2 className="text-2xl text-cds-gray-100 mb-3" style={{ fontWeight: 400 }}>
              3. Permissions
            </h2>
            <p>The App requests the following iOS permission:</p>
            <ul className="mt-3 list-disc pl-6 space-y-2">
              <li>
                <strong className="text-cds-gray-100">Local Network</strong> — required to discover the DAW plugin
                over Bonjour and receive audio via UDP. This permission is used only
                for local device-to-device communication.
              </li>
            </ul>
            <p className="mt-3">
              The App does not access your contacts, location, camera, microphone,
              photos, or any other personal data.
            </p>
          </section>

          <section>
            <h2 className="text-2xl text-cds-gray-100 mb-3" style={{ fontWeight: 400 }}>
              4. Third-Party Services
            </h2>
            <p>
              GSF Fighter does not integrate any third-party analytics, advertising,
              or tracking services.
            </p>
          </section>

          <section>
            <h2 className="text-2xl text-cds-gray-100 mb-3" style={{ fontWeight: 400 }}>
              5. Children&apos;s Privacy
            </h2>
            <p>
              The App is a professional audio tool intended for a general audience.
              It does not knowingly collect any information from children or adults.
            </p>
          </section>

          <section>
            <h2 className="text-2xl text-cds-gray-100 mb-3" style={{ fontWeight: 400 }}>
              6. Changes to This Policy
            </h2>
            <p>
              This policy may be updated from time to time. The latest version will
              always be available at this URL. Continued use of the App after changes
              constitutes acceptance of the updated policy.
            </p>
          </section>

          <section>
            <h2 className="text-2xl text-cds-gray-100 mb-3" style={{ fontWeight: 400 }}>
              7. Contact
            </h2>
            <p>
              Questions about this privacy policy? Contact:{" "}
              <a
                href="mailto:oren.emmanuel@gmail.com"
                className="text-cds-blue-60 hover:text-cds-blue-70 hover:underline"
              >
                oren.emmanuel@gmail.com
              </a>
            </p>
          </section>
        </div>
      </div>
    </main>
  );
}
