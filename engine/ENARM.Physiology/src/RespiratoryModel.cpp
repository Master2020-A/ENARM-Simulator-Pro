// ═══════════════════════════════════════════════════════════════
//   ENARM.Physiology - RespiratoryModel v5
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Physiology/RespiratoryModel.h"
#include "ENARM/Physiology/OxygenationModel.h"
#include "ENARM/Physiology/Constants.h"
#include "ENARM/Physiology/NumericalIntegration.h"

#include <cmath>
#include <algorithm>

namespace ENARM::Physiology {

    namespace {
        constexpr double kPi = 3.14159265358979323846;
    }

    RespiratoryModel::RespiratoryModel() = default;
    RespiratoryModel::RespiratoryModel(const RespiratoryParameters& p) : m_params(p) {}

    void RespiratoryModel::SetParameters(const RespiratoryParameters& p) noexcept { m_params = p; }
    void RespiratoryModel::SetRespiratoryRate(double rpm) noexcept {
        m_params.respRate_rpm = std::clamp(rpm, 4.0, 60.0);
    }
    void RespiratoryModel::SetTidalVolume(double mL) noexcept {
        m_params.tidalVolume_mL = std::clamp(mL, 100.0, 2000.0);
    }
    void RespiratoryModel::SetFiO2(double f) noexcept {
        m_params.FiO2 = std::clamp(f, 0.21, 1.0);
    }
    void RespiratoryModel::SetPEEP(double p) noexcept {
        m_params.PEEP_cmH2O = std::clamp(p, 0.0, 25.0);
    }

    // FIX v5: SDRA shunt aumentado (P/F 100-200 en severity=0.9)
    void RespiratoryModel::SimulateARDS(double severity) noexcept {
        const double s = std::clamp(severity, 0.0, 1.0);
        m_params.compliance_factor = 1.0 - 0.7 * s;
        m_params.lungCompliance_mL_cmH2O = 100.0 * m_params.compliance_factor;
        // Shunt hasta 40% (SDRA severo real)
        m_params.shunt_fraction = 0.05 + 0.40 * s;
        m_params.respRate_rpm = 16.0 + 20.0 * s;
        m_params.tidalVolume_mL = 500.0 * (1.0 - 0.3 * s);
    }

    void RespiratoryModel::SimulateCOPD(double severity) noexcept {
        const double s = std::clamp(severity, 0.0, 1.0);
        m_params.resistance_factor = 1.0 + 4.0 * s;
        m_params.airwayResistance_cmH2O_L_s = 2.0 * m_params.resistance_factor;
        m_params.deadSpace_fraction = 0.30 + 0.25 * s;
        m_params.respRate_rpm = 16.0 - 2.0 * s;
        m_params.tidalVolume_mL = 500.0 * (1.0 - 0.15 * s);
    }

    void RespiratoryModel::SimulateAsthma(double severity) noexcept {
        const double s = std::clamp(severity, 0.0, 1.0);
        m_params.resistance_factor = 1.0 + 6.0 * s;
        m_params.airwayResistance_cmH2O_L_s = 2.0 * m_params.resistance_factor;
        m_params.respRate_rpm = 16.0 + 14.0 * s;
    }

    void RespiratoryModel::SimulatePneumonia(double severity) noexcept {
        const double s = std::clamp(severity, 0.0, 1.0);
        m_params.shunt_fraction = 0.05 + 0.20 * s;
        m_params.compliance_factor = 1.0 - 0.4 * s;
        m_params.lungCompliance_mL_cmH2O = 100.0 * m_params.compliance_factor;
        m_params.respRate_rpm = 16.0 + 12.0 * s;
    }

    void RespiratoryModel::ResetToNormal() noexcept {
        m_params = RespiratoryParameters{};
        m_state = RespiratoryState{};
    }

