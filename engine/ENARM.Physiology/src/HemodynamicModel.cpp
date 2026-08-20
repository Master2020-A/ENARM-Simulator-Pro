// ═══════════════════════════════════════════════════════════════
//   ENARM.Physiology - HemodynamicModel v6
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Physiology/HemodynamicModel.h"
#include "ENARM/Physiology/Constants.h"
#include "ENARM/Physiology/NumericalIntegration.h"

#include <cmath>
#include <algorithm>

namespace ENARM::Physiology {

    namespace {
        constexpr double kPi = 3.14159265358979323846;
    }

    HemodynamicModel::HemodynamicModel() = default;
    HemodynamicModel::HemodynamicModel(const HemodynamicParameters& p) : m_params(p) {}

    void HemodynamicModel::SetParameters(const HemodynamicParameters& p) noexcept { m_params = p; }

    void HemodynamicModel::SetHeartRate(double bpm) noexcept {
        m_params.heartRate_bpm = std::clamp(bpm, 20.0, 250.0);
    }

    void HemodynamicModel::SetStrokeVolume(double mL) noexcept {
        m_params.strokeVolume_mL = std::clamp(mL, 10.0, 200.0);
    }

    // ═══════ BASELINE CHANGES (patologia, se aplican una vez) ═══════

    void HemodynamicModel::SimulateShock(double severity) noexcept {
        const double s = std::clamp(severity, 0.0, 1.0);
        // FIX v6: Shock septico HIPERDINAMICO (fase temprana real)
        // - Taquicardia compensadora
        // - SV mantenido o aumentado (no deprimido) => PP amplia (no estrecha)
        // - Vasodilatacion severa (mas profunda que v5) para mantener MAP shock
        // - Compliance casi normal (PP amplia, tipica de sepsis hiperdinamica)
        m_params.heartRate_bpm  = 75.0 + 50.0 * s;
        m_params.strokeVolume_mL = 70.0 * (1.0 + 0.15 * s);
        m_params.Rp_baseline    = Constants::kPeripheralResistance_mmHg_s_mL *
                                   (1.0 - 0.74 * s);
        m_params.C_compliance   = Constants::kArterialCompliance_mL_mmHg *
                                   (1.0 - 0.05 * s);
    }

    void HemodynamicModel::SimulateHypertension(double severity) noexcept {
        const double s = std::clamp(severity, 0.0, 1.0);
        m_params.Rp_baseline = Constants::kPeripheralResistance_mmHg_s_mL * (1.0 + 0.6 * s);
        m_params.C_compliance = Constants::kArterialCompliance_mL_mmHg * (1.0 - 0.35 * s);
    }

    void HemodynamicModel::ResetToNormal() noexcept {
        m_params = HemodynamicParameters{};
        m_state = HemodynamicState{};
        m_cycleMaxP = 0.0;
        m_cycleMinP = 300.0;
        m_cycleSumP = 0.0;
        m_cycleSamples = 0;
    }

    // ═══════ BASELINE DRUG APPLICATION (bolus, cambios lentos) ═══════

    void HemodynamicModel::ApplyVasoconstrictionBaseline(double factor) noexcept {
        // FIX v6: NA calibrada a curva sigmoide suave (no lineal agresiva)
        // factor 1.0 -> efecto 1.00 (neutral)
        // factor 1.2 -> efecto 1.11 (NA baja dosis: +11% Rp)
        // factor 1.4 -> efecto 1.20 (NA moderada: +20% Rp -> MAP 80-85 sobre shock)
        // factor 2.0 -> efecto 1.41 (NA alta: +41% Rp)
        // factor 3.0 -> efecto 1.63 (saturado)
        const double f = std::clamp(factor, 0.5, 3.0);
        double effect;
        if (f >= 1.0) {
            const double dx = f - 1.0;
            // Sigmoide suave: pendiente 0.6, asintota ~1.85
            effect = 1.0 + 0.6 * dx / (1.0 + 0.45 * dx);
        } else {
            effect = f;
        }
        m_params.Rp_baseline *= effect;
        m_params.Rp_baseline = std::clamp(m_params.Rp_baseline, 0.2, 3.0);
    }

    void HemodynamicModel::ApplyVasodilationBaseline(double factor) noexcept {
        m_params.Rp_baseline /= std::clamp(factor, 0.5, 2.0);
        m_params.Rp_baseline = std::clamp(m_params.Rp_baseline, 0.2, 3.0);
    }

    void HemodynamicModel::ApplyInotropyBaseline(double factor) noexcept {
        m_params.strokeVolume_mL *= std::clamp(factor, 0.3, 2.5);
        m_params.strokeVolume_mL = std::clamp(m_params.strokeVolume_mL, 10.0, 180.0);
    }

    void HemodynamicModel::ApplyChronotropyBaseline(double factor) noexcept {
        m_params.heartRate_bpm *= std::clamp(factor, 0.3, 2.5);
        m_params.heartRate_bpm = std::clamp(m_params.heartRate_bpm, 30.0, 200.0);
    }

    // ═══════ DRUG EFFECTS TICK-BASED (SOBRESCRIBEN) ═══════
    // effectMagnitude en [0,1], maxFactor = valor cuando magnitude=1
    // Ejemplo: alpha1 effect=0.5 con maxFactor=2.0 -> drug_factor = 1 + 0.5*(2-1) = 1.5

