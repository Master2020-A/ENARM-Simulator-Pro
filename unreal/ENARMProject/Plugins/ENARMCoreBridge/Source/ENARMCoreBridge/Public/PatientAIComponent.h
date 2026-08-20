// UPatientAIComponent.h - Expone el paciente virtual ENARM a Blueprints
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatientAIComponent.generated.h"

/**
 * Componente que conecta el motor clinico ENARM (PatientAIController,
 * HemodynamicModel, RespiratoryModel) con el Actor del paciente en UE5.
 */
UCLASS(ClassGroup=(ENARM), meta=(BlueprintSpawnableComponent))
class ENARMCOREBRIDGE_API UPatientAIComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPatientAIComponent();

    // ─── Control de la simulacion ───
    UFUNCTION(BlueprintCallable, Category="ENARM|Simulation")
    void StartSimulation(const FString& CaseId);

    UFUNCTION(BlueprintCallable, Category="ENARM|Simulation")
    void TickSimulation(float DeltaSeconds);

    UFUNCTION(BlueprintCallable, Category="ENARM|Simulation")
    void ResetPatient();

    // ─── Paciente conversacional ───
    UFUNCTION(BlueprintCallable, Category="ENARM|AI")
    void AskPatient(const FString& DoctorQuestion,
                    const FString& OnCompleteCallback);

    UFUNCTION(BlueprintCallable, Category="ENARM|AI")
    bool IsAIReady() const;

    // ─── Signos vitales (para HUD / MetaHuman) ───
    UFUNCTION(BlueprintCallable, Category="ENARM|Vitals")
    float GetMAP() const;

    UFUNCTION(BlueprintCallable, Category="ENARM|Vitals")
    float GetHeartRate() const;

    UFUNCTION(BlueprintCallable, Category="ENARM|Vitals")
    float GetSpO2() const;

    UFUNCTION(BlueprintCallable, Category="ENARM|Vitals")
    float GetSystolicBP() const;

    UFUNCTION(BlueprintCallable, Category="ENARM|Vitals")
    float GetDiastolicBP() const;

    // ─── Farmacos ───
    UFUNCTION(BlueprintCallable, Category="ENARM|Pharmacology")
    bool GiveBolus(const FString&DrugName, float DoseMg);

    UFUNCTION(BlueprintCallable, Category="ENARM|Pharmacology")
    bool StartInfusion(const FString&DrugName, float RateMcgKgMin);

    UFUNCTION(BlueprintCallable, Category="ENARM|Pharmacology")
    bool StopInfusion(const FString&DrugName);

    // ─── Escenarios ───
    UFUNCTION(BlueprintCallable, Category="ENARM|Scenarios")
    void ApplyScenario(const FString&ScenarioName);

protected:
    virtual void BeginPlay() override;

private:
    // Punteros opacos al motor ENARM (evita incluir headers ENARM aqui)
    void* m_Simulator;
    bool  m_Initialized;
};
