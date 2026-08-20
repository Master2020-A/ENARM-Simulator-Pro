#include "ENARM/Simulation/DiseaseProgressionEngine.h"

#include <algorithm>
#include <cmath>

namespace ENARM::Simulation {

    DiseaseProgressionEngine::DiseaseProgressionEngine() = default;

    void DiseaseProgressionEngine::Configure(ScenarioType type) {
        m_type = type;
        m_phase = ProgressionPhase::Stable;
        m_deterioration = 0.0;
        m_improvement = 0.0;
        m_lastMap = 93.0;
        m_lastSpO2 = 98.0;

        switch (type) {
            case ScenarioType::SepticShock:
                m_targets = {65.0, 90.0, 130.0, 35.0,
                             true, true, true, false, "Shock septico"};
                break;
            case ScenarioType::AcuteMI_STEMI:
                m_targets = {65.0, 90.0, 110.0, 28.0,
                             false, false, false, true, "IAMCEST"};
                break;
            case ScenarioType::CardiogenicShock:
                m_targets = {65.0, 90.0, 120.0, 30.0,
                             false, false, true, true, "Shock cardiogenico"};
                break;
            case ScenarioType::HypovolemicShock:
                m_targets = {65.0, 90.0, 140.0, 30.0,
                             false, true, true, false, "Shock hipovolemico"};
                break;
            case ScenarioType::SevereARDS:
                m_targets = {65.0, 88.0, 130.0, 35.0,
                             false, false, false, false, "SDRA"};
                break;
            case ScenarioType::SevereAsthma:
                m_targets = {65.0, 90.0, 130.0, 40.0,
                             false, false, false, false, "Asma severo"};
                break;
            default:
                m_targets = {65.0, 90.0, 130.0, 35.0,
                             false, false, false, false, "Patologia"};
                break;
        }
    }

    void DiseaseProgressionEngine::Update(double dt_s,
                                          const Physiology::HemodynamicModel& heart,
                                          bool fluidResuscitationGiven,
                                          bool vasopressorsActive,
                                          bool antibioticsGiven,
                                          bool reperfusionGiven) {
        const double map = heart.GetMAP();
        const double hr = heart.GetParameters().heartRate_bpm;
        // SpO2 se pasa via RespiratoryModel en integracion futura;
        // aqui usamos MAP y FC del modelo hemodinamico.

        // Deterioro: si el paciente NO recibe tratamiento adecuado
        double deteriorationRate = 0.0;
        if (m_type != ScenarioType::HealthyBaseline) {
            if (map < m_targets.mapMin) deteriorationRate += 0.35;
            if (hr > m_targets.hrMax) deteriorationRate += 0.15;

            // Tratamiento faltante
            if (m_targets.needsFluids && !fluidResuscitationGiven) deteriorationRate += 0.30;
            if (m_targets.needsVasopressors && !vasopressorsActive) deteriorationRate += 0.25;
            if (m_targets.needsAntibiotics && !antibioticsGiven) deteriorationRate += 0.20;
            if (m_targets.needsReperfusion && !reperfusionGiven) deteriorationRate += 0.35;
        }

        // Mejoria: si el paciente recibe tratamiento y los signos mejoran
        double improvementRate = 0.0;
        if (map >= m_targets.mapMin) {
            improvementRate += 0.5;
        }
        if (vasopressorsActive && map >= m_targets.mapMin) improvementRate += 0.3;
        if (fluidResuscitationGiven && map >= m_targets.mapMin) improvementRate += 0.2;

        // Integrar en el tiempo
        m_deterioration = std::max(0.0, m_deterioration + deteriorationRate * dt_s / 60.0);
        m_improvement = std::max(0.0, m_improvement + improvementRate * dt_s / 60.0);

        // La mejora contrarresta el deterioro
        m_deterioration = std::max(0.0, m_deterioration - m_improvement * 0.3);
        m_improvement = std::max(0.0, m_improvement - m_deterioration * 0.1);

        m_lastMap = map;

        UpdatePhase();
    }

    void DiseaseProgressionEngine::UpdatePhase() {
        if (m_deterioration >= 8.0) {
            m_phase = ProgressionPhase::Critical;
        } else if (m_deterioration >= 4.0) {
            m_phase = ProgressionPhase::Decompensating;
        } else if (m_deterioration >= 1.0) {
            m_phase = ProgressionPhase::Compensated;
        } else {
            m_phase = ProgressionPhase::Stable;
        }
    }

    std::string DiseaseProgressionEngine::PhaseToString() const {
        switch (m_phase) {
            case ProgressionPhase::Stable:         return "Estable";
            case ProgressionPhase::Compensated:    return "Compensado";
            case ProgressionPhase::Decompensating: return "Descompensando";
            case ProgressionPhase::Critical:       return "CRITICO";
        }
        return "Desconocido";
    }

    std::vector<std::string> DiseaseProgressionEngine::MissingTreatments() const {
        std::vector<std::string> missing;
        if (m_targets.needsAntibiotics)   missing.push_back("Antibioticos");
        if (m_targets.needsFluids)        missing.push_back("Fluidos IV");
        if (m_targets.needsVasopressors)  missing.push_back("Vasopresores");
        if (m_targets.needsReperfusion)   missing.push_back("Reperfusion");
        return missing;
    }

}