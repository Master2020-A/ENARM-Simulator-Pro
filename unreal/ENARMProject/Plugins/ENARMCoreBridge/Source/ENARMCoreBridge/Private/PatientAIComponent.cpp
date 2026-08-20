// PatientAIComponent.cpp - Implementacion con headers ENARM reales
#include "PatientAIComponent.h"
#include "Engine/World.h"

// Headers del motor clinico ENARM (modulos puros + AI/Data con vcpkg)
#include "ENARM/Physiology/HemodynamicModel.h"
#include "ENARM/Pharmacology/DrugDatabase.h"
#include "ENARM/Domain/Patient.h"
#include "ENARM/Common/Result.h"
#include "ENARM/AI/PatientAIController.h"
#include "ENARM/AI/LLMRouter.h"
#include "ENARM/AI/OllamaClient.h"
#include "ENARM/AI/PromptTemplates/PatientPrompts.h"

UPatientAIComponent::UPatientAIComponent()
    : m_Simulator(nullptr)
    , m_Initialized(false)
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UPatientAIComponent::BeginPlay()
{
    Super::BeginPlay();
    m_Simulator = new ENARM::Physiology::HemodynamicModel();
    m_Initialized = true;
    UE_LOG(LogTemp, Log, TEXT("[ENARM] Bridge inicializado. Farmacos: %d"),
           (int)ENARM::Pharmacology::DrugDatabase::Instance().Count());
}

void UPatientAIComponent::StartSimulation(const FString& CaseId)
{
    if (!m_Simulator) return;
    UE_LOG(LogTemp, Warning, TEXT("[ENARM] Iniciando simulacion caso: %s"), *CaseId);
}

void UPatientAIComponent::TickSimulation(float DeltaSeconds)
{
    if (!m_Simulator) return;
    static_cast<ENARM::Physiology::HemodynamicModel*>(m_Simulator)->Update(DeltaSeconds);
}

void UPatientAIComponent::ResetPatient()
{
    if (!m_Simulator) return;
    static_cast<ENARM::Physiology::HemodynamicModel*>(m_Simulator)->ResetToNormal();
}

void UPatientAIComponent::AskPatient(const FString& DoctorQuestion,
                                     const FString& /*OnCompleteCallback*/)
{
    UE_LOG(LogTemp, Log, TEXT("[ENARM] Pregunta: %s"), *DoctorQuestion);
    // TODO Sprint 10: instanciar PatientAIController con router Ollama
    // y conectar respuesta a TTS + lip sync del MetaHuman
}

bool UPatientAIComponent::IsAIReady() const
{
    return m_Initialized && m_Simulator != nullptr;
}

float UPatientAIComponent::GetMAP() const
{
    if (!m_Simulator) return 0.0f;
    return static_cast<float>(
        static_cast<ENARM::Physiology::HemodynamicModel*>(m_Simulator)->GetMAP());
}

float UPatientAIComponent::GetHeartRate() const
{
    if (!m_Simulator) return 0.0f;
    return static_cast<float>(
        static_cast<ENARM::Physiology::HemodynamicModel*>(m_Simulator)->GetParameters().heartRate_bpm);
}

float UPatientAIComponent::GetSpO2() const
{
    return 98.0f;  // Sprint 10: conectar RespiratoryModel
}

float UPatientAIComponent::GetSystolicBP() const
{
    if (!m_Simulator) return 0.0f;
    return static_cast<float>(
        static_cast<ENARM::Physiology::HemodynamicModel*>(m_Simulator)->GetSystolicBP());
}

float UPatientAIComponent::GetDiastolicBP() const
{
    if (!m_Simulator) return 0.0f;
    return static_cast<float>(
        static_cast<ENARM::Physiology::HemodynamicModel*>(m_Simulator)->GetDiastolicBP());
}

bool UPatientAIComponent::GiveBolus(const FString&DrugName, float DoseMg)
{
    if (!m_Simulator) return false;

    auto drug = ENARM::Pharmacology::DrugDatabase::Instance()
        .FindByName(TCHAR_TO_UTF8(*DrugName));
    if (!drug.has_value())
    {
        UE_LOG(LogTemp, Error, TEXT("[ENARM] Farmaco no encontrado: %s"), *DrugName);
        return false;
    }
    if (DoseMg <= 0.0f)
    {
        UE_LOG(LogTemp, Error, TEXT("[ENARM] Dosis invalida para %s"), *DrugName);
        return false;
    }

    auto* heart = static_cast<ENARM::Physiology::HemodynamicModel*>(m_Simulator);
    if (drug->category == ENARM::Pharmacology::DrugCategory::Vasopressor)
    {
        heart->ApplyVasoconstrictionBaseline(1.3);
    }
    else if (drug->category == ENARM::Pharmacology::DrugCategory::Inotrope)
    {
        heart->SetStrokeVolume(heart->GetParameters().strokeVolume_mL * 1.2);
    }

    UE_LOG(LogTemp, Log, TEXT("[ENARM] Bolo %s %.1f mg OK"), *DrugName, DoseMg);
    return true;
}

bool UPatientAIComponent::StartInfusion(const FString&DrugName, float RateMcgKgMin)
{
    UE_LOG(LogTemp, Log, TEXT("[ENARM] Infusion %s %.1f mcg/kg/min"), *DrugName, RateMcgKgMin);
    return true;
}

bool UPatientAIComponent::StopInfusion(const FString&DrugName)
{
    UE_LOG(LogTemp, Log, TEXT("[ENARM] Detener infusion %s"), *DrugName);
    return true;
}

void UPatientAIComponent::ApplyScenario(const FString& ScenarioName)
{
    if (!m_Simulator) return;
    auto* heart = static_cast<ENARM::Physiology::HemodynamicModel*>(m_Simulator);
    const std::string s = TCHAR_TO_UTF8(*ScenarioName);
    if (s == "sepsis")        heart->SimulateShock(0.8);
    else if (s == "stemi")    heart->SimulateHypertension(0.3);
    else if (s == "normal")   heart->ResetToNormal();
    UE_LOG(LogTemp, Log, TEXT("[ENARM] Escenario aplicado: %s"), *ScenarioName);
}
