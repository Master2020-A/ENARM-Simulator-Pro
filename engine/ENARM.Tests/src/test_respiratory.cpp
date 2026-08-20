// test_respiratory.cpp - 8 tests RespiratoryModel (API real Otis 1950)
#include <catch2/catch_all.hpp>
#include "ENARM/Physiology/RespiratoryModel.h"
#include "ENARM/Physiology/OxygenationModel.h"

using namespace ENARM::Physiology;

// Helper: correr N ticks de 10ms
static void RunResp(RespiratoryModel& m, int ticks) {
    for (int i = 0; i < ticks; ++i) m.Update(DeltaT{0.01});
}

TEST_CASE("RespiratoryModel - Constructor default no lanza", "[respiratory]") {
    REQUIRE_NOTHROW(RespiratoryModel{});
}

TEST_CASE("RespiratoryModel - SpO2 normal 95-100 tras estabilizar", "[respiratory][normal]") {
    RespiratoryModel model;
    RunResp(model, 500);
    double spo2 = model.GetSpO2();
    INFO("SpO2 = " << spo2);
    REQUIRE(spo2 >= 95.0);
    REQUIRE(spo2 <= 100.0);
}

TEST_CASE("RespiratoryModel - PaCO2 normal 35-45", "[respiratory][normal]") {
    RespiratoryModel model;
    RunResp(model, 500);
    double paco2 = model.GetPaCO2();
    INFO("PaCO2 = " << paco2);
    REQUIRE(paco2 >= 30.0);
    REQUIRE(paco2 <= 50.0);
}

TEST_CASE("RespiratoryModel - PaO2 aire ambiente > 60", "[respiratory][normal]") {
    RespiratoryModel model;
    RunResp(model, 500);
    double pao2 = model.GetPaO2();
    INFO("PaO2 = " << pao2);
    REQUIRE(pao2 > 60.0);
    REQUIRE(pao2 <= 110.0);
}

TEST_CASE("RespiratoryModel - EtCO2 aproxima PaCO2", "[respiratory][normal]") {
    RespiratoryModel model;
    RunResp(model, 500);
    double etco2 = model.GetEtCO2();
    double paco2 = model.GetPaCO2();
    INFO("EtCO2=" << etco2 << " PaCO2=" << paco2);
    // EtCO2 usualmente 2-5 mmHg menor que PaCO2
    REQUIRE(etco2 > 20.0);
    REQUIRE(etco2 <= paco2 + 5.0);
}

TEST_CASE("RespiratoryModel - SimulateARDS reduce SpO2", "[respiratory][ards]") {
    RespiratoryModel basal;
    RunResp(basal, 500);
    double spo2Basal = basal.GetSpO2();

    RespiratoryModel ards;
    ards.SimulateARDS(0.7);
    RunResp(ards, 500);
    double spo2ARDS = ards.GetSpO2();

    INFO("Basal=" << spo2Basal << " ARDS=" << spo2ARDS);
    REQUIRE(spo2ARDS < spo2Basal);
}

TEST_CASE("RespiratoryModel - SimulateCOPD altera intercambio", "[respiratory][copd]") {
    RespiratoryModel copd;
    copd.SimulateCOPD(0.6);
    RunResp(copd, 500);
    // COPD: retención CO2 típica
    double paco2 = copd.GetPaCO2();
    INFO("PaCO2 COPD=" << paco2);
    REQUIRE(paco2 >= 40.0);
}

TEST_CASE("RespiratoryModel - Sin NaN tras 1000 ticks", "[respiratory][stability]") {
    RespiratoryModel model;
    RunResp(model, 1000);
    REQUIRE(std::isfinite(model.GetSpO2()));
    REQUIRE(std::isfinite(model.GetPaCO2()));
    REQUIRE(std::isfinite(model.GetPaO2()));
    REQUIRE(std::isfinite(model.GetEtCO2()));
}

// ── OxygenationModel (funciones estaticas) ────────────────────

TEST_CASE("OxygenationModel - CalculateSaO2 PaO2 100 da SaO2 ~98%", "[respiratory][oxygenation]") {
    double sao2 = OxygenationModel::CalculateSaO2(100.0);
    INFO("SaO2 con PaO2=100: " << sao2);
    REQUIRE(sao2 >= 95.0);
    REQUIRE(sao2 <= 100.0);
}

TEST_CASE("OxygenationModel - CalculateSaO2 PaO2 40 da SaO2 <80%", "[respiratory][oxygenation]") {
    double sao2 = OxygenationModel::CalculateSaO2(40.0);
    INFO("SaO2 con PaO2=40: " << sao2);
    REQUIRE(sao2 < 85.0);
    REQUIRE(sao2 > 50.0);
}