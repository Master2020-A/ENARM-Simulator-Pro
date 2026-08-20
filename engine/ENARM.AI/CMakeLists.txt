# ═══════════════════════════════════════════════════════════════
#   ENARM.AI - Integracion con LLMs
# ═══════════════════════════════════════════════════════════════

find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set(PUBLIC_HEADERS
    include/ENARM/AI/LLMTypes.h
    include/ENARM/AI/ILLMClient.h
    include/ENARM/AI/OllamaClient.h
    include/ENARM/AI/OpenAIClient.h
    include/ENARM/AI/LLMRouter.h
    include/ENARM/AI/ConversationMemory.h
    include/ENARM/AI/PatientAIController.h
    include/ENARM/AI/RAGService.h
    include/ENARM/AI/PromptTemplates/PatientPrompts.h
    include/ENARM/AI/PromptTemplates/EvaluatorPrompts.h
)

set(SOURCES
    src/OllamaClient.cpp
    src/OpenAIClient.cpp
    src/LLMRouter.cpp
    src/PatientPrompts.cpp
    src/ConversationMemory.cpp
    src/PatientAIController.cpp
    src/RAGService.cpp
)

enarm_add_library(ENARM.AI
    FOLDER "Engine"
    SOURCES ${SOURCES}
    PUBLIC_HEADERS ${PUBLIC_HEADERS}
    PUBLIC_DEPS
        ENARM.Common
        ENARM.Domain
        ENARM.Data
        cpr::cpr
        nlohmann_json::nlohmann_json
)