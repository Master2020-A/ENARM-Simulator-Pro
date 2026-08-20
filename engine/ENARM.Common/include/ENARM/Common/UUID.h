// ═══════════════════════════════════════════════════════════════
//   ENARM.Common - UUID
// ═══════════════════════════════════════════════════════════════
#pragma once

#include <string>
#include <cstdint>
#include <array>
#include <compare>

namespace ENARM::Common {

    class UUID {
    public:
        UUID() = default;

        [[nodiscard]] static UUID Generate();
        [[nodiscard]] static UUID FromString(const std::string& str);

        [[nodiscard]] std::string ToString() const;
        [[nodiscard]] bool IsNull() const noexcept;

        auto operator<=>(const UUID&) const = default;

        [[nodiscard]] const std::array<uint8_t, 16>& Bytes() const noexcept { return m_bytes; }

    private:
        std::array<uint8_t, 16> m_bytes{};
    };

}

namespace std {
    template<>
    struct hash<ENARM::Common::UUID> {
        size_t operator()(const ENARM::Common::UUID& uuid) const noexcept {
            const auto& b = uuid.Bytes();
            size_t h = 0;
            for (auto byte : b) h = h * 31 + byte;
            return h;
        }
    };
}