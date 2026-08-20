// ═══════════════════════════════════════════════════════════════
//   test_respiratory.cpp
//   Tests del modelo respiratorio y de oxigenación
//
//   Tags: [respiratory] [unit] [physiology]
//
//   Valores normales de referencia:
//   SpO2:   95-100%
//   PaO2:   75-100 mmHg (aire ambiente)
//   PaCO2:  35-45 mmHg
//   EtCO2:  35-45 mmHg
//   FR:     12-20 rpm
// ═══════════════════════════════════════════════════════════════

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "ENARM/Physiology/RespiratoryModel.h"
#include "ENARM/Physiology/OxygenationModel.h"

using namespace Catch::Matchers;

// ── Helper ───────────────────────────────────────────────────────
static ENARM::Physiology::RespiratoryModel MakeNormalRespiratoryModel() {
    ENARM::Physiology::RespiratoryModel model;
    model.Reset();
    return model;
}

// ================================================================
// GRUPO 1: Invariantes físicos respiratorios
// ================================================================

TEST_CASE("Respiratory: SpO2 entre 0 y 100%", "[respiratory][unit][invariant]") {
    auto model = MakeNormalRespiratoryModel();
    model.Step(0.01f);

    auto vitals = model.GetCurrentVitals();

    REQUIRE(vitals.spO2 >= 0.0f);
    REQUIRE(vitals.spO2 <= 100.0f);
}

TEST_CASE("Respiratory: PaCO2 positivo", "[respiratory][unit][invariant]") {
    auto model = MakeNormalRespiratoryModel();
    model.Step(0.01f);

    auto vitals = model.GetCurrentVitals();

    REQUIRE(vitals.paCO2 > 0.0f);
}

TEST_CASE("Respiratory: FR positiva", "[respiratory][unit][invariant]") {
    auto model = MakeNormalRespiratoryModel();
    model.Step(0.01f);

    auto vitals = model.GetCurrentVitals();

    REQUIRE(vitals.respiratoryRate > 0.0f);
}

// ================================================================
// GRUPO 2: Valores normales en reposo
// ================================================================

TEST_CASE("Respiratory: SpO2 normal en reposo (95-100%)", "[respiratory][unit][normal]") {
    auto model = MakeNormalRespiratoryModel();

    for (int i = 0; i < 100; ++i) {
        model.Step(0.01f);
    }

    auto vitals = model.GetCurrentVitals();

    CHECK(vitals.spO2 >= 95.0f);
    CHECK(vitals.spO2 <= 100.0f);
    INFO("SpO2 = " << vitals.spO2 << "%");
}

TEST_CASE("Respiratory: PaCO2 normal en reposo (35-45 mmHg)", "[respiratory][unit][normal]") {
    auto model = MakeNormalRespiratoryModel();

    for (int i = 0; i < 100; ++i) {
        model.Step(0.01f);
    }

    auto vitals = model.GetCurrentVitals();

    CHECK(vitals.paCO2 >= 35.0f);
    CHECK(vitals.paCO2 <= 45.0f);
    INFO("PaCO2 = " << vitals.paCO2 << " mmHg");
}

TEST_CASE("Respiratory: FR normal en reposo (12-20 rpm)", "[respiratory][unit][normal]") {
    auto model = MakeNormalRespiratoryModel();

    for (int i = 0; i < 100; ++i) {
        model.Step(0.01f);
    }

    auto vitals = model.GetCurrentVitals();

    CHECK(vitals.respiratoryRate >= 12.0f);
    CHECK(vitals.respiratoryRate <= 20.0f);
    INFO("FR = " << vitals.respiratoryRate << " rpm");
}

// ================================================================
// GRUPO 3: Estabilidad numérica
// ================================================================

TEST_CASE("Respiratory: Sin NaN después de 1000 ticks", "[respiratory][unit][stability]") {
    auto model = MakeNormalRespiratoryModel();

    for (int i = 0; i < 1000; ++i) {
        model.Step(0.01f);
    }

    auto vitals = model.GetCurrentVitals();

    REQUIRE_FALSE(std::isnan(vitals.spO2));
    REQUIRE_FALSE(std::isnan(vitals.paCO2));
    REQUIRE_FALSE(std::isnan(vitals.paO2));
    REQUIRE_FALSE(std::isnan(vitals.respiratoryRate));
    REQUIRE_FALSE(std::isinf(vitals.spO2));
}
