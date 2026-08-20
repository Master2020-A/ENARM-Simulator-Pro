// PatientAIComponent.cpp - Implementacion con headers ENARM reales
#include "PatientAIComponent.h"
#include "Engine/World.h"

// Headers del motor clinico ENARM (solo Physiology, sin dependencias vcpkg)
#include "ENARM/Physiology/HemodynamicModel.h"
#include "ENARM/Physiology/RespiratoryModel.h"

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
    // TODO Sprint 10: conectar PatientAIController (requiere vcpkg en UE5)
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
    // Physiology puro no incluye respiratorio; se conecta en Sprint 10
    return 98.0f;
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
    // Farmacos requieren ENARM.Pharmacology (vcpkg) - Sprint 10
    UE_LOG(LogTemp, Warning, TEXT("[ENARM] Bolo %s %f mg (pendiente integration farmacos)"),
           *DrugName, DoseMg);
    return true;
}

bool UPatientAIComponent::StartInfusion(const FString&DrugName, float RateMcgKgMin)
{
    UE_LOG(LogTemp, Warning, TEXT("[ENARM] Infusion %s %f mcg/kg/min (pendiente)"), *DrugName, RateMcgKgMin);
    return true;
}

bool UPatientAIComponent::StopInfusion(const FString&DrugName)
{
    UE_LOG(LogTemp, Warning, TEXT("[ENARM] Detener infusion %s"), *DrugName);
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
