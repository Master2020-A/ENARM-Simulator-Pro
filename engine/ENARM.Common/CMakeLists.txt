# â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•
#   ENARM.Common - Utilidades compartidas
# â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•

find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(tomlplusplus CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set(PUBLIC_HEADERS
    include/ENARM/Common/Version.h
    include/ENARM/Common/Logger.h
    include/ENARM/Common/Config.h
    include/ENARM/Common/Result.h
    include/ENARM/Common/UUID.h
    include/ENARM/Common/SpecialtyMapper.h
)

set(SOURCES
    src/Logger.cpp
    src/Config.cpp
    src/UUID.cpp
)

enarm_add_library(ENARM.Common
    FOLDER "Engine"
    SOURCES ${SOURCES}
    PUBLIC_HEADERS ${PUBLIC_HEADERS}
    PUBLIC_DEPS
        spdlog::spdlog
        fmt::fmt
        tomlplusplus::tomlplusplus
        nlohmann_json::nlohmann_json
)
