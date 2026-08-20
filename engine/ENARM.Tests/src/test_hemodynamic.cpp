// test_hemodynamic.cpp - 12 tests HemodynamicModel (API real v6)
#include <catch2/catch_all.hpp>
#include "ENARM/Physiology/HemodynamicModel.h"

using namespace ENARM::Physiology;

static void RunSimulation(HemodynamicModel& m, int ticks) {
    for (int i = 0; i < ticks; ++i) m.Update(DeltaT{0.01});
}

TEST_CASE("HemodynamicModel - Constructor default no lanza", "[hemodynamic]") {
    REQUIRE_NOTHROW(HemodynamicModel{});
}

TEST_CASE("HemodynamicModel - MAP basal en rango fisiologico", "[hemodynamic]") {
    HemodynamicModel model;
    RunSimulation(model, 500);
    double map = model.GetMAP();
    INFO("MAP basal: " << map);
    REQUIRE(map >= 60.0);
    REQUIRE(map <= 120.0);
}

TEST_CASE("HemodynamicModel - Sistolica mayor que Diastolica", "[hemodynamic]") {
    HemodynamicModel model;
    RunSimulation(model, 500);
    REQUIRE(model.GetSystolicBP() > model.GetDiastolicBP());
}

TEST_CASE("HemodynamicModel - Pulse pressure positiva", "[hemodynamic]") {
    HemodynamicModel model;
    RunSimulation(model, 500);
    REQUIRE(model.GetPulsePressure() > 0.0);
}

TEST_CASE("HemodynamicModel - Cardiac output positivo", "[hemodynamic]") {
    HemodynamicModel model;
    RunSimulation(model, 500);
    REQUIRE(model.GetCardiacOutput() > 0.0);
}

TEST_CASE("HemodynamicModel - SimulateShock(0.8) reduce MAP", "[hemodynamic][shock]") {
    HemodynamicModel model;
    RunSimulation(model, 300);
    double mapBasal = model.GetMAP();

    model.SimulateShock(0.8);
    RunSimulation(model, 500);
    double mapShock = model.GetMAP();

    INFO("Basal: " << mapBasal << " Shock: " << mapShock);
    REQUIRE(mapShock < mapBasal);
}

TEST_CASE("HemodynamicModel - Shock severo MAP menor 80", "[hemodynamic][shock]") {
    HemodynamicModel model;
    model.SimulateShock(0.9);
    RunSimulation(model, 500);
    INFO("MAP shock severo: " << model.GetMAP());
    REQUIRE(model.GetMAP() < 80.0);
}

TEST_CASE("HemodynamicModel - Vasoconstriction aumenta Rp", "[hemodynamic][vasopressor]") {
    HemodynamicModel model;
    double rpBasal = model.GetParameters().Rp_baseline;
    model.ApplyVasoconstrictionBaseline(1.5);
    double rpPost = model.GetParameters().Rp_baseline;
    REQUIRE(rpPost > rpBasal);
}

TEST_CASE("HemodynamicModel - Vasoconstriction en shock recupera MAP", "[hemodynamic][sepsis]") {
    HemodynamicModel model;
    model.SimulateShock(0.8);
    RunSimulation(model, 300);
    double mapPre = model.GetMAP();

    model.ApplyVasoconstrictionBaseline(2.0);
    RunSimulation(model, 500);
    double mapPost = model.GetMAP();

    INFO("Pre: " << mapPre << " Post: " << mapPost);
    REQUIRE(mapPost > mapPre);
}

TEST_CASE("HemodynamicModel - SetHeartRate actualiza parametros", "[hemodynamic]") {
    HemodynamicModel model;
    model.SetHeartRate(120.0);
    REQUIRE(model.GetParameters().heartRate_bpm == Catch::Approx(120.0));
}

TEST_CASE("HemodynamicModel - ResetToNormal restaura fisiologia", "[hemodynamic]") {
    HemodynamicModel model;
    model.SimulateShock(0.8);
    RunSimulation(model, 200);

    model.ResetToNormal();
    RunSimulation(model, 500);
    double map = model.GetMAP();
    INFO("MAP post-reset: " << map);
    REQUIRE(map >= 60.0);
    REQUIRE(map <= 120.0);
}

TEST_CASE("HemodynamicModel - SimulateHypertension aumenta MAP", "[hemodynamic][hta]") {
    HemodynamicModel basal;
    RunSimulation(basal, 500);
    double mapBasal = basal.GetMAP();

    HemodynamicModel hta;
    hta.SimulateHypertension(0.7);
    RunSimulation(hta, 500);
    double mapHTA = hta.GetMAP();

    INFO("Basal: " << mapBasal << " HTA: " << mapHTA);
    REQUIRE(mapHTA > mapBasal);
}