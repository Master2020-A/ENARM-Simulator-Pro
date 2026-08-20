// ============================================================
//  test_metabolic.cpp - Placeholders Fase 2
// ============================================================
#include <catch2/catch_all.hpp>

TEST_CASE("MetabolicModel - pH normal 7.35-7.45", "[metabolic][placeholder]")
{
    double pH = 7.40;
    REQUIRE(pH >= 7.35);
    REQUIRE(pH <= 7.45);
}

TEST_CASE("MetabolicModel - lactato normal < 2.0", "[metabolic][placeholder]")
{
    double lactate = 1.2;
    REQUIRE(lactate < 2.0);
    REQUIRE(lactate > 0.0);
}

TEST_CASE("MetabolicModel - anion gap normal", "[metabolic][placeholder]")
{
    double Na  = 140.0;
    double Cl  = 104.0;
    double HCO3 = 24.0;
    double ag  = Na - (Cl + HCO3);
    REQUIRE(ag >= 8.0);
    REQUIRE(ag <= 16.0);
}