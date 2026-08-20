# ============================================================
#   ENARM SIMULATOR PRO - Root CMakeLists.txt v3
# ============================================================

cmake_minimum_required(VERSION 3.28)

# CRITICO: CMP0091 debe ir ANTES de project()
if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

# Runtime library (necesita CMP0091 NEW)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
    CACHE STRING "MSVC runtime library" FORCE)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{VCPKG_ROOT})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "Vcpkg toolchain file")
    else()
        set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "Vcpkg toolchain file")
    endif()
endif()

# vcpkg triplet: x64-windows usa CRT dinamico por defecto
set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "vcpkg triplet")

project(ENARMSimulator
    VERSION 0.1.0
    DESCRIPTION "ENARM Simulator Pro"
    LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

option(ENARM_BUILD_TESTS       "Compilar tests"                  ON)
option(ENARM_BUILD_BENCHMARKS  "Compilar benchmarks"             OFF)
option(ENARM_BUILD_TOOLS       "Compilar tools ImGui"            ON)
option(ENARM_ENABLE_LTO        "Link Time Optimization"          OFF)
option(ENARM_ENABLE_ASAN       "AddressSanitizer (Debug)"        OFF)
option(ENARM_WARNINGS_AS_ERRORS "Warnings como errores"          OFF)

if(MSVC)
    add_compile_options(
        /W4 /permissive- /Zc:__cplusplus /Zc:preprocessor /utf-8 /MP /EHsc
        /DNOMINMAX /DWIN32_LEAN_AND_MEAN /D_WIN32_WINNT=0x0A00
    )
    # Suprimir warnings comunes que no son criticos
    add_compile_options(
        /wd4834   # nodiscard warning
        /wd4189   # variable local sin usar
        /wd4100   # parametro sin usar
    )

    if(ENARM_WARNINGS_AS_ERRORS)
        add_compile_options(/WX)
    endif()
    if(ENARM_ENABLE_LTO)
        add_compile_options($<$<CONFIG:Release>:/GL>)
        add_link_options($<$<CONFIG:Release>:/LTCG>)
    endif()
endif()

include(cmake/EnarmHelpers.cmake OPTIONAL)

message(STATUS "")
message(STATUS "============================================================")
message(STATUS "  ENARM Simulator Pro v${PROJECT_VERSION}")
message(STATUS "============================================================")
message(STATUS "  Build type:      ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ Standard:    C++${CMAKE_CXX_STANDARD}")
message(STATUS "  Compiler:        ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  Toolchain:       ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "  Triplet:         ${VCPKG_TARGET_TRIPLET}")
message(STATUS "  MSVC Runtime:    ${CMAKE_MSVC_RUNTIME_LIBRARY}")
message(STATUS "============================================================")
message(STATUS "")

# --- ENGINE (orden importa) ---
add_subdirectory(engine/ENARM.Common)
add_subdirectory(engine/ENARM.Domain)
add_subdirectory(engine/ENARM.Physiology)
add_subdirectory(engine/ENARM.Pharmacology)
add_subdirectory(engine/ENARM.Simulation)
add_subdirectory(engine/ENARM.Data)
add_subdirectory(engine/ENARM.AI)
add_subdirectory(engine/ENARM.Assessment)
add_subdirectory(engine/ENARM.MCQ)
add_subdirectory(engine/ENARM.Learning)
add_subdirectory(engine/ENARM.GUI)

# --- Tests ---
add_subdirectory(tests/ENARM.SmokeTest)

# --- Tools ---
add_subdirectory(tools/ENARM.CLI)
add_subdirectory(tools/ENARM.App)

if(ENARM_BUILD_TESTS)
    enable_testing()
    include(CTest)
endif()

include(GNUInstallDirs)

# ─── Packaging (CPack) ──────────────────────────────────────────
set(CPACK_PACKAGE_NAME "ENARM-Simulator-Pro")
set(CPACK_PACKAGE_VENDOR "ENARM")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Simulador clinico profesional para preparacion ENARM")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_GENERATOR "ZIP")
if(WIN32)
    set(CPACK_GENERATOR "ZIP")
endif()
set(CPACK_WIX_UPGRADE_GUID "8A0B5C1E-2D3F-4A56-9B78-CDEF01234567")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "ENARM-Simulator-Pro")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
include(CPack)

message(STATUS "")
message(STATUS "Configuracion CMake completa")
message(STATUS "")
# ---------------------------------------------------------------
# Tests unitarios (Catch2) — Fase 0
# ---------------------------------------------------------------
enable_testing()
add_subdirectory(engine/ENARM.Tests)
