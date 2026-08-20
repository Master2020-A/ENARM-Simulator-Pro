// ═══════════════════════════════════════════════════════════════
//   ENARM.Simulation - Bridge v2 (sin acumulacion)
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Simulation/PhysiologyPharmacologyBridge.h"

#include <algorithm>
#include <cmath>

namespace ENARM::Simulation {

    double PhysiologyPharmacologyBridge::EffectToFactor(double effect, double maxFactor) {
        return 1.0 + effect * (maxFactor - 1.0);
    }

    void PhysiologyPharmacologyBridge::ApplyPharmacologicalEffects(
        Physiology::HemodynamicModel& heart,
        Physiology::RespiratoryModel& lungs,
        const Pharmacology::DrugAdministration& drugs)
    {
        // PASO 1: Reset factores farmacologicos previos
        heart.ResetDrugEffects();

        const auto e = drugs.GetCombinedEffects();

        // ─── ALFA-1: Vasoconstriccion ───
        // magnitude en [0,1], sobrescribe cada tick
        double vasoMagnitude = std::clamp(e.alpha1, -1.0, 1.0);
        heart.SetDrugVasoEffect(vasoMagnitude, 2.0);

        // ─── BETA-1: Inotropia + cronotropia ───
        double inoMagnitude = std::clamp(e.beta1, -1.0, 1.0);
        heart.SetDrugInotropyEffect(inoMagnitude, 1.5);

        // Cronotropia: beta1 positivo sube HR, opioides bajan
        double chronoMag = std::clamp(e.beta1 - e.opioid * 0.3, -1.0, 1.0);
        heart.SetDrugChronotropyEffect(chronoMag, 1.5);

        // ─── BETA-2: Vasodilatacion adicional ───
        if (e.beta2 > 0.1) {
            // Ligera reduccion adicional de Rp
            const double curr = heart.GetParameters().Rp_drug_factor;
            heart.SetDrugVasoEffect(vasoMagnitude - e.beta2 * 0.2, 2.0);
        }

        // ─── OPIOIDES + GABA: Depresion respiratoria ───
        // Efecto sobre FR pero SIN acumular. Base = 16 rpm, opioides bajan
        // Reconstruir FR desde efectos combinados
        const double baseFR = 16.0;   // asumido; luego lo hacemos parametrico
        double resp_depression = 0.0;
        if (e.opioid > 0.05) resp_depression += e.opioid * 0.7;   // opioides deprimen fuerte
        if (e.gaba > 0.1)    resp_depression += e.gaba * 0.3;     // sedantes leve

        resp_depression = std::clamp(resp_depression, 0.0, 0.75);   // max 75% reduccion

        // Solo modificar si hay efecto significativo
        if (resp_depression > 0.05) {
            const double newFR = baseFR * (1.0 - resp_depression);
            const double currFR = lungs.GetParameters().respRate_rpm;
            // Aproximar hacia el objetivo (respuesta gradual)
            const double blendFR = currFR * 0.98 + newFR * 0.02;
            lungs.SetRespiratoryRate(std::clamp(blendFR, 4.0, 40.0));
        }
    }

}