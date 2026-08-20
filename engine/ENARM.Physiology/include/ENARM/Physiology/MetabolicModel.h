// ═══════════════════════════════════════════════════════════════
//   ENARM.Physiology - MetabolicModel
//   Balance acido-base, electrolitos, lactato, glucosa
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/Physiology/PhysiologyTypes.h"

namespace ENARM::Physiology {

    // Estado metabolico del paciente
    struct MetabolicState {
        // Acido-base
        double pH{7.40};             // 7.35-7.45
        double HCO3_mEqL{24.0};     // 22-28
        double PaCO2_mmHg{40.0};    // 35-45
        double baseExcess{0.0};     // -2 a +2

        // Electrolitos
        double Na_mEqL{140.0};      // 135-145
        double K_mEqL{4.2};         // 3.5-5.0
        double Cl_mEqL{104.0};      // 98-106

        // Metabolica
        double glucose_mgdl{95.0};  // 70-110 ayunas
        double lactate_mmolL{1.2};  // < 2.0 normal

        [[nodiscard]] double AnionGap() const noexcept {
            return Na_mEqL - (Cl_mEqL + HCO3_mEqL);
        }
    };

    class MetabolicModel {
    public:
        MetabolicModel();

        void Update(double dt_s);

        // ─── Escenarios ───
        void SimulateSepsis(double severity);       // lactato alto, pH bajo
        void SimulateDKA(double glucose);          // cetoacidosis diabetica
        void SimulateAcidosis(double pHTarget);    // acidosis metabolica
        void SimulateHypokalemia(double KTarget);  // hipokalemia
        void SimulateHyperkalemia(double KTarget); // hiperkalemia
        void ResetToNormal();

        // ─── Acceso ───
        [[nodiscard]] const MetabolicState& GetState() const noexcept { return m_state; }

        [[nodiscard]] double GetPH() const noexcept { return m_state.pH; }
        [[nodiscard]] double GetLactate() const noexcept { return m_state.lactate_mmolL; }
        [[nodiscard]] double GetK() const noexcept { return m_state.K_mEqL; }
        [[nodiscard]] double GetGlucose() const noexcept { return m_state.glucose_mgdl; }
        [[nodiscard]] double GetAnionGap() const noexcept { return m_state.AnionGap(); }

    private:
        MetabolicState m_state;
    };

}