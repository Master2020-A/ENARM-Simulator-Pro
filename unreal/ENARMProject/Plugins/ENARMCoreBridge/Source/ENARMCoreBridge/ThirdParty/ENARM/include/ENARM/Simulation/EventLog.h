// ═══════════════════════════════════════════════════════════════
//   ENARM.Simulation - Event log
// ═══════════════════════════════════════════════════════════════
#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <sstream>

namespace ENARM::Simulation {

    enum class EventType : uint8_t {
        Scenario,           // Inicio de escenario
        Medication,         // Farmaco administrado
        Vitals,             // Cambio significativo en signos vitales
        Alarm,              // Alarma activada
        Intervention,       // Procedimiento realizado
        Assessment,         // Evaluacion clinica
        Improvement,        // Mejora clinica detectada
        Deterioration,      // Deterioro clinico
        Info                // Informativo
    };

    struct Event {
        double simTime_s{0.0};      // Tiempo simulado
        EventType type{EventType::Info};
        std::string category;       // "hemodynamic", "respiratory", "drug", ...
        std::string message;
        int severity{0};            // 0=info, 1=warn, 2=alert, 3=critical
    };

    class EventLog {
    public:
        void Add(double simTime_s, EventType type,
                 const std::string& category,
                 const std::string& message,
                 int severity = 0);

        [[nodiscard]] const std::vector<Event>& GetEvents() const noexcept { return m_events; }
        [[nodiscard]] std::vector<Event> GetEventsByType(EventType t) const;
        [[nodiscard]] std::vector<Event> GetEventsBySeverity(int minSeverity) const;

        [[nodiscard]] std::string ToString() const;
        void Clear() noexcept { m_events.clear(); }

    private:
        std::vector<Event> m_events;
    };

    // Helper: formatea tiempo como HH:MM:SS
    std::string FormatSimTime(double sec);

}