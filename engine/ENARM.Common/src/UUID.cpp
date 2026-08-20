// ═══════════════════════════════════════════════════════════════
//   ENARM.Common - UUID Implementation
// ═══════════════════════════════════════════════════════════════
#include "ENARM/Common/UUID.h"

#include <random>
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace ENARM::Common {

    namespace {
        thread_local std::mt19937_64 g_rng{std::random_device{}()};
    }

    UUID UUID::Generate() {
        UUID uuid;
        std::uniform_int_distribution<uint64_t> dist;
        uint64_t p1 = dist(g_rng);
        uint64_t p2 = dist(g_rng);

        for (int i = 0; i < 8; ++i) {
            uuid.m_bytes[i] = static_cast<uint8_t>((p1 >> (i * 8)) & 0xFF);
            uuid.m_bytes[8 + i] = static_cast<uint8_t>((p2 >> (i * 8)) & 0xFF);
        }

        uuid.m_bytes[6] = (uuid.m_bytes[6] & 0x0F) | 0x40;
        uuid.m_bytes[8] = (uuid.m_bytes[8] & 0x3F) | 0x80;

        return uuid;
    }

    std::string UUID::ToString() const {
        std::ostringstream oss;
        oss << std::hex << std::setfill('0');
        for (size_t i = 0; i < 16; ++i) {
            oss << std::setw(2) << static_cast<int>(m_bytes[i]);
            if (i == 3 || i == 5 || i == 7 || i == 9) oss << '-';
        }
        return oss.str();
    }

    bool UUID::IsNull() const noexcept {
        for (auto b : m_bytes) if (b != 0) return false;
        return true;
    }

    UUID UUID::FromString(const std::string& str) {
        UUID uuid;
        std::string clean;
        clean.reserve(32);
        for (char c : str) {
            if (c != '-') clean += c;
        }
        if (clean.size() != 32) {
            throw std::invalid_argument("Invalid UUID string: " + str);
        }
        for (size_t i = 0; i < 16; ++i) {
            uuid.m_bytes[i] = static_cast<uint8_t>(
                std::stoi(clean.substr(i * 2, 2), nullptr, 16)
            );
        }
        return uuid;
    }

}