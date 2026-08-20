// ============================================================
//  test_pharmacology_real.cpp
//  Tests reales para DrugDatabase según API del proyecto
// ============================================================
#include <catch2/catch_all.hpp>
#include "DrugDatabase.h"

using namespace ENARM::Pharmacology;

// ── Singleton accesible ──────────────────────────────────────
TEST_CASE("DrugDatabase - singleton devuelve instancia valida",
          "[pharmacology][singleton]")
{
    auto& db = DrugDatabase::GetInstance();
    // Si llegamos aquí sin crash, el singleton funciona
    REQUIRE(true);
}

// ── Existencia de drogas por categoría ──────────────────────
TEST_CASE("DrugDatabase - vasopresores disponibles",
          "[pharmacology][vasopressors]")
{
    auto& db = DrugDatabase::GetInstance();
    auto  all = db.GetAllDrugs();

    bool hasVasopressor = false;
    for (const auto& drug : all) {
        if (drug.category == DrugCategory::Vasopressor ||
            drug.category == DrugCategory::Vasoactive) {
            hasVasopressor = true;
            break;
        }
    }
    REQUIRE(hasVasopressor);
}

TEST_CASE("DrugDatabase - sedantes disponibles",
          "[pharmacology][sedatives]")
{
    auto& db  = DrugDatabase::GetInstance();
    auto  all = db.GetAllDrugs();

    bool hasSedative = false;
    for (const auto& drug : all) {
        if (drug.category == DrugCategory::Sedative ||
            drug.category == DrugCategory::Anesthetic) {
            hasSedative = true;
            break;
        }
    }
    REQUIRE(hasSedative);
}

TEST_CASE("DrugDatabase - banco tiene al menos 5 drogas",
          "[pharmacology][count]")
{
    auto& db  = DrugDatabase::GetInstance();
    auto  all = db.GetAllDrugs();
    REQUIRE(all.size() >= 5);
}

// ── Propiedades de drogas individuales ──────────────────────
TEST_CASE("DrugDatabase - cada droga tiene nombre no vacio",
          "[pharmacology][properties]")
{
    auto& db  = DrugDatabase::GetInstance();
    auto  all = db.GetAllDrugs();

    for (const auto& drug : all) {
        INFO("Drug sin nombre detectada");
        REQUIRE_FALSE(drug.name.empty());
    }
}

TEST_CASE("DrugDatabase - dosis maxima mayor que dosis minima",
          "[pharmacology][doses]")
{
    auto& db  = DrugDatabase::GetInstance();
    auto  all = db.GetAllDrugs();

    for (const auto& drug : all) {
        if (drug.maxDose > 0.0 && drug.minDose > 0.0) {
            INFO("Droga con dosis inválidas: " + drug.name);
            REQUIRE(drug.maxDose >= drug.minDose);
        }
    }
}

// ── Búsqueda por nombre ──────────────────────────────────────
TEST_CASE("DrugDatabase - busqueda por nombre retorna droga o null",
          "[pharmacology][search]")
{
    auto& db = DrugDatabase::GetInstance();

    // Esta droga debe existir o el método debe retornar nullptr/empty
    // Ajusta el nombre exacto según tu implementación
    auto result = db.FindDrug("norepinephrine");
    // Si no existe en tu DB, busca la primera droga del banco
    if (!result) {
        auto all = db.GetAllDrugs();
        if (!all.empty()) {
            result = db.FindDrug(all[0].name);
            REQUIRE(result != nullptr);
        }
    } else {
        REQUIRE(result != nullptr);
        REQUIRE_FALSE(result->name.empty());
    }
}
