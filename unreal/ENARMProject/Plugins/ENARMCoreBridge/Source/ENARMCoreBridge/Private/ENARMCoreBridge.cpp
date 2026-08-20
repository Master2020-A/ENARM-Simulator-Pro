// ENARMCoreBridge.cpp
#include "ENARMCoreBridge.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FENARMCoreBridgeModule"

void FENARMCoreBridgeModule::StartupModule()
{
    // El motor clinico ENARM se inicializa via UPatientAIComponent
    // y UClinicalSceneGameMode al inicio de la simulacion.
}

void FENARMCoreBridgeModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FENARMCoreBridgeModule, ENARMCoreBridge)
