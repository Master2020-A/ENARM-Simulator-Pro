// â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•
//   ENARM.GUI - Tipos comunes (con navegacion)
// â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•
#pragma once

#include <string>
#include <cstdint>

namespace ENARM::GUI {

    enum class VitalStatus : uint8_t {
        Normal, Warning, Critical, Emergency
    };

    struct WindowConfig {
        std::string title{"ENARM Simulator Pro"};
        int width{1600};
        int height{900};
        bool maximized{true};
        bool vsync{true};
    };

    // Vista activa (navegacion)
    enum class AppView : uint8_t {
        Loading,           // Splash inicial
        Dashboard,         // Hero + acciones rapidas
        CasesList,         // Lista de casos clinicos
        PatientSession,    // Chat + vitals + acciones
        SessionSummary,    // Resumen post-sesion
        GuidelinesList,    // Sprint 7A.5: Lista de GPCs
        GuidelineDetail,   // Sprint 7A.5: Detalle GPC
        EmergencyCode,     // Sprint 16: Codigo de emergencia
        ECGMonitor,        // Sprint 12.5: Monitor ECG en tiempo real
        Settings
    };

    // Mantener AppMode como alias por compatibilidad
    using AppMode = AppView;

}