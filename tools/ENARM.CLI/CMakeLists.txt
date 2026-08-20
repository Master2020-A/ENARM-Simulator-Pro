# ═══════════════════════════════════════════════════════════════
#   ENARM.CLI - Sesion clinica interactiva
# ═══════════════════════════════════════════════════════════════

enarm_add_executable(ENARM.CLI
    FOLDER "Tools"
    SOURCES src/main.cpp
    DEPS
        ENARM::ENARM.Common
        ENARM::ENARM.Domain
        ENARM::ENARM.Physiology
        ENARM::ENARM.Pharmacology
        ENARM::ENARM.Simulation
        ENARM::ENARM.Data
        ENARM::ENARM.AI
)