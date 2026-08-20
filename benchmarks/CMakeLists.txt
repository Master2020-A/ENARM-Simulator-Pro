# ============================================================
#   ENARM Benchmarks - CMakeLists
#   Compila y registra benchmarks con CTest
# ============================================================

find_package(Catch2 CONFIG REQUIRED)

add_executable(ENARM.Benchmarks
    benchmark_simulation.cpp
)

target_link_libraries(ENARM.Benchmarks PRIVATE
    ENARM.Simulation
    ENARM.Physiology
    ENARM.Pharmacology
    ENARM.Common
    ENARM.Domain
)

target_compile_features(ENARM.Benchmarks PRIVATE cxx_std_20)

if(MSVC)
    target_link_options(ENARM.Benchmarks PRIVATE /FORCE:MULTIPLE)
endif()

# Registrar como benchmark (no test)
add_test(NAME benchmark_simulation
    COMMAND ENARM.Benchmarks 100 30
)
set_tests_properties(benchmark_simulation PROPERTIES
    LABELS "benchmark"
    TIMEOUT 120
)
