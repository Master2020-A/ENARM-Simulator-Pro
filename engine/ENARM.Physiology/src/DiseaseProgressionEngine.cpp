// DiseaseProgressionEngine.cpp
// ENARM Simulator Pro — Sprint S2
#include "ENARM/Physiology/DiseaseProgressionEngine.h"
#include <algorithm>
#include <cmath>

namespace ENARM::Physiology {

DiseaseProgressionEngine::DiseaseProgressionEngine(const std::string& diseaseType)
    : diseaseType_(diseaseType)
{
    LoadConfig(diseaseType);
}

void DiseaseProgressionEngine::LoadConfig(const std::string& disease) {
    // Configuración por enfermedad
    if (disease == "SepticShock" || disease == "Sepsis") {
        config_.timeToDecompensate  = 30.0;
        config_.timeToCritical      = 60.0;
        config_.timeToIrreversible  = 120.0;
        config_.mortalityUntreated  = 0.30;
        config_.criticalTreatments  = {
            "antibiotics", "fluids", "norepinephrine",
            "vasopressors", "cultures", "lactate"
        };
        config_.initialSymptoms = {
            "fiebre", "taquicardia", "hipotension",
            "confusion", "oliguria"
        };
    } else if (disease == "IAMCEST" || disease == "STEMI") {
        config_.timeToDecompensate  = 20.0;
        config_.timeToCritical      = 45.0;
        config_.timeToIrreversible  = 90.0;
        config_.mortalityUntreated  = 0.25;
        config_.criticalTreatments  = {
            "aspirina", "clopidogrel", "heparina",
            "fibrinolisis", "angioplastia", "AAS"
        };
        config_.initialSymptoms = {
            "dolor toracico", "diaforesis",
            "nausea", "disnea"
        };
    } else if (disease == "Stroke" || disease == "EVC") {
        config_.timeToDecompensate  = 15.0;
        config_.timeToCritical      = 30.0;
        config_.timeToIrreversible  = 60.0;  // "tiempo es cerebro"
        config_.mortalityUntreated  = 0.20;
        config_.criticalTreatments  = {
            "tPA", "trombectomia", "alteplase",
            "TC", "imagen"
        };
        config_.initialSymptoms = {
            "hemiplejia", "afasia", "desviacion ocular",
            "cefalea", "vomito"
        };
    } else {
        // Default generico
        config_.timeToDecompensate  = 45.0;
        config_.timeToCritical      = 90.0;
        config_.timeToIrreversible  = 180.0;
        config_.mortalityUntreated  = 0.15;
    }

    activeSymptoms_ = config_.initialSymptoms;
}

void DiseaseProgressionEngine::Advance(double minutes) {
    if (!isAlive_) return;

    timeElapsed_ += minutes;
    CheckMilestones();
}

void DiseaseProgressionEngine::CheckMilestones() {
    // Contar tratamientos críticos aplicados
    int criticalTreatmentsApplied = 0;
    for (const auto& action : appliedTreatments_) {
        if (IsTreatmentCritical(action.name)) {
            criticalTreatmentsApplied++;
        }
    }

    bool hasTreatment = criticalTreatmentsApplied >= 2;

    // Transiciones de estado
    if (currentStage_ == DiseaseStage::Compensated) {
        double threshold = hasTreatment
            ? config_.timeToDecompensate * 2.0   // con tratamiento: doble tiempo
            : config_.timeToDecompensate;

        if (timeElapsed_ >= threshold) {
            currentStage_ = DiseaseStage::Decompensating;
            ProgressionEvent ev;
            ev.timeMinutes    = timeElapsed_;
            ev.newStage       = DiseaseStage::Decompensating;
            ev.description    = "Paciente se está descompensando";
            ev.isCriticalAlert = false;
            pendingEvents_.push_back(ev);
        }
    }

    if (currentStage_ == DiseaseStage::Decompensating) {
        double threshold = hasTreatment
            ? config_.timeToCritical * 1.5
            : config_.timeToCritical;

        if (timeElapsed_ >= threshold) {
            currentStage_ = DiseaseStage::Critical;
            ProgressionEvent ev;
            ev.timeMinutes    = timeElapsed_;
            ev.newStage       = DiseaseStage::Critical;
            ev.description    = "Estado crítico — ventana terapéutica estrecha";
            ev.isCriticalAlert = true;
            pendingEvents_.push_back(ev);
        }
    }

    if (currentStage_ == DiseaseStage::Critical && !hasTreatment) {
        if (timeElapsed_ >= config_.timeToIrreversible) {
            currentStage_ = DiseaseStage::Irreversible;
            ProgressionEvent ev;
            ev.timeMinutes    = timeElapsed_;
            ev.newStage       = DiseaseStage::Irreversible;
            ev.description    = "Daño irreversible establecido";
            ev.isCriticalAlert = true;
            pendingEvents_.push_back(ev);
        }
    }

    // Recuperación con tratamiento adecuado
    if (hasTreatment && criticalTreatmentsApplied >= 3) {
        if (currentStage_ == DiseaseStage::Decompensating ||
            currentStage_ == DiseaseStage::Critical) {
            currentStage_ = DiseaseStage::Recovering;
        }
    }
}

TreatmentResponse DiseaseProgressionEngine::ApplyTreatment(const std::string& treatment) {
    TreatmentAction action;
    action.name             = treatment;
    action.administeredAt   = timeElapsed_;

    TreatmentResponse response = TreatmentResponse::None;

    if (IsTreatmentCritical(treatment)) {
        response         = TreatmentResponse::Complete;
        action.response  = response;

        // Aplicar beneficio inmediato si es tratamiento correcto
        if (currentStage_ == DiseaseStage::Compensated ||
            currentStage_ == DiseaseStage::Decompensating) {
            // Retrasar progresión
        }
    } else {
        response        = TreatmentResponse::Partial;
        action.response = response;
    }

    appliedTreatments_.push_back(action);

    // Re-evaluar estado con tratamiento
    CheckMilestones();

    return response;
}

double DiseaseProgressionEngine::GetMortalityRisk() const {
    int criticalApplied = 0;
    for (const auto& action : appliedTreatments_) {
        if (IsTreatmentCritical(action.name)) criticalApplied++;
    }

    double baseRisk = config_.mortalityUntreated;

    // Reducir riesgo por tratamientos
    double treatmentBenefit = std::min(0.8, criticalApplied * 0.15);
    double risk = baseRisk * (1.0 - treatmentBenefit);

    // Aumentar riesgo por progresión tardía
    switch (currentStage_) {
        case DiseaseStage::Compensated:    break;
        case DiseaseStage::Decompensating: risk *= 1.5;  break;
        case DiseaseStage::Critical:       risk *= 2.5;  break;
        case DiseaseStage::Irreversible:   risk *= 4.0;  break;
        case DiseaseStage::Recovering:     risk *= 0.5;  break;
        case DiseaseStage::Resolved:       risk  = 0.02; break;
    }

    return std::clamp(risk, 0.0, 1.0);
}

std::vector<ProgressionEvent> DiseaseProgressionEngine::PollEvents() {
    auto events = pendingEvents_;
    pendingEvents_.clear();
    return events;
}

DiseaseProgressionEngine::ProgressionVitals
DiseaseProgressionEngine::GetVitalImpact() const {
    ProgressionVitals vitals;

    switch (currentStage_) {
        case DiseaseStage::Compensated:
            vitals.mapDelta     =  0.0;
            vitals.hrDelta      =  5.0;
            vitals.lactateLevel =  1.5;
            break;
        case DiseaseStage::Decompensating:
            vitals.mapDelta     = -15.0;
            vitals.hrDelta      =  20.0;
            vitals.lactateLevel =  3.0;
            vitals.spo2Delta    =  -3.0;
            break;
        case DiseaseStage::Critical:
            vitals.mapDelta     = -30.0;
            vitals.hrDelta      =  35.0;
            vitals.lactateLevel =  6.0;
            vitals.spo2Delta    =  -8.0;
            break;
        case DiseaseStage::Irreversible:
            vitals.mapDelta     = -45.0;
            vitals.hrDelta      =  50.0;
            vitals.lactateLevel = 10.0;
            vitals.spo2Delta    = -15.0;
            break;
        case DiseaseStage::Recovering:
            vitals.mapDelta     = -5.0;
            vitals.hrDelta      =  10.0;
            vitals.lactateLevel =  2.0;
            break;
        case DiseaseStage::Resolved:
            vitals.lactateLevel = 1.0;
            break;
    }

    return vitals;
}

void DiseaseProgressionEngine::Reset() {
    currentStage_       = DiseaseStage::Compensated;
    timeElapsed_        = 0.0;
    isAlive_            = true;
    pendingEvents_.clear();
    appliedTreatments_.clear();
    LoadConfig(diseaseType_);
}

bool DiseaseProgressionEngine::IsTreatmentCritical(const std::string& treatment) const {
    std::string tLower = treatment;
    std::transform(tLower.begin(), tLower.end(), tLower.begin(), ::tolower);

    for (const auto& ct : config_.criticalTreatments) {
        std::string ctLower = ct;
        std::transform(ctLower.begin(), ctLower.end(), ctLower.begin(), ::tolower);
        if (tLower == ctLower || tLower.find(ctLower) != std::string::npos) {
            return true;
        }
    }
    return false;
}

} // namespace ENARM::Physiology
