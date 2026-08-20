// ═══════════════════════════════════════════════════════════════
//   ENARM.Common - Config (TOML)
// ═══════════════════════════════════════════════════════════════
#pragma once

#include "ENARM/Common/Result.h"
#include <toml++/toml.h>

#include <filesystem>
#include <string>
#include <string_view>
#include <optional>

namespace ENARM::Common {

    class Config {
    public:
        static Result<Config> LoadFromFile(const std::filesystem::path& path);
        static Result<Config> LoadFromString(std::string_view content);

        [[nodiscard]] std::optional<std::string> GetString(std::string_view path) const;
        [[nodiscard]] std::optional<int64_t>     GetInt(std::string_view path) const;
        [[nodiscard]] std::optional<double>      GetDouble(std::string_view path) const;
        [[nodiscard]] std::optional<bool>        GetBool(std::string_view path) const;

        [[nodiscard]] std::string GetStringOr(std::string_view path, std::string def) const;
        [[nodiscard]] int64_t     GetIntOr(std::string_view path, int64_t def) const;
        [[nodiscard]] double      GetDoubleOr(std::string_view path, double def) const;
        [[nodiscard]] bool        GetBoolOr(std::string_view path, bool def) const;

        [[nodiscard]] const toml::table& Raw() const noexcept { return m_table; }

    private:
        toml::table m_table;
    };

}