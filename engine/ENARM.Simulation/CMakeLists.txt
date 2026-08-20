# ═══════════════════════════════════════════════════════════════
#   ENARM.Simulation - Orquestador de paciente
# ═══════════════════════════════════════════════════════════════

set(PUBLIC_HEADERS
    include/ENARM/Simulation/EventLog.h
    include/ENARM/Simulation/PhysiologyPharmacologyBridge.h
    include/ENARM/Simulation/ClinicalScenarios.h
    include/ENARM/Simulation/PatientSimulator.h
    include/ENARM/Simulation/DiseaseProgressionEngine.h
    include/ENARM/Simulation/EmergencyCode.h
    include/ENARM/Simulation/SimulationRoom.h
)

set(SOURCES
    src/EventLog.cpp
    src/PhysiologyPharmacologyBridge.cpp
    src/ClinicalScenarios.cpp
    src/PatientSimulator.cpp
    src/DiseaseProgressionEngine.cpp
    src/EmergencyCode.cpp
    src/SimulationRoom.cpp
)

enarm_add_library(ENARM.Simulation
    FOLDER "Engine"
    SOURCES ${SOURCES}
    PUBLIC_HEADERS ${PUBLIC_HEADERS}
    PUBLIC_DEPS
        ENARM.Common
        ENARM.Domain
        ENARM.Physiology
        ENARM.Pharmacology
)