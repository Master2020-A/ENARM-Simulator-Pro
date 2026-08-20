# ═══════════════════════════════════════════════════════════════
#   ENARM.Pharmacology - Farmacocinetica y base farmacos ICU
# ═══════════════════════════════════════════════════════════════

set(PUBLIC_HEADERS
    include/ENARM/Pharmacology/DrugTypes.h
    include/ENARM/Pharmacology/DrugParameters.h
    include/ENARM/Pharmacology/PharmacokineticModel.h
    include/ENARM/Pharmacology/DrugDatabase.h
    include/ENARM/Pharmacology/DrugAdministration.h
)

set(SOURCES
    src/PharmacokineticModel.cpp
    src/DrugDatabase.cpp
    src/DrugAdministration.cpp
)

enarm_add_library(ENARM.Pharmacology
    FOLDER "Engine"
    SOURCES ${SOURCES}
    PUBLIC_HEADERS ${PUBLIC_HEADERS}
    PUBLIC_DEPS
        ENARM.Common
        ENARM.Domain
        ENARM.Physiology
)