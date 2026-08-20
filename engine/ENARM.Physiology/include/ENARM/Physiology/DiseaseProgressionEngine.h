#pragma once
// DiseaseProgressionEngine.h — Evolución temporal de enfermedad
// El paciente empeora sin tratamiento, mejora con tratamiento correcto
// ENARM Simulator Pro — Sprint S2
#include <string>
#include <vector>
#include <functional>
#include <chrono>

namespace ENARM::Physiology {

enum class DiseaseStage {
    Compensated,      // Estable, sin tratamiento empeorará
    Decompensating,   // Descompensando
    Critical,         // Crítico — ventana terapéutica estrecha
    Irreversible,     // Daño irreversible
    Recovering,       // Respondiendo a tratamiento
    Resolved          // Resuelto
};

enum class TreatmentResponse {
    None,
    Partial,
    Complete,
    Adverse
};

struct ProgressionEvent {
    double timeMinutes;
    DiseaseStage newStage;
    std::string description;
    std::vector<std::string> newSymptoms;
    std::vector<std::string> resolvedSymptoms;
    bool isCriticalAlert{false};
};

struct TreatmentAction {
    std::string name;
    double administeredAt;   // minutos desde inicio
    TreatmentResponse response{TreatmentResponse::None};
};

class DiseaseProgressionEngine {
public:
    explicit DiseaseProgressionEngine(const std::string& diseaseType);

    // Avanzar tiempo (minutos) — actualiza estado
    void Advance(double minutes);

    // Registrar tratamiento administrado
    TreatmentResponse ApplyTreatment(const std::string& treatment);

    // Estado actual
    DiseaseStage GetCurrentStage() const { return currentStage_; }
    double GetTimeElapsed() const { return timeElapsed_; }
    bool IsPatientAlive() const { return isAlive_; }
    double GetMortalityRisk() const;  // 0-1

    // Eventos pendientes de mostrar
    std::vector<ProgressionEvent> PollEvents();

    // Síntomas activos en este momento
    std::vector<std::string> GetActiveSymptoms() const { return activeSymptoms_; }

    // Signos vitales afectados por progresión
    struct ProgressionVitals {
        double mapDelta{0.0};      // Cambio en MAP (mmHg)
        double hrDelta{0.0};       // Cambio en FC
        double lactateLevel{1.0};  // mmol/L
        double spo2Delta{0.0};     // Cambio SpO2 %
        double tempDelta{0.0};     // Cambio temperatura
    };
    ProgressionVitals GetVitalImpact() const;

    // Reset para nuevo caso
    void Reset();

private:
    std::string diseaseType_;
    DiseaseStage currentStage_{DiseaseStage::Compensated};
    double timeElapsed_{0.0};
    bool isAlive_{true};
    std::vector<std::string> activeSymptoms_;
    std::vector<ProgressionEvent> pendingEvents_;
    std::vector<TreatmentAction> appliedTreatments_;

    // Configuración de progresión por enfermedad
    struct ProgressionConfig {
        double timeToDecompensate{30.0};   // min sin tratamiento
        double timeToCritical{60.0};       // min
        double timeToIrreversible{120.0};  // min
        double mortalityUntreated{0.30};   // 30% sin tratar
        std::vector<std::string> criticalTreatments;
        std::vector<std::string> initialSymptoms;
    };

    ProgressionConfig config_;
    void LoadConfig(const std::string& disease);
    void CheckMilestones();
    bool IsTreatmentCritical(const std::string& treatment) const;
};

} // namespace ENARM::Physiology
