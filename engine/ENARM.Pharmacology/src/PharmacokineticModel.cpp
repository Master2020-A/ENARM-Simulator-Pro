// ═══════════════════════════════════════════════════════════════
//   ENARM.Pharmacology - PK Model Implementation
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Pharmacology/PharmacokineticModel.h"

#include <cmath>
#include <algorithm>

namespace ENARM::Pharmacology {

    void PharmacokineticModel::UpdateDrugState(
        DrugState& state,
        const DrugParameters& params,
        double patientWeight_kg,
        double dt_s)
    {
        const double dt_min = dt_s / 60.0;

        // ─── Infusion continua: agregar farmaco al compartimento central ───
        if (state.isInfusing && state.infusionRate_mgh > 0.0) {
            const double rate_mg_per_min = state.infusionRate_mgh / 60.0;
            state.amountCentral_mg += rate_mg_per_min * dt_min;
        }

        // ─── EDOs del modelo 2 compartimentos ───
        // dA1/dt = -(k10 + k12) * A1 + k21 * A2
        // dA2/dt = k12 * A1 - k21 * A2
        //
        // Metodo: Euler (suficiente para dt=10ms)

        const double A1 = state.amountCentral_mg;
        const double A2 = state.amountPeripheral_mg;

        const double dA1 = (-(params.k10_per_min + params.k12_per_min) * A1
                            + params.k21_per_min * A2) * dt_min;
        const double dA2 = (params.k12_per_min * A1
                            - params.k21_per_min * A2) * dt_min;

        state.amountCentral_mg    = std::max(0.0, A1 + dA1);
        state.amountPeripheral_mg = std::max(0.0, A2 + dA2);

        // ─── Concentracion plasmatica ───
        // Cp = A1 / Vc (mg / L => * 1000 = mcg/mL)
        const double Vc_L = params.Vc_L_kg * patientWeight_kg;
        if (Vc_L > 0.0) {
            state.plasmaConcentration_mcgmL = state.amountCentral_mg / Vc_L;
            // 1 mg/L = 1 mcg/mL exacto
        } else {
            state.plasmaConcentration_mcgmL = 0.0;
        }

        // ─── Calcular efectos sobre receptores ───
        // Reset efectos
        state.alpha1_effect = 0.0;
        state.beta1_effect = 0.0;
        state.beta2_effect = 0.0;
        state.dopamine_effect = 0.0;
        state.gaba_effect = 0.0;
        state.opioid_effect = 0.0;

        for (const auto& eff : params.effects) {
            const double magnitude = CalculateReceptorEffect(
                state.plasmaConcentration_mcgmL,
                eff.EC50_mcgmL,
                eff.Hill_coefficient,
                eff.Emax_potency
            );
            const double signed_mag = eff.isAgonist ? magnitude : -magnitude;

            switch (eff.receptor) {
                case Receptor::Alpha1:    state.alpha1_effect   += signed_mag; break;
                case Receptor::Beta1:     state.beta1_effect    += signed_mag; break;
                case Receptor::Beta2:     state.beta2_effect    += signed_mag; break;
                case Receptor::Dopamine1: state.dopamine_effect += signed_mag; break;
                case Receptor::GABA_A:    state.gaba_effect     += signed_mag; break;
                case Receptor::MuOpioid:  state.opioid_effect   += signed_mag; break;
                default: break;
            }
        }

        state.timeSinceLastDose_min += dt_min;
    }

    void PharmacokineticModel::ApplyIVPush(
        DrugState& state,
        const DrugParameters& params,
        double dose_mg)
    {
        // Bolo IV: toda la dosis va al compartimento central instantaneamente
        state.amountCentral_mg += dose_mg;
        state.timeSinceLastDose_min = 0.0;
        if (state.drugName.empty()) state.drugName = params.name;
    }

    void PharmacokineticModel::StartInfusion(DrugState& state, double rate_mg_per_hour) {
        state.infusionRate_mgh = rate_mg_per_hour;
        state.isInfusing = true;
    }

    void PharmacokineticModel::StopInfusion(DrugState& state) {
        state.infusionRate_mgh = 0.0;
        state.isInfusing = false;
    }

    double PharmacokineticModel::CalculateReceptorEffect(
        double Cp, double EC50, double Hill, double Emax) noexcept
    {
        if (Cp <= 0.0 || EC50 <= 0.0) return 0.0;
        const double num = std::pow(Cp, Hill);
        const double den = std::pow(EC50, Hill) + num;
        return Emax * (num / den);
    }

    double PharmacokineticModel::CalculateSteadyStateConcentration(
        double infusionRate_mg_per_min, double clearance_mL_per_min) noexcept
    {
        if (clearance_mL_per_min <= 0.0) return 0.0;
        return (infusionRate_mg_per_min * 1000.0) / clearance_mL_per_min;
    }

}