# ═══════════════════════════════════════════════════════════════
#   ENARM.App - Aplicacion grafica principal
# ═══════════════════════════════════════════════════════════════

add_executable(ENARM.App WIN32 src/main.cpp)

target_compile_features(ENARM.App PRIVATE cxx_std_20)

set_target_properties(ENARM.App PROPERTIES
    FOLDER "Tools"
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
    OUTPUT_NAME "ENARM"
)

# FIX: libpqxx (pqxx.dll) exporta simbolos inline de std::string_view que
# colisionan con la CRT (LNK2005). /FORCE:MULTIPLE acepta la primera
# definicion (la correcta) y convierte el error en warning benigno.
if(MSVC)
    target_link_options(ENARM.App PRIVATE /FORCE:MULTIPLE)
endif()

target_link_libraries(ENARM.App PRIVATE
    ENARM::ENARM.Common
    ENARM::ENARM.Domain
    ENARM::ENARM.Physiology
    ENARM::ENARM.Pharmacology
    ENARM::ENARM.Simulation
    ENARM::ENARM.Data
    ENARM::ENARM.AI
    ENARM::ENARM.GUI
    d3d11
    dxgi
    d3dcompiler
)

install(TARGETS ENARM.App
    RUNTIME DESTINATION bin
)