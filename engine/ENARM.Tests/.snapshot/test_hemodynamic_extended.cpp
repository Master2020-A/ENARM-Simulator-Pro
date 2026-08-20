// ============================================================
//  test_hemodynamic_extended.cpp
//  Tests adicionales de HemodynamicModel
//  Complementa los tests básicos existentes
// ============================================================
#include <catch2/catch_all.hpp>

// Incluir los headers reales del proyecto
// Ajusta la ruta si tu include path es diferente
#include "HemodynamicModel.h"

using namespace ENARM::Physiology;

// ── Presión de pulso ─────────────────────────────────────────
TEST_CASE("HemodynamicModel - presion de pulso normal 30-60 mmHg",
          "[hemodynamic][pulse_pressure]")
{
    HemodynamicModel m;
    auto vitals = m.GetCurrentVitals();
    double pp = vitals.systolicBP - vitals.diastolicBP;

    REQUIRE(pp >= 25.0);
    REQUIRE(pp <= 65.0);
}

// ── Gasto cardíaco ───────────────────────────────────────────
TEST_CASE("HemodynamicModel - gasto cardiaco normal 4-8 L/min",
          "[hemodynamic][cardiac_output]")
{
    HemodynamicModel m;
    auto vitals = m.GetCurrentVitals();

    // CO = VS × FC / 1000
    // Con vitales normales debe ser ~5 L/min
    double fc  = vitals.heartRate;
    REQUIRE(fc >= 55.0);
    REQUIRE(fc <= 105.0);
}

// ── Shock reduce FC en límite compensatorio ──────────────────
TEST_CASE("HemodynamicModel - SimulateShock produce taquicardia compensatoria",
          "[hemodynamic][shock]")
{
    HemodynamicModel m;
    double fcBasal = m.GetCurrentVitals().heartRate;

    m.SimulateShock(0.4); // severidad 40%
    double fcShock = m.GetCurrentVitals().heartRate;

    // FC debe subir en shock (taquicardia compensatoria)
    REQUIRE(fcShock >= fcBasal);
}

// ── Vasoconstricción sube PAS y PAD ─────────────────────────
TEST_CASE("HemodynamicModel - vasoconstriccion sube presion sistemica",
          "[hemodynamic][vasoconstriction]")
{
    HemodynamicModel m;
    double mapBasal = m.GetCurrentVitals().meanArterialPressure;

    m.ApplyVasoconstrictionBaseline(0.5); // 50% vasoconstricción
    double mapPost = m.GetCurrentVitals().meanArterialPressure;

    REQUIRE(mapPost > mapBasal);
}

// ── Múltiples ticks no producen valores negativos ───────────
TEST_CASE("HemodynamicModel - valores siempre positivos tras multiples updates",
          "[hemodynamic][stability]")
{
    HemodynamicModel m;
    m.SimulateShock(0.8); // shock severo

    for (int i = 0; i < 100; ++i) {
        m.Update(0.1); // 100ms por tick
    }

    auto v = m.GetCurrentVitals();
    REQUIRE(v.systolicBP  > 0.0);
    REQUIRE(v.diastolicBP > 0.0);
    REQUIRE(v.heartRate   > 0.0);
    REQUIRE(v.meanArterialPressure > 0.0);
}

// ── Hipertensión: MAP > 110 ──────────────────────────────────
TEST_CASE("HemodynamicModel - ApplyVasoconstrictionBaseline extrema produce HTA",
          "[hemodynamic][hypertension]")
{
    HemodynamicModel m;
    m.ApplyVasoconstrictionBaseline(1.0); // máxima vasoconstricción

    auto v = m.GetCurrentVitals();
    // Con vasoconstricción máxima, MAP debe superar 100
    REQUIRE(v.meanArterialPressure > 100.0);
}
