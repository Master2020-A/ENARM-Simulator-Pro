enarm_add_executable(ENARM.SmokeTest
    FOLDER "Tests"
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