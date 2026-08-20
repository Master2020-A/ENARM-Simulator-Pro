// ═══════════════════════════════════════════════════════════════
//   ENARM.Physiology - Respiratorio
// ═══════════════════════════════════════════════════════════════
//
// Basado en:
//   Otis AB, Fenn WO, Rahn H. "Mechanics of breathing in man"
//   J Appl Physiol. 1950;2(11):592-607.
//
// Ecuacion del movimiento respiratorio:
//   P_musc = E*V + R*dV/dt + I*d2V/dt2
//   (Ignoramos I por ser despreciable en respiracion espontanea normal)
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/Physiology/PhysiologyTypes.h"

namespace ENARM::Physiology {

    struct RespiratoryParameters {
        // Mecanica pulmonar
        double lungCompliance_mL_cmH2O   {100.0};  // Cl
        double airwayResistance_cmH2O_L_s{2.0};    // Raw
        double chestWallCompliance       {200.0};  // Cw

        // Patron respiratorio
        double respRate_rpm              {16.0};   // FR
        double tidalVolume_mL            {500.0};  // VT deseado
        double inspiration_fraction      {0.33};   // Ti/Ttot (I:E = 1:2)

        // Fraccion inspirada de O2
        double FiO2                      {0.21};   // Aire ambiente
        double PEEP_cmH2O                {0.0};    // 0 en respiracion espontanea

        // Modificadores patologicos
        double compliance_factor         {1.0};    // <1 = fibrosis, SDRA
        double resistance_factor         {1.0};    // >1 = asma, EPOC
        double shunt_fraction            {0.05};   // Qs/Qt fisiologico ~5%
        double deadSpace_fraction        {0.30};   // Vd/Vt fisiologico ~30%
    };

    // ═══════════════════════════════════════════════════════════════
    class RespiratoryModel {
    public:
        RespiratoryModel();
        explicit RespiratoryModel(const RespiratoryParameters& params);

        void Update(DeltaT dt_s);

        void SetParameters(const RespiratoryParameters& params) noexcept;
        void SetRespiratoryRate(double rpm) noexcept;
        void SetTidalVolume(double mL) noexcept;
        void SetFiO2(double fraction) noexcept;
        void SetPEEP(double cmH2O) noexcept;

        // Escenarios
        void SimulateARDS(double severity) noexcept;      // 0..1
        void SimulateCOPD(double severity) noexcept;
        void SimulateAsthma(double severity) noexcept;
        void SimulatePneumonia(double severity) noexcept;
        void ResetToNormal() noexcept;

        // Getters
        [[nodiscard]] const RespiratoryState& GetState() const noexcept { return m_state; }
        [[nodiscard]] const RespiratoryParameters& GetParameters() const noexcept { return m_params; }

        [[nodiscard]] double GetSpO2() const noexcept { return m_state.SpO2_pct; }
        [[nodiscard]] double GetPaO2() const noexcept { return m_state.PaO2_mmHg; }
        [[nodiscard]] double GetPaCO2() const noexcept { return m_state.PaCO2_mmHg; }
        [[nodiscard]] double GetEtCO2() const noexcept { return m_state.etCO2_mmHg; }
        [[nodiscard]] double GetMinuteVent() const noexcept { return m_state.minuteVentilation_L; }

    private:
        void UpdateBreathingCycle(DeltaT dt_s);
        void UpdateGasExchange();

        RespiratoryParameters m_params;
        RespiratoryState m_state;

        // Para stats por respiracion
        double m_breathMaxV_mL{0.0};
        double m_breathTotalV_mL{0.0};
        int    m_breathsInMinute{0};
        double m_minuteAccum_s{0.0};
        double m_minuteAccumV_mL{0.0};
    };

}