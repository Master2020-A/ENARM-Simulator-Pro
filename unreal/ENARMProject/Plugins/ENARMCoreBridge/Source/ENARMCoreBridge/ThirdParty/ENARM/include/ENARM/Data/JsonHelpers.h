// ═══════════════════════════════════════════════════════════════
//   ENARM.Data - Helpers (libpqxx v7.10+)
// ═══════════════════════════════════════════════════════════════
#pragma once

#include <nlohmann/json.hpp>
#include <pqxx/pqxx>
#include <string>
#include <optional>
#include <vector>

namespace ENARM::Data {

    // Extraer JSONB como nlohmann::json (usa auto& para compatibilidad row/row_ref)
    template <typename Row>
    inline nlohmann::json GetJson(const Row& row, const std::string& col) {
        if (row[col].is_null()) return nlohmann::json::object();
        try {
            return nlohmann::json::parse(row[col].template as<std::string>());
        } catch (...) {
            return nlohmann::json::object();
        }
    }

    template <typename Row>
    inline nlohmann::json GetJsonOr(const Row& row, const std::string& col,
                                     nlohmann::json def) {
        if (row[col].is_null()) return def;
        try {
            return nlohmann::json::parse(row[col].template as<std::string>());
        } catch (...) {
            return def;
        }
    }

    template <typename Row>
    inline std::optional<std::string> GetOptString(const Row& row, const std::string& col) {
        if (row[col].is_null()) return std::nullopt;
        return row[col].template as<std::string>();
    }

    template <typename Row>
    inline std::optional<int> GetOptInt(const Row& row, const std::string& col) {
        if (row[col].is_null()) return std::nullopt;
        return row[col].template as<int>();
    }

    // Parseo manual de TEXT[] de PostgreSQL: {"a","b","c"}
    template <typename Row>
    inline std::vector<std::string> GetStringArray(const Row& row, const std::string& col) {
        std::vector<std::string> result;
        if (row[col].is_null()) return result;

        std::string raw = row[col].template as<std::string>();
        if (raw.size() < 2 || raw.front() != '{' || raw.back() != '}') return result;

        raw = raw.substr(1, raw.size() - 2);
        std::string current;
        bool inQuotes = false;
        bool escaped = false;

        for (char c : raw) {
            if (escaped) {
                current += c;
                escaped = false;
                continue;
            }
            if (c == '\\') { escaped = true; continue; }
            if (c == '"') { inQuotes = !inQuotes; continue; }
            if (c == ',' && !inQuotes) {
                if (!current.empty()) result.push_back(current);
                current.clear();
            } else {
                current += c;
            }
        }
        if (!current.empty()) result.push_back(current);
        return result;
    }

    // vector<string> a TEXT[] literal para SQL
    inline std::string ToPgTextArray(const std::vector<std::string>& v) {
        if (v.empty()) return "{}";
        std::string out = "{";
        for (size_t i = 0; i < v.size(); ++i) {
            out += "\"";
            for (char c : v[i]) {
                if (c == '\\' || c == '"') out += '\\';
                out += c;
            }
            out += "\"";
            if (i + 1 < v.size()) out += ",";
        }
        out += "}";
        return out;
    }

}