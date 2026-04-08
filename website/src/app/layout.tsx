import type { Metadata } from "next";
import { Geist_Mono } from "next/font/google";
import "./globals.css";

const mono = Geist_Mono({
  variable: "--font-mono",
  subsets: ["latin"],
});

export const metadata: Metadata = {
  title: "GSF FIGHTER — Master Bus Streaming x Gesaffelstein",
  description:
    "Stream your DAW master bus to mobile in real-time. 6 Translation presets by Gesaffelstein. Street Fighter arcade aesthetic.",
  keywords: [
    "GSF FIGHTER",
    "Gesaffelstein",
    "DAW streaming",
    "Mix to Mobile",
    "audio plugin",
    "JUCE",
    "VST3",
    "AU",
  ],
  openGraph: {
    title: "GSF FIGHTER",
    description: "Master Bus Streaming System x Gesaffelstein",
    type: "website",
  },
};

export default function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) {
  return (
    <html lang="en" className={`${mono.variable} h-full antialiased`}>
      <body className="min-h-full flex flex-col scanlines">{children}</body>
    </html>
  );
}
