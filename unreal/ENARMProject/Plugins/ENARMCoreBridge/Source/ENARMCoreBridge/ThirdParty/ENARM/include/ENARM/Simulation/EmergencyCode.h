// ═══════════════════════════════════════════════════════════════
//   ENARM.Simulation - EmergencyCode (Sprint 16)
//   Codigos criticos con cronometros de decision:
//   Azul (paro), STEMI, Stroke, Trauma, Sepsis
// ═══════════════════════════════════════════════════════════════
#pragma once

#include <string>
#include <vector>

namespace ENARM::Simulation {

    enum class EmergencyCodeType : uint8_t {
        CodeBlue,        // Paro cardiorespiratorio
        CodeSTEMI,       // IAM con elevacion del ST
        CodeStroke,      // ACV isquemico agudo
        CodeTrauma,      // Politraumatizado
        CodeSepsis       // Sepsis severa
    };

    // Accion del estudiante durante el codigo
    struct CodeAction {
        std::string name;
        double      timeSeconds{0.0};  // Tiempo desde inicio del codigo
        bool        correct{false};
        int         points{0};
        std::string feedback;
    };

    // Estado del codigo en curso
    struct CodeStatus {
        EmergencyCodeType type{EmergencyCodeType::CodeBlue};
        std::string codeName;
        double      elapsedSeconds{0.0};
        double      timeLimitSeconds{600.0};  // 10 min por defecto
        bool        patientSaved{false};
        bool        codeEnded{false};
        int         totalPoints{0};
        int         maxPoints{0};
        std::vector<CodeAction> actions;

        [[nodiscard]] double TimeRemaining() const { return timeLimitSeconds - elapsedSeconds; }
        [[nodiscard]] bool TimeUp() const { return elapsedSeconds >= timeLimitSeconds; }
    };

    class EmergencyCode {
    public:
        EmergencyCode();

        // Iniciar un codigo
        void Start(EmergencyCodeType type);
        void Update(double dt_s);

        // Accion del estudiante (devuelve feedback)
        CodeAction PerformAction(const std::string& actionName);

        // Consultas
        [[nodiscard]] const CodeStatus& GetStatus() const noexcept { return m_status; }
        [[nodiscard]] bool IsRunning() const noexcept { return m_running; }
        [[nodiscard]] std::string GetCodeName() const;

        // Deteccion de errores graves
        [[nodiscard]] std::vector<std::string> SeriousErrors() const;

    private:
        bool IsActionExpected(const std::string& actionName, double elapsed) const;
        int  PointsForAction(const std::string& actionName) const;

        CodeStatus m_status;
        bool m_running{false};
        std::vector<std::string> m_errors;
    };

}