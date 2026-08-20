#include "ENARM/Physiology/MetabolicModel.h"

#include <algorithm>
#include <cmath>

namespace ENARM::Physiology {

    MetabolicModel::MetabolicModel() = default;

    void MetabolicModel::Update(double /*dt_s*/) {
        // Los cambios se aplican via escenarios o setters;
        // aqui se podria integrar con el tiempo en el futuro.
    }

    void MetabolicModel::SimulateSepsis(double severity) {
        // Severidad 0-1
        const double s = std::clamp(severity, 0.0, 1.0);
        m_state.lactate_mmolL = 1.2 + s * 4.0;        // hasta ~5.2 (shock)
        m_state.pH = 7.40 - s * 0.15;                  // hasta ~7.25
        m_state.HCO3_mEqL = 24.0 - s * 8.0;           // hasta ~16
        m_state.K_mEqL = 4.2 + s * 0.6;               // hiperkalemia leve
        
    }

    void MetabolicModel::SimulateDKA(double glucose) {
        m_state.glucose_mgdl = glucose;
        m_state.pH = 7.40 - (glucose > 300.0 ? 0.10 : 0.03);
        m_state.HCO3_mEqL = 24.0 - (glucose > 300.0 ? 8.0 : 2.0);
        m_state.K_mEqL = 5.5;  // hiperkalemia por acidosis
        m_state.Cl_mEqL = 104.0;
        m_state.Na_mEqL = 138.0;
        
    }

    void MetabolicModel::SimulateAcidosis(double pHTarget) {
        m_state.pH = std::clamp(pHTarget, 6.8, 7.45);
        m_state.HCO3_mEqL = 24.0 - (7.40 - m_state.pH) * 20.0;
        if (m_state.HCO3_mEqL < 5.0) m_state.HCO3_mEqL = 5.0;
        
    }

    void MetabolicModel::SimulateHypokalemia(double KTarget) {
        m_state.K_mEqL = std::clamp(KTarget, 1.5, 3.4);
    }

    void MetabolicModel::SimulateHyperkalemia(double KTarget) {
        m_state.K_mEqL = std::clamp(KTarget, 5.1, 8.0);
    }

    void MetabolicModel::ResetToNormal() {
        m_state = MetabolicState{};
    }



}