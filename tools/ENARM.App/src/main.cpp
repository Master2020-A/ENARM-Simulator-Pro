// ═══════════════════════════════════════════════════════════════
//   ENARM.App - Punto de entrada
// ═══════════════════════════════════════════════════════════════
#include <windows.h>

#include "ENARM/GUI/Application.h"
#include "ENARM/Common/Logger.h"

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
    ENARM::Common::LoggerConfig logCfg;
    logCfg.appName = "enarm_app";
    logCfg.enableFile = true;
    logCfg.enableConsole = false;
    logCfg.asyncMode = true;
    ENARM::Common::Logger::Initialize(logCfg);

    ENARM_LOG_INFO("=== ENARM App iniciando ===");

    ENARM::GUI::Application app;

    ENARM::GUI::WindowConfig wcfg;
    wcfg.title     = "ENARM Simulator Pro";
    wcfg.width     = 1600;
    wcfg.height    = 900;
    wcfg.maximized = true;
    wcfg.vsync     = true;

    auto initRes = app.Initialize(wcfg);
    if (initRes.IsErr()) {
        MessageBoxA(nullptr, initRes.Error().message.c_str(),
                    "ENARM - Error inicializacion", MB_ICONERROR);
        ENARM_LOG_CRITICAL("Init failed: {}", initRes.Error().message);
        return 1;
    }

    int ret = app.Run();
    ENARM_LOG_INFO("=== ENARM App terminando ===");
    ENARM::Common::Logger::Shutdown();
    return ret;
}