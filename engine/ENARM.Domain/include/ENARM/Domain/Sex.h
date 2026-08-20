// ═══════════════════════════════════════════════════════════════
//   ENARM.Domain - Sexo biologico
// ═══════════════════════════════════════════════════════════════
#pragma once

#include <cstdint>

namespace ENARM::Domain {

    enum class Sex : uint8_t {
        Male,
        Female,
        Intersex,
        Unknown
    };

}