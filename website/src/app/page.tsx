import Hero from "@/components/Hero";
import Features from "@/components/Features";
import Presets from "@/components/Presets";
import HowItWorks from "@/components/HowItWorks";
import TechSpecs from "@/components/TechSpecs";
import Download from "@/components/Download";
import Footer from "@/components/Footer";

export default function Home() {
  return (
    <main className="grid-bg">
      <Hero />
      <Features />
      <Presets />
      <HowItWorks />
      <TechSpecs />
      <Download />
      <Footer />
    </main>
  );
}
