// ═══════════════════════════════════════════════════════════════
//   ENARM.Simulation - EventLog Implementation
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Simulation/EventLog.h"

#include <iomanip>
#include <algorithm>

namespace ENARM::Simulation {

    std::string FormatSimTime(double sec) {
        int total = (int)sec;
        int h = total / 3600;
        int m = (total % 3600) / 60;
        int s = total % 60;
        std::ostringstream oss;
        oss << std::setfill('0')
            << std::setw(2) << h << ":"
            << std::setw(2) << m << ":"
            << std::setw(2) << s;
        return oss.str();
    }

    void EventLog::Add(double simTime_s, EventType type,
                       const std::string& category,
                       const std::string& message,
                       int severity) {
        m_events.push_back({simTime_s, type, category, message, severity});
    }

    std::vector<Event> EventLog::GetEventsByType(EventType t) const {
        std::vector<Event> result;
        for (const auto& e : m_events) if (e.type == t) result.push_back(e);
        return result;
    }

    std::vector<Event> EventLog::GetEventsBySeverity(int minSev) const {
        std::vector<Event> result;
        for (const auto& e : m_events) if (e.severity >= minSev) result.push_back(e);
        return result;
    }

    std::string EventLog::ToString() const {
        std::ostringstream oss;
        for (const auto& e : m_events) {
            const char* sev = "  ";
            switch (e.severity) {
                case 1: sev = "! "; break;
                case 2: sev = "!!"; break;
                case 3: sev = "***"; break;
            }
            oss << "[" << FormatSimTime(e.simTime_s) << "] "
                << sev << " [" << e.category << "] " << e.message << "\n";
        }
        return oss.str();
    }

}