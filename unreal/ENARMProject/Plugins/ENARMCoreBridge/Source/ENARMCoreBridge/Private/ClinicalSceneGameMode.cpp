// ClinicalSceneGameMode.cpp
#include "ClinicalSceneGameMode.h"
#include "PatientAIComponent.h"
#include "Engine/World.h"
#include "EngineUtils.h"

AClinicalSceneGameMode::AClinicalSceneGameMode()
    : m_PatientAI(nullptr)
    , m_SceneReady(false)
{
    // HUD por defecto: se asigna un Blueprint en el editor
}

void AClinicalSceneGameMode::BeginPlay()
{
    Super::BeginPlay();
    m_PatientAI = FindPatientAI();
    if (m_PatientAI)
    {
        UE_LOG(LogTemp, Log, TEXT("[ENARM] Paciente virtual encontrado"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[ENARM] No se encontro UPatientAIComponent en el nivel"));
    }
}

UPatientAIComponent* AClinicalSceneGameMode::FindPatientAI() const
{
    if (const UWorld* World = GetWorld())
    {
        for (TActorIterator<AActor> It(World); It; ++It)
        {
            if (UPatientAIComponent* Comp = It->FindComponentByClass<UPatientAIComponent>())
            {
                return Comp;
            }
        }
    }
    return nullptr;
}

void AClinicalSceneGameMode::InitializeScenario(const FString& ScenarioName)
{
    if (!m_PatientAI)
    {
        m_PatientAI = FindPatientAI();
    }
    if (m_PatientAI)
    {
        m_PatientAI->StartSimulation(FString::Printf(TEXT("escenario_%s"), *ScenarioName));
        m_PatientAI->ApplyScenario(ScenarioName);
        m_SceneReady = true;
        UE_LOG(LogTemp, Log, TEXT("[ENARM] Escenario iniciado: %s"), *ScenarioName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[ENARM] No se pudo iniciar escenario: sin paciente"));
    }
}

bool AClinicalSceneGameMode::IsSceneReady() const
{
    return m_SceneReady && m_PatientAI != nullptr;
}
