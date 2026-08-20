// ═══════════════════════════════════════════════════════════════
//   ENARM.Common - Logger Implementation
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Common/Logger.h"

#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

#include <mutex>
#include <unordered_map>
#include <vector>

namespace ENARM::Common {

    bool Logger::s_initialized = false;
    LoggerConfig Logger::s_config{};

    namespace {
        std::mutex g_mutex;
        std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> g_loggers;
    }

    spdlog::level::level_enum Logger::ToSpdlogLevel(LogLevel level) noexcept {
        switch (level) {
            case LogLevel::Trace:    return spdlog::level::trace;
            case LogLevel::Debug:    return spdlog::level::debug;
            case LogLevel::Info:     return spdlog::level::info;
            case LogLevel::Warn:     return spdlog::level::warn;
            case LogLevel::Error:    return spdlog::level::err;
            case LogLevel::Critical: return spdlog::level::critical;
            case LogLevel::Off:      return spdlog::level::off;
        }
        return spdlog::level::info;
    }

    void Logger::Initialize(const LoggerConfig& config) {
        std::lock_guard lock(g_mutex);
        if (s_initialized) return;

        s_config = config;

        if (config.enableFile) {
            std::error_code ec;
            std::filesystem::create_directories(config.logDirectory, ec);
        }

        if (config.asyncMode) {
            spdlog::init_thread_pool(8192, 1);
        }

        s_initialized = true;
    }

    void Logger::Shutdown() {
        std::lock_guard lock(g_mutex);
        if (!s_initialized) return;
        for (auto& [name, logger] : g_loggers) logger->flush();
        g_loggers.clear();
        spdlog::shutdown();
        s_initialized = false;
    }

    std::shared_ptr<spdlog::logger> Logger::Get(std::string_view name) {
        std::lock_guard lock(g_mutex);

        std::string key(name);
        auto it = g_loggers.find(key);
        if (it != g_loggers.end()) return it->second;

        std::vector<spdlog::sink_ptr> sinks;

        if (s_config.enableConsole) {
            auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console->set_level(ToSpdlogLevel(s_config.consoleLevel));
            console->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%n] %v");
            sinks.push_back(console);
        }

#ifdef _WIN32
        auto msvc = std::make_shared<spdlog::sinks::msvc_sink_mt>();
        msvc->set_level(spdlog::level::trace);
        msvc->set_pattern("[%H:%M:%S.%e] [%l] [%n] %v");
        sinks.push_back(msvc);
#endif

        if (s_config.enableFile) {
            const auto filepath = s_config.logDirectory /
                                  (s_config.appName + "_" + key + ".log");
            auto file = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                filepath.string(),
                s_config.maxFileSizeMB * 1024 * 1024,
                s_config.maxFiles
            );
            file->set_level(ToSpdlogLevel(s_config.fileLevel));
            file->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%n] [thread %t] %v");
            sinks.push_back(file);
        }

        std::shared_ptr<spdlog::logger> logger;
        if (s_config.asyncMode) {
            logger = std::make_shared<spdlog::async_logger>(
                key, sinks.begin(), sinks.end(),
                spdlog::thread_pool(),
                spdlog::async_overflow_policy::block
            );
        } else {
            logger = std::make_shared<spdlog::logger>(key, sinks.begin(), sinks.end());
        }

        logger->set_level(spdlog::level::trace);
        logger->flush_on(spdlog::level::warn);
        spdlog::register_logger(logger);

        g_loggers[key] = logger;
        return logger;
    }

    void Logger::SetLevel(LogLevel level) {
        std::lock_guard lock(g_mutex);
        for (auto& [name, logger] : g_loggers) {
            logger->set_level(ToSpdlogLevel(level));
        }
    }

    bool Logger::IsInitialized() noexcept {
        return s_initialized;
    }

}