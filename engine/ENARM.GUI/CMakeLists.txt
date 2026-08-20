cmake_minimum_required(VERSION 3.28)

find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

# ---- Fuentes GUI ----
set(GUI_SOURCES
    src/Application.cpp
    src/AppContext.cpp
    src/Theme.cpp

    # Sprint 7A.2.5: Provider de findings/estudios por caso
    src/CaseFindingsProvider.cpp
    src/CaseFindingsProvider_Studies.cpp

    # Views (version nueva: namespace ENARM::GUI::Views)
    src/Views/CasesListView.cpp
    src/Views/PatientSessionView.cpp
    src/Views/ScoringPanelView.cpp
    src/Views/GuidelinesListView.cpp
    src/Views/EmergencyCodeView.cpp
    src/Views/ECGMonitorView.cpp

    # Views opcionales (agregar cuando existan)
    # src/Views/CaseSelectorView.cpp
    # src/Views/PatientChatView.cpp
    # src/Views/VitalMonitorView.cpp
    # src/Views/MCQPracticeView.cpp
    # src/Views/ProgressView.cpp
    # src/Views/SettingsView.cpp
)

# Filtrar solo los que existen
set(GUI_SOURCES_EXISTING)
foreach(src ${GUI_SOURCES})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
        list(APPEND GUI_SOURCES_EXISTING ${src})
    endif()
endforeach()

add_library(ENARM.GUI STATIC ${GUI_SOURCES_EXISTING})
add_library(ENARM::ENARM.GUI ALIAS ENARM.GUI)

target_include_directories(ENARM.GUI
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(ENARM.GUI
    PUBLIC
        ENARM.Common
        ENARM.Domain
        ENARM.AI
        ENARM.Assessment
        ENARM.Simulation
        ENARM.Data
    PRIVATE
        imgui::imgui
        implot::implot
        nlohmann_json::nlohmann_json
        spdlog::spdlog
)

target_compile_features(ENARM.GUI PUBLIC cxx_std_20)

target_compile_options(ENARM.GUI PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W3 /utf-8 /bigobj>
)

set_target_properties(ENARM.GUI PROPERTIES
    FOLDER "Libraries"
)


