// ═══════════════════════════════════════════════════════════════
//   ENARM.Physiology - Hemodinamica v6
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/Physiology/PhysiologyTypes.h"

namespace ENARM::Physiology {

    struct HemodynamicParameters {
        double C_compliance     {1.75};
        double Rp_baseline      {1.063};   // Basal del paciente (shock lo modifica)
        double Rp_drug_factor   {1.0};     // Factor por farmacos (RESET cada tick)
        double Rc_charImpedance {0.033};
        double L_inertance      {0.0005};

        double heartRate_bpm    {75.0};
        double strokeVolume_mL  {70.0};
        double sv_drug_factor   {1.0};     // Factor SV por farmacos (RESET cada tick)
        double hr_drug_factor   {1.0};     // Factor HR por farmacos (RESET cada tick)
        double ejectionFraction {0.60};

        // Rp efectiva: baseline * drug
        [[nodiscard]] double GetEffectiveRp() const noexcept {
            return Rp_baseline * Rp_drug_factor;
        }

        [[nodiscard]] double GetEffectiveHR() const noexcept {
            return heartRate_bpm * hr_drug_factor;
        }

        [[nodiscard]] double GetEffectiveSV() const noexcept {
            return strokeVolume_mL * sv_drug_factor;
        }
    };

    class HemodynamicModel {
    public:
        HemodynamicModel();
        explicit HemodynamicModel(const HemodynamicParameters& params);

        void Update(DeltaT dt_s);

        void SetParameters(const HemodynamicParameters& params) noexcept;
        void SetHeartRate(double bpm) noexcept;
        void SetStrokeVolume(double mL) noexcept;

        // ─── Baseline changes (patologia) ───
        void SimulateShock(double severity) noexcept;
        // Shock septico hiperdinamico: SV mantenido (PP amplia),
        // vasodilatacion severa -74%, compliance casi normal
        void SimulateHypertension(double severity) noexcept;
        void ResetToNormal() noexcept;

        // ─── Baseline drugs (bolus, cambios lentos) ───
        void ApplyVasoconstrictionBaseline(double factor) noexcept;
        // Vasoconstriction: curva sigmoide suave (NA calibrada)
        //   factor 1.2 -> +11% Rp ; 1.4 -> +20% ; 2.0 -> +41% ; 3.0 -> +63%
        void ApplyVasodilationBaseline(double factor) noexcept;
        void ApplyInotropyBaseline(double factor) noexcept;
        void ApplyChronotropyBaseline(double factor) noexcept;

        // ─── Drug effects (para bridge - se aplican cada tick, sobrescriben) ───
        // effectMagnitude en [0,1]. maxFactor = valor cuando magnitude=1
        void SetDrugVasoEffect(double effectMagnitude, double maxFactor) noexcept;
        void SetDrugInotropyEffect(double effectMagnitude, double maxFactor) noexcept;
        void SetDrugChronotropyEffect(double effectMagnitude, double maxFactor) noexcept;

        // Reset de factores de drogas (llamar al inicio de cada tick del bridge)
        void ResetDrugEffects() noexcept;

        [[nodiscard]] const HemodynamicState& GetState() const noexcept { return m_state; }
        [[nodiscard]] const HemodynamicParameters& GetParameters() const noexcept { return m_params; }

        [[nodiscard]] double GetSystolicBP() const noexcept { return m_state.systolicBP_mmHg; }
        [[nodiscard]] double GetDiastolicBP() const noexcept { return m_state.diastolicBP_mmHg; }
        [[nodiscard]] double GetMAP() const noexcept { return m_state.meanArterialP_mmHg; }
        [[nodiscard]] double GetCardiacOutput() const noexcept { return m_state.cardiacOutput_Lmin; }
        [[nodiscard]] double GetPulsePressure() const noexcept { return m_state.pulsePressure_mmHg; }

    private:
        [[nodiscard]] double VentricularOutflow(double t_in_cycle_s) const noexcept;
        [[nodiscard]] double SystoleDuration() const noexcept;
        void OnCycleComplete() noexcept;

        HemodynamicParameters m_params;
        HemodynamicState m_state;

        double m_cycleMaxP{0.0};
        double m_cycleMinP{300.0};
        double m_cycleSumP{0.0};
        int    m_cycleSamples{0};
    };

}