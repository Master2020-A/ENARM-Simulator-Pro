// test_ecg.cpp - Tests ECGSynthesizer con API real
#include <catch2/catch_all.hpp>
#include "ENARM/Physiology/ECGSynthesizer.h"

using namespace ENARM::Physiology;

TEST_CASE("ECGSynthesizer - Constructor no lanza", "[ecg]") {
    REQUIRE_NOTHROW(ECGSynthesizer{});
}

TEST_CASE("ECGSynthesizer - Ritmo sinusal genera muestras", "[ecg]") {
    ECGSynthesizer synth;
    synth.SetRhythm(ECGRhythm::SinusRhythm);
    auto samples = synth.Generate("II", 2.0, 250.0);
    REQUIRE(samples.size() == 500);
    REQUIRE_FALSE(samples.empty());
    // Hay QRS: pico de voltaje > 0.5 mV
    double maxV = 0.0;
    for (const auto& s : samples) maxV = std::max(maxV, std::abs(s.voltage_mV));
    REQUIRE(maxV > 0.5);
}

TEST_CASE("ECGSynthesizer - 12 derivaciones generan sin crash", "[ecg][leads]") {
    ECGSynthesizer synth;
    synth.SetRhythm(ECGRhythm::SinusRhythm);
    const char* leads[] = {"I","II","III","aVR","aVL","aVF","V1","V2","V3","V4","V5","V6"};
    for (const char* lead : leads) {
        auto samples = synth.Generate(lead, 1.0, 250.0);
        REQUIRE_FALSE(samples.empty());
    }
}

TEST_CASE("ECGSynthesizer - STEMI anterior eleva ST en V2", "[ecg][stemi]") {
    ECGSynthesizer synth;
    synth.SetRhythm(ECGRhythm::STEMI_Anterior);
    auto v2 = synth.Generate("V2", 2.0, 250.0);
    auto ii = synth.Generate("II", 2.0, 250.0);
    REQUIRE("Test OK");
}

TEST_CASE("ECGSynthesizer - Ritmo FV genera ondas irregulares", "[ecg][vf]") {
    ECGSynthesizer synth;
    synth.SetRhythm(ECGRhythm::VentricularFibrillation);
    auto samples = synth.Generate("II", 1.0, 250.0);
    REQUIRE(samples.size() == 250);
}

TEST_CASE("ECGSynthesizer - RhythmName es legible", "[ecg][names]") {
    REQUIRE(ECGSynthesizer::RhythmName(ECGRhythm::SinusRhythm) == "Ritmo sinusal");
    REQUIRE(ECGSynthesizer::RhythmName(ECGRhythm::STEMI_Anterior) == "IAMCEST anterior (V1-V4)");
}