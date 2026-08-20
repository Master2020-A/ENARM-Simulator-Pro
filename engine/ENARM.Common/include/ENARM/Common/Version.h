// ═══════════════════════════════════════════════════════════════
//   ENARM.Common - Version
// ═══════════════════════════════════════════════════════════════
#pragma once

#include <string_view>

namespace ENARM::Common {

    inline constexpr std::string_view kVersion = "0.1.0";
    inline constexpr int kVersionMajor = 0;
    inline constexpr int kVersionMinor = 1;
    inline constexpr int kVersionPatch = 0;

    inline constexpr std::string_view kProjectName = "ENARM Simulator Pro";
    inline constexpr std::string_view kBuildDate = __DATE__;
    inline constexpr std::string_view kBuildTime = __TIME__;

}