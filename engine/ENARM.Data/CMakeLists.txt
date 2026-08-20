# ═══════════════════════════════════════════════════════════════
#   ENARM.Data - Persistencia PostgreSQL (libpqxx)
# ═══════════════════════════════════════════════════════════════

find_package(libpqxx CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set(PUBLIC_HEADERS
    include/ENARM/Data/ConnectionConfig.h
    include/ENARM/Data/DatabaseManager.h
    include/ENARM/Data/JsonHelpers.h
    include/ENARM/Data/MCQEngine.h
    include/ENARM/Data/Repositories/UserRepository.h
    include/ENARM/Data/Repositories/StudentRepository.h
    include/ENARM/Data/Repositories/CaseRepository.h
    include/ENARM/Data/Repositories/MCQRepository.h
    include/ENARM/Data/Repositories/SessionRepository.h
    include/ENARM/Data/Repositories/GuidelineRepository.h
)

set(SOURCES
    src/DatabaseManager.cpp
    src/MCQEngine.cpp
    src/Repositories/UserRepository.cpp
    src/Repositories/StudentRepository.cpp
    src/Repositories/CaseRepository.cpp
    src/Repositories/MCQRepository.cpp
    src/Repositories/SessionRepository.cpp
    src/Repositories/GuidelineRepository.cpp
)

enarm_add_library(ENARM.Data
    FOLDER "Engine"
    SOURCES ${SOURCES}
    PUBLIC_HEADERS ${PUBLIC_HEADERS}
    PUBLIC_DEPS
        ENARM.Common
        ENARM.Domain
        libpqxx::pqxx
        nlohmann_json::nlohmann_json
)