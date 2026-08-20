// ═══════════════════════════════════════════════════════════════
//   ENARM.Simulation - EmergencyCode Implementation
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Simulation/EmergencyCode.h"

#include <algorithm>
#include <cctype>

namespace ENARM::Simulation {

    namespace {
        std::string ToLower(std::string s) {
            std::transform(s.begin(), s.end(), s.begin(),
                [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
            return s;
        }

        bool ContainsAny(const std::string& haystack,
                         const std::vector<std::string>& needles) {
            const std::string h = ToLower(haystack);
            for (const auto& n : needles) {
                if (h.find(ToLower(n)) != std::string::npos) return true;
            }
            return false;
        }
    }

    EmergencyCode::EmergencyCode() = default;

    void EmergencyCode::Start(EmergencyCodeType type) {
        m_status = CodeStatus{};
        m_status.type = type;
        m_running = true;
        m_errors.clear();

        switch (type) {
            case EmergencyCodeType::CodeBlue:
                m_status.codeName = "Codigo Azul (Paro Cardiorespiratorio)";
                m_status.timeLimitSeconds = 300.0;  // 5 min
                m_status.maxPoints = 100;
                break;
            case EmergencyCodeType::CodeSTEMI:
                m_status.codeName = "Codigo STEMI (IAM)";
                m_status.timeLimitSeconds = 600.0;  // 10 min (puerta-baloon < 90)
                m_status.maxPoints = 100;
                break;
            case EmergencyCodeType::CodeStroke:
                m_status.codeName = "Codigo Stroke (ACV)";
                m_status.timeLimitSeconds = 600.0;
                m_status.maxPoints = 100;
                break;
            case EmergencyCodeType::CodeTrauma:
                m_status.codeName = "Codigo Trauma (Politraumatizado)";
                m_status.timeLimitSeconds = 900.0;  // 15 min
                m_status.maxPoints = 100;
                break;
            case EmergencyCodeType::CodeSepsis:
                m_status.codeName = "Codigo Sepsis";
                m_status.timeLimitSeconds = 360.0;  // 6 min (bundle 1h -> 60s sim)
                m_status.maxPoints = 100;
                break;
        }
    }

    void EmergencyCode::Update(double dt_s) {
        if (!m_running) return;
        m_status.elapsedSeconds += dt_s;
        if (m_status.TimeUp()) {
            m_status.codeEnded = true;
            m_running = false;
        }
    }

    int EmergencyCode::PointsForAction(const std::string& actionName) const {
        // Puntajes por accion segun el codigo
        switch (m_status.type) {
            case EmergencyCodeType::CodeBlue:
                if (ContainsAny(actionName, {"reanimacion", "rcp", "compresiones", "masaje"})) return 15;
                if (ContainsAny(actionName, {"desfibrilacion", "descarga", "shock"})) return 20;
                if (ContainsAny(actionName, {"epinefrina", "adrenalina"})) return 10;
                if (ContainsAny(actionName, {"amiodarona"})) return 10;
                if (ContainsAny(actionName, {"ventilar", "ambu", "bolsa", "oxigeno"})) return 5;
                break;
            case EmergencyCodeType::CodeSTEMI:
                if (ContainsAny(actionName, {"ecg", "electrocardiograma"})) return 15;
                if (ContainsAny(actionName, {"aspirina", "aas"})) return 10;
                if (ContainsAny(actionName, {"clopidogrel", "ticagrelor", "antiplaquetario"})) return 10;
                if (ContainsAny(actionName, {"heparina"})) return 10;
                if (ContainsAny(actionName, {"reperfusion", "hemodinamia", "trombolisis", "alteplasa"})) return 25;
                if (ContainsAny(actionName, {"nitratos"})) return 5;
                if (ContainsAny(actionName, {"troponina"})) return 5;
                break;
            case EmergencyCodeType::CodeStroke:
                if (ContainsAny(actionName, {"nihss", "escala"})) return 15;
                if (ContainsAny(actionName, {"tac", "tomografia"})) return 20;
                if (ContainsAny(actionName, {"alteplasa", "trombolisis", "tpa"})) return 25;
                if (ContainsAny(actionName, {"glucosa"})) return 5;
                break;
            case EmergencyCodeType::CodeTrauma:
                if (ContainsAny(actionName, {"via aerea", "intubar", "cervical", "collarin"})) return 15;
                if (ContainsAny(actionName, {"faxt", "tactico"})) return 10;
                if (ContainsAny(actionName, {"cristaloides", "fluidos", "ringer", "salina"})) return 10;
                if (ContainsAny(actionName, {"toracostomia", "drenaje", "pleural"})) return 15;
                if (ContainsAny(actionName, {"panta", "hemorragia", "torniquete"})) return 10;
                break;
            case EmergencyCodeType::CodeSepsis:
                if (ContainsAny(actionName, {"hemocultivo"})) return 15;
                if (ContainsAny(actionName, {"lactato"})) return 10;
                if (ContainsAny(actionName, {"antibiotico"})) return 20;
                if (ContainsAny(actionName, {"fluidos", "ringer", "solucion", "cristaloides"})) return 15;
                if (ContainsAny(actionName, {"vasopresor", "norepinefrina"})) return 15;
                break;
        }
        return 0;
    }

    bool EmergencyCode::IsActionExpected(const std::string& actionName, double elapsed) const {
        // Orden de acciones esperado por codigo
        switch (m_status.type) {
            case EmergencyCodeType::CodeBlue:
                return ContainsAny(actionName, {"compresiones", "rcp", "desfibrilacion",
                                                "epinefrina", "ventilar"});
            case EmergencyCodeType::CodeSTEMI:
                if (elapsed < 120.0)
                    return ContainsAny(actionName, {"ecg", "aspirina"});
                return ContainsAny(actionName, {"reperfusion", "hemodinamia", "trombolisis"});
            case EmergencyCodeType::CodeStroke:
                if (elapsed < 180.0)
                    return ContainsAny(actionName, {"nihss", "tac"});
                return ContainsAny(actionName, {"alteplasa", "trombolisis"});
            case EmergencyCodeType::CodeTrauma:
                return ContainsAny(actionName, {"via aerea", "cristaloides", "compresion"});
            case EmergencyCodeType::CodeSepsis:
                return ContainsAny(actionName, {"hemocultivo", "lactato", "antibiotico", "fluidos"});
        }
        return false;
    }

    CodeAction EmergencyCode::PerformAction(const std::string& actionName) {
        CodeAction action;
        action.name = actionName;
        action.timeSeconds = m_status.elapsedSeconds;

        const int pts = PointsForAction(actionName);
        const bool expected = IsActionExpected(actionName, m_status.elapsedSeconds);

        action.correct = (pts > 0 && expected);
        action.points = expected ? pts : 0;

        if (action.correct) {
            action.feedback = "Accion correcta: +" + std::to_string(pts) + " pts.";
            m_status.totalPoints += pts;
            m_status.actions.push_back(action);
        } else if (pts > 0) {
            action.feedback = "Accion valida pero fuera de secuencia.";
            m_status.actions.push_back(action);
        } else {
            action.feedback = "Accion no reconocida para este codigo.";
            m_status.actions.push_back(action);
        }

        // Deteccion de errores graves
        if (m_status.type == EmergencyCodeType::CodeBlue &&
            ContainsAny(actionName, {"epinefrina"}) && m_status.elapsedSeconds > 240.0) {
            m_errors.push_back("Epinefrina administrada demasiado tarde (> 4 min)");
        }
        if (m_status.type == EmergencyCodeType::CodeSTEMI &&
            ContainsAny(actionName, {"trombolisis", "alteplasa"}) &&
            ContainsAny(actionName, {"contraindicacion", "sangrado"})) {
            m_errors.push_back("Trombolisis con contraindicacion");
        }
        if (m_status.type == EmergencyCodeType::CodeStroke &&
            ContainsAny(actionName, {"alteplasa"}) && m_status.elapsedSeconds > 270.0) {
            m_errors.push_back("Alteplasa fuera de ventana terapeutica");
        }

        return action;
    }

    std::vector<std::string> EmergencyCode::SeriousErrors() const {
        return m_errors;
    }

    std::string EmergencyCode::GetCodeName() const {
        return m_status.codeName;
    }

}