// ═══════════════════════════════════════════════════════════════
//   ENARM.Pharmacology - Modelo PK/PD 2 compartimentos
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/Pharmacology/DrugParameters.h"
#include "ENARM/Physiology/PhysiologyTypes.h"

namespace ENARM::Pharmacology {

    // Estado de un farmaco en el paciente
    struct DrugState {
        std::string drugName;

        // Cantidades (mg)
        double amountCentral_mg{0.0};      // A1
        double amountPeripheral_mg{0.0};   // A2

        // Concentracion plasmatica actual (mcg/mL)
        double plasmaConcentration_mcgmL{0.0};

        // Efecto actual (0-1) por receptor
        double alpha1_effect{0.0};
        double beta1_effect{0.0};
        double beta2_effect{0.0};
        double dopamine_effect{0.0};
        double gaba_effect{0.0};
        double opioid_effect{0.0};

        // Infusion continua activa
        double infusionRate_mgh{0.0};      // 0 si no hay infusion
        bool isInfusing{false};

        // Tiempo desde ultima dosis
        double timeSinceLastDose_min{0.0};
    };

    // ═══════════════════════════════════════════════════════════════
    class PharmacokineticModel {
    public:
        // Actualiza estado del farmaco un paso dt (segundos)
        // patientWeight_kg: peso del paciente
        static void UpdateDrugState(
            DrugState& state,
            const DrugParameters& params,
            double patientWeight_kg,
            double dt_s
        );

        // Aplicar dosis IV push (bolo)
        static void ApplyIVPush(
            DrugState& state,
            const DrugParameters& params,
            double dose_mg
        );

        // Iniciar/actualizar infusion continua
        static void StartInfusion(
            DrugState& state,
            double rate_mg_per_hour
        );

        static void StopInfusion(DrugState& state);

        // Calcular efecto sobre receptor especifico (0-1)
        // Usando ecuacion de Hill: E = Emax * C^n / (EC50^n + C^n)
        [[nodiscard]] static double CalculateReceptorEffect(
            double plasmaConcentration_mcgmL,
            double EC50,
            double Hill,
            double Emax
        ) noexcept;

        // Concentracion en steady state con infusion
        // Css = R / Cl  (R en mg/min, Cl en mL/min)
        [[nodiscard]] static double CalculateSteadyStateConcentration(
            double infusionRate_mg_per_min,
            double clearance_mL_per_min
        ) noexcept;
    };

}