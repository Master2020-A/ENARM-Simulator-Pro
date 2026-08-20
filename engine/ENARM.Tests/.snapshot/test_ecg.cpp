// test_ecg.cpp - 5 tests ECGSynthesizer
#include <catch2/catch_all.hpp>
#include "ENARM/Physiology/ECGSynthesizer.h"

using namespace ENARM::Physiology;

TEST_CASE("ECGSynthesizer - Constructor no lanza", "[ecg]") {
    REQUIRE_NOTHROW(ECGSynthesizer{});
}

TEST_CASE("ECGSynthesizer - Genera ECG normal con 12 derivaciones", "[ecg]") {
    ECGSynthesizer synth;
    auto ecg = synth.Generate(ECGRhythm::Normal, 72.0);
    REQUIRE(ecg.leads.size() == 12);
}

TEST_CASE("ECGSynthesizer - STEMI Anterior tiene STE", "[ecg][stemi]") {
    ECGSynthesizer synth;
    auto ecg = synth.Generate(ECGRhythm::STEMI_Anterior, 95.0);
    REQUIRE(ecg.hasSTE == true);
    REQUIRE(synth.HasSTEMICriteria(ecg) == true);
}

TEST_CASE("ECGSynthesizer - ECG normal SIN criterios STEMI", "[ecg]") {
    ECGSynthesizer synth;
    auto ecg = synth.Generate(ECGRhythm::Normal, 72.0);
    REQUIRE(synth.HasSTEMICriteria(ecg) == false);
}

TEST_CASE("ECGSynthesizer - GenerateForScenario IAMCEST detecta STEMI", "[ecg]") {
    ECGSynthesizer synth;
    auto ecg = synth.GenerateForScenario("IAMCEST");
    REQUIRE(synth.HasSTEMICriteria(ecg) == true);
}