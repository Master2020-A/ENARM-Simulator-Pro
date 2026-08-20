// ═══════════════════════════════════════════════════════════════
//   ENARM.Pharmacology - Administrador de farmacos por paciente
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/Pharmacology/PharmacokineticModel.h"
#include "ENARM/Pharmacology/DrugDatabase.h"

#include <unordered_map>
#include <vector>

namespace ENARM::Pharmacology {

    // ═══════════════════════════════════════════════════════════════
    //  Administra multiples farmacos a un paciente
    // ═══════════════════════════════════════════════════════════════
    class DrugAdministration {
    public:
        explicit DrugAdministration(double patientWeight_kg);

        // Dar bolo IV
        // dose_mg puede calcularse desde mg/kg usando el peso
        bool GiveBolus(const std::string& drugName, double dose_mg);

        // Iniciar infusion continua (rate en mcg/kg/min)
        bool StartInfusion(const std::string& drugName, double rate_mcg_kg_min);

        // Ajustar velocidad de infusion
        bool AdjustInfusion(const std::string& drugName, double newRate_mcg_kg_min);

        // Detener infusion
        bool StopInfusion(const std::string& drugName);

        // Actualizar todos los farmacos activos
        void Update(double dt_s);

        // Consultar estado
        [[nodiscard]] const DrugState* GetState(const std::string& drugName) const;
        [[nodiscard]] std::vector<std::string> GetActiveDrugs() const;

        // Efectos totales sumados de todos los farmacos activos
        struct CombinedEffects {
            double alpha1{0.0};       // Vasoconstriccion
            double beta1{0.0};        // Inotropia/cronotropia
            double beta2{0.0};        // Broncodilatacion
            double dopamine{0.0};     // Renal
            double gaba{0.0};         // Sedacion
            double opioid{0.0};       // Analgesia/depresion respiratoria
        };
        [[nodiscard]] CombinedEffects GetCombinedEffects() const;

        [[nodiscard]] double GetPatientWeight() const noexcept { return m_weight_kg; }

    private:
        double m_weight_kg;
        std::unordered_map<std::string, DrugState> m_activeDrugs;
        std::unordered_map<std::string, DrugParameters> m_drugParams;
    };

}