    void RespiratoryModel::UpdateBreathingCycle(DeltaT dt_s) {
        const double T_breath = 60.0 / m_params.respRate_rpm;
        const double T_insp = T_breath * m_params.inspiration_fraction;

        m_state.timeInBreath_s += dt_s;
        if (m_state.timeInBreath_s >= T_breath) {
            m_state.timeInBreath_s -= T_breath;
            m_breathTotalV_mL = m_breathMaxV_mL;
            m_state.tidalVolume_mL = m_breathTotalV_mL;
            m_minuteAccumV_mL += m_breathTotalV_mL;
            ++m_breathsInMinute;
            m_breathMaxV_mL = 0.0;
        }

        m_state.inInspiration = (m_state.timeInBreath_s < T_insp);

        const double VT_target = m_params.tidalVolume_mL;

        if (m_state.inInspiration) {
            const double t = m_state.timeInBreath_s;
            const double V_change = VT_target * 0.5 *
                                    (1.0 - std::cos(kPi * t / T_insp));
            const double V_new = 2400.0 + V_change;
            m_state.airflow_mL_s = (V_new - m_state.alveolarVolume_mL) / dt_s;
            m_state.alveolarVolume_mL = V_new;
        } else {
            const double t_exp = m_state.timeInBreath_s - T_insp;
            const double tau = m_params.airwayResistance_cmH2O_L_s *
                               (m_params.lungCompliance_mL_cmH2O / 1000.0);
            const double V_delta = VT_target * std::exp(-t_exp / std::max(tau, 0.1));
            const double V_new = 2400.0 + V_delta;
            m_state.airflow_mL_s = (V_new - m_state.alveolarVolume_mL) / dt_s;
            m_state.alveolarVolume_mL = V_new;
        }

        if (m_state.alveolarVolume_mL - 2400.0 > m_breathMaxV_mL) {
            m_breathMaxV_mL = m_state.alveolarVolume_mL - 2400.0;
        }

        m_minuteAccum_s += dt_s;
        if (m_minuteAccum_s >= 60.0) {
            m_state.minuteVentilation_L = m_minuteAccumV_mL / 1000.0;
            m_minuteAccum_s = 0.0;
            m_minuteAccumV_mL = 0.0;
            m_breathsInMinute = 0;
        } else {
            m_state.minuteVentilation_L =
                (m_params.tidalVolume_mL * m_params.respRate_rpm) / 1000.0;
        }
    }

    void RespiratoryModel::UpdateGasExchange() {
        // PaCO2
        constexpr double VCO2_normal_mL_min = 250.0;
        constexpr double k_conversion = 0.863;

        const double VE_total = m_state.minuteVentilation_L;
        const double VA_alveolar = VE_total * (1.0 - m_params.deadSpace_fraction);

        if (VA_alveolar > 0.5) {
            const double PaCO2_target = (VCO2_normal_mL_min * k_conversion) / VA_alveolar;
            m_state.PaCO2_mmHg = m_state.PaCO2_mmHg * 0.90 + PaCO2_target * 0.10;
        }
        m_state.PaCO2_mmHg = std::clamp(m_state.PaCO2_mmHg, 15.0, 120.0);

        // PAO2
        constexpr double R = 0.8;
        constexpr double Patm = Constants::kAtmosphericP_mmHg;
        constexpr double PH2O = Constants::kWaterVaporP_mmHg;

        m_state.PACO2_mmHg = m_state.PaCO2_mmHg;
        m_state.PAO2_mmHg = m_params.FiO2 * (Patm - PH2O) - (m_state.PaCO2_mmHg / R);

        // PaO2 con shunt
        const double shunt = m_params.shunt_fraction;

        constexpr double Hb = 15.0;
        constexpr double PvO2_typical = 40.0;
        const double SvO2 = OxygenationModel::CalculateSaO2(PvO2_typical);
        const double CvO2 = Hb * 1.34 * (SvO2 / 100.0) + 0.003 * PvO2_typical;

        const double ScO2 = OxygenationModel::CalculateSaO2(m_state.PAO2_mmHg);
        const double CcO2 = Hb * 1.34 * (ScO2 / 100.0) + 0.003 * m_state.PAO2_mmHg;

        const double CaO2 = CcO2 * (1.0 - shunt) + CvO2 * shunt;

        double SaO2_calc = (CaO2 / (1.34 * Hb)) * 100.0;
        SaO2_calc = std::clamp(SaO2_calc, 0.0, 100.0);

        m_state.PaO2_mmHg = OxygenationModel::CalculatePaO2FromSaO2(
            SaO2_calc, Constants::kNormalpH, m_state.PaCO2_mmHg, Constants::kNormalTempC
        );

        m_state.SaO2_pct = SaO2_calc;
        m_state.SpO2_pct = SaO2_calc;

        m_state.etCO2_mmHg = m_state.PaCO2_mmHg *
                             (1.0 - m_params.deadSpace_fraction * 0.15);
    }

    void RespiratoryModel::Update(DeltaT dt_s) {
        UpdateBreathingCycle(dt_s);
        UpdateGasExchange();
    }

}