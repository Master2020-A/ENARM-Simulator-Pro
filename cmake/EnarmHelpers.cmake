# ═══════════════════════════════════════════════════════════════
#   ENARM CMake Helpers v3 - runtime uniforme por target
# ═══════════════════════════════════════════════════════════════

function(enarm_add_library TARGET_NAME)
    set(options "")
    set(oneValueArgs FOLDER)
    set(multiValueArgs SOURCES PUBLIC_HEADERS PUBLIC_DEPS PRIVATE_DEPS)
    cmake_parse_arguments(ENARM "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT ENARM_SOURCES AND NOT ENARM_PUBLIC_HEADERS)
        message(FATAL_ERROR "enarm_add_library(${TARGET_NAME}): no SOURCES ni PUBLIC_HEADERS")
    endif()

    add_library(${TARGET_NAME} STATIC ${ENARM_SOURCES} ${ENARM_PUBLIC_HEADERS})
    add_library(ENARM::${TARGET_NAME} ALIAS ${TARGET_NAME})

    target_include_directories(${TARGET_NAME}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

    target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)

    # FIX: runtime dinamico uniforme (evita conflictos symbols)
    set_target_properties(${TARGET_NAME} PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
    )

    if(ENARM_PUBLIC_DEPS)
        target_link_libraries(${TARGET_NAME} PUBLIC ${ENARM_PUBLIC_DEPS})
    endif()

    if(ENARM_PRIVATE_DEPS)
        target_link_libraries(${TARGET_NAME} PRIVATE ${ENARM_PRIVATE_DEPS})
    endif()

    if(ENARM_FOLDER)
        set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "${ENARM_FOLDER}")
    endif()

    if(ENARM_PUBLIC_HEADERS)
        source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" PREFIX "Headers" FILES ${ENARM_PUBLIC_HEADERS})
    endif()
    if(ENARM_SOURCES)
        source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Sources" FILES ${ENARM_SOURCES})
    endif()
endfunction()

function(enarm_add_executable TARGET_NAME)
    set(options "")
    set(oneValueArgs FOLDER)
    set(multiValueArgs SOURCES DEPS)
    cmake_parse_arguments(ENARM "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT ENARM_SOURCES)
        message(FATAL_ERROR "enarm_add_executable(${TARGET_NAME}): no SOURCES")
    endif()

    add_executable(${TARGET_NAME} ${ENARM_SOURCES})
    target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)

    # FIX: runtime dinamico uniforme
    set_target_properties(${TARGET_NAME} PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
    )

    # FIX: libpqxx (pqxx.dll) exporta simbolos de std::string_view que
    # colisionan con los inline de la CRT (LNK2005). /FORCE:MULTIPLE acepta
    # la primera definicion y silencia el error.
    if(MSVC)
        target_link_options(${TARGET_NAME} PRIVATE /FORCE:MULTIPLE)
    endif()

    if(ENARM_DEPS)
        target_link_libraries(${TARGET_NAME} PRIVATE ${ENARM_DEPS})
    endif()

    if(ENARM_FOLDER)
        set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "${ENARM_FOLDER}")
    endif()
endfunction()