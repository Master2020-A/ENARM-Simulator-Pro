// ═══════════════════════════════════════════════════════════════
//   ENARM.Pharmacology - DrugAdministration Implementation
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Pharmacology/DrugAdministration.h"
#include <algorithm>
#include <cctype>

namespace ENARM::Pharmacology {

    namespace {
        std::string ToLower(std::string s) {
            std::transform(s.begin(), s.end(), s.begin(),
                [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
            return s;
        }
    }

    DrugAdministration::DrugAdministration(double patientWeight_kg)
        : m_weight_kg(patientWeight_kg) {}

    bool DrugAdministration::GiveBolus(const std::string& drugName, double dose_mg) {
        auto drugOpt = DrugDatabase::Instance().FindByName(drugName);
        if (!drugOpt) return false;

        const auto& params = *drugOpt;
        const std::string key = ToLower(drugName);

        // Crear estado si no existe
        if (m_activeDrugs.find(key) == m_activeDrugs.end()) {
            DrugState s;
            s.drugName = params.name;
            m_activeDrugs[key] = s;
            m_drugParams[key] = params;
        }

        PharmacokineticModel::ApplyIVPush(m_activeDrugs[key], params, dose_mg);
        return true;
    }

    bool DrugAdministration::StartInfusion(const std::string& drugName, double rate_mcg_kg_min) {
        auto drugOpt = DrugDatabase::Instance().FindByName(drugName);
        if (!drugOpt) return false;

        const auto& params = *drugOpt;
        const std::string key = ToLower(drugName);

        if (m_activeDrugs.find(key) == m_activeDrugs.end()) {
            DrugState s;
            s.drugName = params.name;
            m_activeDrugs[key] = s;
            m_drugParams[key] = params;
        }

        // Convertir mcg/kg/min a mg/hora
        // rate mg/h = rate_mcg_kg_min * weight_kg * 60 / 1000
        const double rate_mg_h = rate_mcg_kg_min * m_weight_kg * 60.0 / 1000.0;
        PharmacokineticModel::StartInfusion(m_activeDrugs[key], rate_mg_h);
        return true;
    }

    bool DrugAdministration::AdjustInfusion(const std::string& drugName, double newRate_mcg_kg_min) {
        return StartInfusion(drugName, newRate_mcg_kg_min);
    }

    bool DrugAdministration::StopInfusion(const std::string& drugName) {
        const std::string key = ToLower(drugName);
        auto it = m_activeDrugs.find(key);
        if (it == m_activeDrugs.end()) return false;
        PharmacokineticModel::StopInfusion(it->second);
        return true;
    }

    void DrugAdministration::Update(double dt_s) {
        for (auto& [key, state] : m_activeDrugs) {
            auto pIt = m_drugParams.find(key);
            if (pIt == m_drugParams.end()) continue;
            PharmacokineticModel::UpdateDrugState(state, pIt->second, m_weight_kg, dt_s);
        }
    }

    const DrugState* DrugAdministration::GetState(const std::string& drugName) const {
        const std::string key = ToLower(drugName);
        auto it = m_activeDrugs.find(key);
        return (it != m_activeDrugs.end()) ? &it->second : nullptr;
    }

    std::vector<std::string> DrugAdministration::GetActiveDrugs() const {
        std::vector<std::string> result;
        for (const auto& [key, state] : m_activeDrugs) {
            if (state.plasmaConcentration_mcgmL > 0.001 || state.isInfusing) {
                result.push_back(state.drugName);
            }
        }
        return result;
    }

    DrugAdministration::CombinedEffects DrugAdministration::GetCombinedEffects() const {
        CombinedEffects e;
        for (const auto& [key, state] : m_activeDrugs) {
            e.alpha1   += state.alpha1_effect;
            e.beta1    += state.beta1_effect;
            e.beta2    += state.beta2_effect;
            e.dopamine += state.dopamine_effect;
            e.gaba     += state.gaba_effect;
            e.opioid   += state.opioid_effect;
        }
        return e;
    }

}