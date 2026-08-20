// test_disease_progression.cpp - 4 tests DiseaseProgressionEngine
#include <catch2/catch_all.hpp>
#include "ENARM/Physiology/DiseaseProgressionEngine.h"

using namespace ENARM::Physiology;

TEST_CASE("DiseaseProgression - Inicia en Compensated", "[progression]") {
    DiseaseProgressionEngine engine("SepticShock");
    REQUIRE(engine.GetCurrentStage() == DiseaseStage::Compensated);
    REQUIRE(engine.IsPatientAlive() == true);
}

TEST_CASE("DiseaseProgression - Sin tratamiento progresa", "[progression]") {
    DiseaseProgressionEngine engine("SepticShock");
    engine.Advance(45.0);
    REQUIRE(engine.GetCurrentStage() != DiseaseStage::Compensated);
}

TEST_CASE("DiseaseProgression - Tratamiento reduce mortalidad", "[progression]") {
    DiseaseProgressionEngine engine("SepticShock");
    engine.Advance(20.0);
    engine.ApplyTreatment("antibiotics");
    engine.ApplyTreatment("fluids");
    engine.ApplyTreatment("norepinephrine");
    REQUIRE(engine.GetMortalityRisk() < 0.5);
}

TEST_CASE("DiseaseProgression - Tiempo se acumula", "[progression]") {
    DiseaseProgressionEngine engine("SepticShock");
    engine.Advance(10.0);
    engine.Advance(15.0);
    REQUIRE(engine.GetTimeElapsed() == Catch::Approx(25.0).margin(0.1));
}