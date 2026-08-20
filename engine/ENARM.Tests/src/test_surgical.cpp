// ============================================================
//  test_surgical.cpp - Tests SurgicalProcedure (Sprint 15)
// ============================================================
#include <catch2/catch_all.hpp>
#include "ENARM/Simulation/SurgicalProcedure.h"

using namespace ENARM::Simulation;

TEST_CASE("SurgicalProcedure - Apendicectomia pasos correctos",
          "[surgical][appendectomy]")
{
    SurgicalProcedure proc;
    proc.Start(ProcedureType::Appendectomy);
    REQUIRE(proc.IsRunning());
    REQUIRE(proc.GetProcedureName() == "Apendicectomia");

    // Paso 1: asepsia
    REQUIRE(proc.PerformAction("asepsia"));
    proc.NextStep();
    // Paso 2: incision McBurney
    REQUIRE(proc.PerformAction("incision mcburney"));
    proc.NextStep();
    // Paso 3: localizar apendice
    REQUIRE(proc.PerformAction("localizar apendice"));
    proc.NextStep();
    // Paso 4: ligadura
    REQUIRE(proc.PerformAction("ligadura mesoapendice"));
    proc.NextStep();
    // Paso 5: cierre
    REQUIRE(proc.PerformAction("cierre sutura"));
    proc.NextStep();

    REQUIRE(proc.GetStatus().IsFinished());
    REQUIRE(proc.GetStatus().totalPoints == proc.GetStatus().maxPoints);
    REQUIRE(proc.GetStatus().patientStable);
}

TEST_CASE("SurgicalProcedure - error critico complica al paciente",
          "[surgical][critical]")
{
    SurgicalProcedure proc;
    proc.Start(ProcedureType::Appendectomy);

    // Paso 3: error critico -> lesionar ciego
    proc.PerformAction("asepsia");
    proc.NextStep();
    proc.PerformAction("incision");
    proc.NextStep();
    bool ok = proc.PerformAction("lesionar ciego");
    REQUIRE_FALSE(ok);
    REQUIRE_FALSE(proc.GetStatus().patientStable);
    REQUIRE_FALSE(proc.IsRunning());
}

TEST_CASE("SurgicalProcedure - instrumentos disponibles",
          "[surgical][instruments]")
{
    auto instruments = SurgicalProcedure::AvailableInstruments(ProcedureType::AdvancedCPR);
    REQUIRE_FALSE(instruments.empty());
    REQUIRE(instruments.size() >= 4);
}

TEST_CASE("SurgicalProcedure - tiempo limite",
          "[surgical][timer]")
{
    SurgicalProcedure proc;
    proc.Start(ProcedureType::Thoracostomy);  // 300s
    proc.Update(301.0);
    REQUIRE_FALSE(proc.IsRunning());
    REQUIRE(proc.GetStatus().completed);
}

TEST_CASE("SurgicalProcedure - Cesarea requiere extraccion cuidadosa",
          "[surgical][cesarean]")
{
    SurgicalProcedure proc;
    proc.Start(ProcedureType::CesareanSection);
    REQUIRE(proc.GetProcedureName() == "Cesarea");
}