    void HemodynamicModel::SetDrugVasoEffect(double magnitude, double maxFactor) noexcept {
        magnitude = std::clamp(magnitude, -1.0, 1.0);
        // Positivo = vasoconstriccion (drug_factor > 1)
        // Negativo = vasodilatacion (drug_factor < 1)
        double factor;
        if (magnitude >= 0) {
            factor = 1.0 + magnitude * (maxFactor - 1.0);
        } else {
            factor = 1.0 / (1.0 + (-magnitude) * (maxFactor - 1.0));
        }
        m_params.Rp_drug_factor = std::clamp(factor, 0.3, 3.0);
    }

    void HemodynamicModel::SetDrugInotropyEffect(double magnitude, double maxFactor) noexcept {
        magnitude = std::clamp(magnitude, -1.0, 1.0);
        double factor;
        if (magnitude >= 0) {
            factor = 1.0 + magnitude * (maxFactor - 1.0);
        } else {
            factor = 1.0 / (1.0 + (-magnitude) * (maxFactor - 1.0));
        }
        m_params.sv_drug_factor = std::clamp(factor, 0.3, 2.5);
    }

    void HemodynamicModel::SetDrugChronotropyEffect(double magnitude, double maxFactor) noexcept {
        magnitude = std::clamp(magnitude, -1.0, 1.0);
        double factor;
        if (magnitude >= 0) {
            factor = 1.0 + magnitude * (maxFactor - 1.0);
        } else {
            factor = 1.0 / (1.0 + (-magnitude) * (maxFactor - 1.0));
        }
        m_params.hr_drug_factor = std::clamp(factor, 0.3, 2.5);
    }

    void HemodynamicModel::ResetDrugEffects() noexcept {
        m_params.Rp_drug_factor = 1.0;
        m_params.sv_drug_factor = 1.0;
        m_params.hr_drug_factor = 1.0;
    }

    // ═══════ SIMULATION ═══════

    double HemodynamicModel::SystoleDuration() const noexcept {
        const double hr = m_params.GetEffectiveHR();
        const double T_cycle = 60.0 / std::max(hr, 20.0);
        double frac = 0.30 + 0.15 * std::min(1.0, hr / 150.0);
        return T_cycle * frac;
    }

    double HemodynamicModel::VentricularOutflow(double t_in_cycle_s) const noexcept {
        const double Ts = SystoleDuration();
        if (t_in_cycle_s > Ts) return 0.0;
        const double SV = m_params.GetEffectiveSV();
        if (SV <= 0.0) return 0.0;
        const double Q_peak = SV * kPi / (2.0 * Ts);
        return Q_peak * std::sin(kPi * t_in_cycle_s / Ts);
    }

    void HemodynamicModel::OnCycleComplete() noexcept {
        if (m_cycleSamples > 0) {
            m_state.systolicBP_mmHg    = m_cycleMaxP;
            m_state.diastolicBP_mmHg   = m_cycleMinP;
            m_state.pulsePressure_mmHg = m_cycleMaxP - m_cycleMinP;
            m_state.meanArterialP_mmHg = m_cycleSumP / m_cycleSamples;
            m_state.cardiacOutput_Lmin =
                (m_params.GetEffectiveSV() * m_params.GetEffectiveHR()) / 1000.0;
        }
        m_cycleMaxP = 0.0;
        m_cycleMinP = 300.0;
        m_cycleSumP = 0.0;
        m_cycleSamples = 0;
    }

    void HemodynamicModel::Update(DeltaT dt_s) {
        const double hr = m_params.GetEffectiveHR();
        if (hr < 10.0) {
            // Paro cardiaco: no hay flujo
            m_state.aorticFlow_mL_s = 0.0;
            m_state.systolicBP_mmHg = 0.0;
            m_state.diastolicBP_mmHg = 0.0;
            m_state.meanArterialP_mmHg = 0.0;
            m_state.pulsePressure_mmHg = 0.0;
            m_state.cardiacOutput_Lmin = 0.0;
            return;
        }

        const double T_cycle = 60.0 / hr;

        m_state.timeInCycle_s += dt_s;
        if (m_state.timeInCycle_s >= T_cycle) {
            m_state.timeInCycle_s -= T_cycle;
            OnCycleComplete();
        }

        const double Q_vent = VentricularOutflow(m_state.timeInCycle_s);
        m_state.aorticFlow_mL_s = Q_vent;

        const double Rp = m_params.GetEffectiveRp();
        auto dPdt = [&](double /*t*/, double P) {
            return (Q_vent - P / Rp) / m_params.C_compliance;
        };

        m_state.aorticPressure_mmHg = RK4(
            0.0, m_state.aorticPressure_mmHg, dt_s, dPdt
        );

        // Clamp de seguridad para evitar explosion numerica
        m_state.aorticPressure_mmHg = std::clamp(m_state.aorticPressure_mmHg, 0.0, 300.0);

        const double P_display = m_state.aorticPressure_mmHg +
                                  Q_vent * m_params.Rc_charImpedance;

        if (P_display > m_cycleMaxP) m_cycleMaxP = P_display;
        if (P_display < m_cycleMinP) m_cycleMinP = P_display;
        m_cycleSumP += P_display;
        ++m_cycleSamples;
    }

}