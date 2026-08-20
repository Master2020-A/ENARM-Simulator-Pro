// â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•
//   ENARM.GUI - Application (main loop)
// â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•
#pragma once

#include "ENARM/GUI/GuiTypes.h"
#include "ENARM/GUI/AppContext.h"
#include "ENARM/Common/Result.h"

#include <memory>

namespace ENARM::GUI {

    class Application {
    public:
        // Forward decl publico para permitir acceso al WndProc
        struct Impl;

        Application();
        ~Application();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        Common::Result<void> Initialize(const WindowConfig& config = {});
        int Run();
        void Shutdown();

        [[nodiscard]] AppContext& GetContext() noexcept { return m_context; }

    private:
        void RenderFrame();
        void RenderMainMenuBar();
        void RenderDockspace();
        void RenderPlaceholderPanels();
        void RenderStatusBar();
        void RenderLoadingScreen();
        void ConnectServices();

        std::unique_ptr<Impl> m_impl;
        AppContext m_context;
        bool m_running{false};
        WindowConfig m_config;
    };

}
