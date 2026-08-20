// ═══════════════════════════════════════════════════════════════
//   ENARM.Common - Config Implementation
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Common/Config.h"

#include <sstream>
#include <vector>

namespace ENARM::Common {

    namespace {
        std::vector<std::string> SplitPath(std::string_view path) {
            std::vector<std::string> parts;
            std::string current;
            for (char c : path) {
                if (c == '.') {
                    if (!current.empty()) parts.push_back(std::move(current));
                    current.clear();
                } else {
                    current += c;
                }
            }
            if (!current.empty()) parts.push_back(std::move(current));
            return parts;
        }

        const toml::node* Navigate(const toml::table& root, std::string_view path) {
            auto parts = SplitPath(path);
            if (parts.empty()) return nullptr;

            const toml::node* node = &root;
            for (const auto& part : parts) {
                if (auto* tbl = node->as_table()) {
                    node = tbl->get(part);
                    if (!node) return nullptr;
                } else {
                    return nullptr;
                }
            }
            return node;
        }
    }

    Result<Config> Config::LoadFromFile(const std::filesystem::path& path) {
        if (!std::filesystem::exists(path)) {
            return Err<Config>(404, "Config file not found: " + path.string(), "config");
        }
        try {
            Config cfg;
            cfg.m_table = toml::parse_file(path.string());
            return Result<Config>::Ok(std::move(cfg));
        } catch (const toml::parse_error& err) {
            std::stringstream ss;
            ss << "TOML parse error: " << err.description()
               << " at line " << err.source().begin.line;
            return Err<Config>(400, ss.str(), "config");
        }
    }

    Result<Config> Config::LoadFromString(std::string_view content) {
        try {
            Config cfg;
            cfg.m_table = toml::parse(content);
            return Result<Config>::Ok(std::move(cfg));
        } catch (const toml::parse_error& err) {
            return Err<Config>(400, std::string(err.description()), "config");
        }
    }

    std::optional<std::string> Config::GetString(std::string_view path) const {
        auto* node = Navigate(m_table, path);
        if (!node) return std::nullopt;
        if (auto val = node->value<std::string>()) return *val;
        return std::nullopt;
    }

    std::optional<int64_t> Config::GetInt(std::string_view path) const {
        auto* node = Navigate(m_table, path);
        if (!node) return std::nullopt;
        if (auto val = node->value<int64_t>()) return *val;
        return std::nullopt;
    }

    std::optional<double> Config::GetDouble(std::string_view path) const {
        auto* node = Navigate(m_table, path);
        if (!node) return std::nullopt;
        if (auto val = node->value<double>()) return *val;
        if (auto val = node->value<int64_t>()) return static_cast<double>(*val);
        return std::nullopt;
    }

    std::optional<bool> Config::GetBool(std::string_view path) const {
        auto* node = Navigate(m_table, path);
        if (!node) return std::nullopt;
        if (auto val = node->value<bool>()) return *val;
        return std::nullopt;
    }

    std::string Config::GetStringOr(std::string_view path, std::string def) const {
        return GetString(path).value_or(std::move(def));
    }
    int64_t Config::GetIntOr(std::string_view path, int64_t def) const {
        return GetInt(path).value_or(def);
    }
    double Config::GetDoubleOr(std::string_view path, double def) const {
        return GetDouble(path).value_or(def);
    }
    bool Config::GetBoolOr(std::string_view path, bool def) const {
        return GetBool(path).value_or(def);
    }

}