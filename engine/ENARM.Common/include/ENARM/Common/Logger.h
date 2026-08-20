// ═══════════════════════════════════════════════════════════════
//   ENARM.Common - Logger (spdlog wrapper)
// ═══════════════════════════════════════════════════════════════
#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>
#include <memory>
#include <string>
#include <string_view>
#include <filesystem>

namespace ENARM::Common {

    enum class LogLevel {
        Trace, Debug, Info, Warn, Error, Critical, Off
    };

    struct LoggerConfig {
        std::filesystem::path logDirectory{"logs"};
        std::string appName{"enarm"};
        LogLevel consoleLevel{LogLevel::Info};
        LogLevel fileLevel{LogLevel::Debug};
        bool enableConsole{true};
        bool enableFile{true};
        bool asyncMode{true};
        size_t maxFileSizeMB{50};
        size_t maxFiles{10};
    };

    class Logger {
    public:
        static void Initialize(const LoggerConfig& config = {});
        static void Shutdown();
        static std::shared_ptr<spdlog::logger> Get(std::string_view name = "core");
        static void SetLevel(LogLevel level);
        static bool IsInitialized() noexcept;

    private:
        static spdlog::level::level_enum ToSpdlogLevel(LogLevel level) noexcept;
        static bool s_initialized;
        static LoggerConfig s_config;
    };

}

#define ENARM_LOG_TRACE(...)    ::ENARM::Common::Logger::Get()->trace(__VA_ARGS__)
#define ENARM_LOG_DEBUG(...)    ::ENARM::Common::Logger::Get()->debug(__VA_ARGS__)
#define ENARM_LOG_INFO(...)     ::ENARM::Common::Logger::Get()->info(__VA_ARGS__)
#define ENARM_LOG_WARN(...)     ::ENARM::Common::Logger::Get()->warn(__VA_ARGS__)
#define ENARM_LOG_ERROR(...)    ::ENARM::Common::Logger::Get()->error(__VA_ARGS__)
#define ENARM_LOG_CRITICAL(...) ::ENARM::Common::Logger::Get()->critical(__VA_ARGS__)