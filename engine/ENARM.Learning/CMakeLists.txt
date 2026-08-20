# ═══════════════════════════════════════════════════════════════
#   ENARM.Learning - Aprendizaje adaptativo
#   SpacedRepetition (FSRS), WeaknessDetector, AdaptiveCaseSelector
# ═══════════════════════════════════════════════════════════════

set(PUBLIC_HEADERS
    include/ENARM/Learning/SpacedRepetition.h
    include/ENARM/Learning/WeaknessDetector.h
)

set(SOURCES
    src/SpacedRepetition.cpp
    src/WeaknessDetector.cpp
)

enarm_add_library(ENARM.Learning
    FOLDER "Engine"
    SOURCES ${SOURCES}
    PUBLIC_HEADERS ${PUBLIC_HEADERS}
    PUBLIC_DEPS
        ENARM.Common
)