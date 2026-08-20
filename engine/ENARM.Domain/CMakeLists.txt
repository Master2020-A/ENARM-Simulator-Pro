# ═══════════════════════════════════════════════════════════════
#   ENARM.Domain - Modelos de dominio
# ═══════════════════════════════════════════════════════════════

find_package(nlohmann_json CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)

set(PUBLIC_HEADERS
    include/ENARM/Domain/Sex.h
    include/ENARM/Domain/MedicalSpecialty.h
    include/ENARM/Domain/VitalSigns.h
    include/ENARM/Domain/Patient.h
    include/ENARM/Domain/Symptom.h
    include/ENARM/Domain/ClinicalCase.h
)

set(SOURCES
    src/VitalSigns.cpp
    src/Patient.cpp
    src/ClinicalCase.cpp
)

enarm_add_library(ENARM.Domain
    FOLDER "Engine"
    SOURCES ${SOURCES}
    PUBLIC_HEADERS ${PUBLIC_HEADERS}
    PUBLIC_DEPS
        ENARM.Common
        nlohmann_json::nlohmann_json
        magic_enum::magic_enum
)