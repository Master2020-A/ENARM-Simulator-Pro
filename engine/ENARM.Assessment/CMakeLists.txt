cmake_minimum_required(VERSION 3.28)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_library(ENARM.Assessment STATIC
    # Core
    src/RubricDefinition.cpp
    src/CompetencyEvaluator.cpp
    src/ScoreCalculator.cpp
    src/AssessmentEngine.cpp
    src/FeedbackGenerator.cpp

    # Sprint 7A.2: Rubricas de 7 casos nuevos
    src/NeumoniaRubric.cpp
    src/PreeclampsiaRubric.cpp
    src/ApendicitisRubric.cpp
    src/TCERubric.cpp
    src/CADRubric.cpp
    src/LRARubric.cpp
    src/BronquiolitisRubric.cpp
)
add_library(ENARM::ENARM.Assessment ALIAS ENARM.Assessment)

set_target_properties(ENARM.Assessment PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
)

target_include_directories(ENARM.Assessment
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(ENARM.Assessment
    PUBLIC
        ENARM.Common
        ENARM.Domain
        ENARM.AI
    PRIVATE
        nlohmann_json::nlohmann_json
        spdlog::spdlog
)

target_compile_definitions(ENARM.Assessment PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_features(ENARM.Assessment PUBLIC cxx_std_20)

target_compile_options(ENARM.Assessment PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W3 /utf-8>
)

set_target_properties(ENARM.Assessment PROPERTIES
    FOLDER "Libraries"
)