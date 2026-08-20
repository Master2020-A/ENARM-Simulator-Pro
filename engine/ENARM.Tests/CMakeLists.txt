cmake_minimum_required(VERSION 3.28)
project(ENARM.Tests)

find_package(Catch2 3 REQUIRED)

add_executable(ENARM.Tests
    src/main_test.cpp
    src/test_hemodynamic.cpp
    src/test_pharmacology.cpp
    src/test_assessment.cpp
    src/test_competency.cpp
    src/test_metabolic.cpp
    src/test_ecg.cpp
)

target_link_libraries(ENARM.Tests PRIVATE
    ENARM.Physiology
    ENARM.Pharmacology
    ENARM.Simulation
    ENARM.Assessment
    ENARM.Data
    ENARM.Common
    ENARM.Domain
    Catch2::Catch2WithMain
)

target_compile_features(ENARM.Tests PRIVATE cxx_std_20)

include(CTest)
include(Catch)
catch_discover_tests(ENARM.Tests)