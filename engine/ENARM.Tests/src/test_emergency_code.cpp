// ============================================================
//  test_emergency_code.cpp - Tests EmergencyCode (Sprint 16)
// ============================================================
#include <catch2/catch_all.hpp>
#include "ENARM/Simulation/EmergencyCode.h"

using namespace ENARM::Simulation;

TEST_CASE("EmergencyCode - CodeBlue inicia y tiene nombre",
          "[emergency][codeblue]")
{
    EmergencyCode code;
    code.Start(EmergencyCodeType::CodeBlue);
    REQUIRE(code.IsRunning());
    REQUIRE(code.GetCodeName() == "Codigo Azul (Paro Cardiorespiratorio)");
}

TEST_CASE("EmergencyCode - accion correcta suma puntos",
          "[emergency][scoring]")
{
    EmergencyCode code;
    code.Start(EmergencyCodeType::CodeBlue);
    auto r = code.PerformAction("compresiones toracicas");
    REQUIRE(r.correct);
    REQUIRE(r.points > 0);
    REQUIRE(code.GetStatus().totalPoints > 0);
}

TEST_CASE("EmergencyCode - accion incorrecta no suma",
          "[emergency][scoring]")
{
    EmergencyCode code;
    code.Start(EmergencyCodeType::CodeBlue);
    auto r = code.PerformAction("recetar paracetamol");
    REQUIRE_FALSE(r.correct);
    REQUIRE(r.points == 0);
}

TEST_CASE("EmergencyCode - CodeSTEMI reconocido",
          "[emergency][stemi]")
{
    EmergencyCode code;
    code.Start(EmergencyCodeType::CodeSTEMI);
    REQUIRE(code.GetCodeName().find("STEMI") != std::string::npos);
    // Acciones de IAM
    auto ecg = code.PerformAction("ecg 12 derivaciones");
    REQUIRE(ecg.correct);
    auto asp = code.PerformAction("aspirina");
    REQUIRE(asp.correct);
    // Avanzar tiempo para ventana de reperfusion (> 120s)
    code.Update(150.0);
    auto rep = code.PerformAction("reperfusion hemodinamia");
    REQUIRE(rep.correct);
}

TEST_CASE("EmergencyCode - tiempo limite agota el codigo",
          "[emergency][timer]")
{
    EmergencyCode code;
    code.Start(EmergencyCodeType::CodeBlue);  // 300s
    code.Update(301.0);
    REQUIRE_FALSE(code.IsRunning());
    REQUIRE(code.GetStatus().codeEnded);
}

TEST_CASE("EmergencyCode - CodeSepsis bundle acciones",
          "[emergency][sepsis]")
{
    EmergencyCode code;
    code.Start(EmergencyCodeType::CodeSepsis);
    auto hc = code.PerformAction("hemocultivo");
    REQUIRE(hc.correct);
    auto lat = code.PerformAction("lactato");
    REQUIRE(lat.correct);
    auto abx = code.PerformAction("antibiotico empirico");
    REQUIRE(abx.correct);
    auto fl = code.PerformAction("fluidos cristaloides");
    REQUIRE(fl.correct);
}

TEST_CASE("EmergencyCode - errores graves detectados",
          "[emergency][errors]")
{
    EmergencyCode code;
    code.Start(EmergencyCodeType::CodeBlue);
    // Avanzar el tiempo a 5 min
    code.Update(250.0);
    code.PerformAction("epinefrina");
    REQUIRE_FALSE(code.SeriousErrors().empty());
}