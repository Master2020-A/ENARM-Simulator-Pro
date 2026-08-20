// ═══════════════════════════════════════════════════════════════
//   ENARM.Simulation - SurgicalProcedure (Sprint 15)
//   Procedimientos quirurgicos basicos con arbol de decision:
//   Apendicectomia, Colecistectomia, Herniorrafia, Cesarea,
//   Toracostomia, Traqueostomia, Sutura, Drenaje absceso, RCP
// ═══════════════════════════════════════════════════════════════
#pragma once

#include <string>
#include <vector>

namespace ENARM::Simulation {

    // Tipos de procedimiento
    enum class ProcedureType : uint8_t {
        Appendectomy,        // Apendicectomia
        LapCholecystectomy,  // Colecistectomia laparoscopica
        Herniorrhaphy,       // Herniorrafia inguinal
        CesareanSection,     // Cesarea
        Thoracostomy,        // Toracostomia (drenaje pleural)
        Tracheostomy,        // Traqueostomia
        WoundSuture,         // Sutura de heridas
        AbscessDrainage,     // Drenaje de abscesos
        AdvancedCPR         // RCP avanzado
    };

    // Paso del procedimiento
    struct ProcedureStep {
        std::string name;
        std::string description;
        std::string instrument;     // Instrumento requerido
        std::vector<std::string> correctActions;
        std::vector<std::string> wrongActions;      // Errores criticos
        int points{10};
        bool isCritical{false};     // Si se omite, paciente complica
        bool completed{false};
    };

    // Estado del procedimiento
    struct ProcedureStatus {
        ProcedureType type{ProcedureType::Appendectomy};
        std::string procedureName;
        std::vector<ProcedureStep> steps;
        size_t currentStep{0};
        int totalPoints{0};
        int maxPoints{0};
        bool completed{false};
        bool patientStable{true};
        std::vector<std::string> complications;
        double elapsedSeconds{0.0};
        double timeLimitSeconds{900.0};

        [[nodiscard]] bool IsFinished() const { return currentStep >= steps.size(); }
    };

    class SurgicalProcedure {
    public:
        SurgicalProcedure();

        // Iniciar procedimiento
        void Start(ProcedureType type);
        void Update(double dt_s);

        // Accion del estudiante
        bool PerformAction(const std::string& actionName);
        void NextStep();

        // Consultas
        [[nodiscard]] const ProcedureStatus& GetStatus() const noexcept { return m_status; }
        [[nodiscard]] bool IsRunning() const noexcept { return m_running; }
        [[nodiscard]] std::string GetProcedureName() const;

        // Instrumentos disponibles en el campo
        [[nodiscard]] static std::vector<std::string> AvailableInstruments(ProcedureType type);

    private:
        void BuildSteps(ProcedureType type);
        bool IsCorrectForStep(const std::string& action) const;

        ProcedureStatus m_status;
        bool m_running{false};
    };

}