# ================================================================
#   ENARM.MCQ - Motor de preguntas ENARM (MCQ Engine)
# ================================================================

set(PUBLIC_HEADERS
    include/ENARM/MCQ/MCQTypes.h
    include/ENARM/MCQ/MCQSession.h
)

set(SOURCES
    src/MCQSession.cpp
)

enarm_add_library(ENARM.MCQ
    FOLDER "Engine"
    SOURCES ${SOURCES}
    PUBLIC_HEADERS ${PUBLIC_HEADERS}
    PUBLIC_DEPS
        ENARM.Common
        ENARM.Domain
)