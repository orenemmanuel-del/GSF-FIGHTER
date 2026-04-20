import type { Metadata } from "next";
import { IBM_Plex_Sans, IBM_Plex_Mono } from "next/font/google";
import "./globals.css";

const plex = IBM_Plex_Sans({
  variable: "--font-plex",
  subsets: ["latin"],
  weight: ["300", "400", "600"],
});

const plexMono = IBM_Plex_Mono({
  variable: "--font-plex-mono",
  subsets: ["latin"],
  weight: ["400"],
});

export const metadata: Metadata = {
  title: "GSF Fighter — Master Bus Streaming System",
  description:
    "Stream your DAW master bus to mobile in real-time. Check your mix on phone speaker instantly.",
  keywords: [
    "GSF Fighter",
    "DAW streaming",
    "master bus",
    "audio plugin",
    "JUCE",
    "VST3",
    "AU",
  ],
  openGraph: {
    title: "GSF Fighter",
    description: "Master Bus Streaming System",
    type: "website",
  },
};

export default function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) {
  return (
    <html lang="en" className={`${plex.variable} ${plexMono.variable} h-full antialiased`}>
      <body className="min-h-full flex flex-col" style={{ fontFamily: "var(--font-plex), 'Helvetica Neue', Arial, sans-serif" }}>
        {children}
      </body>
    </html>
  );
}
