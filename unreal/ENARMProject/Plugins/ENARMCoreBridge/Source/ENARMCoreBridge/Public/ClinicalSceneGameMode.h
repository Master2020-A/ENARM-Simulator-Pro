// ClinicalSceneGameMode.h - GameMode de la escena clinica ENARM
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ClinicalSceneGameMode.generated.h"

class UPatientAIComponent;

/**
 * GameMode de la escena clinica: inicializa el simulador,
 * el paciente virtual y el HUD.
 */
UCLASS()
class ENARMCOREBRIDGE_API AClinicalSceneGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AClinicalSceneGameMode();

    // Inicializar escenario (llamado por Blueprints al empezar)
    UFUNCTION(BlueprintCallable, Category="ENARM|Setup")
    void InitializeScenario(const FString& ScenarioName);

    // Buscar componente del paciente en el nivel
    UFUNCTION(BlueprintCallable, Category="ENARM|Setup")
    UPatientAIComponent* FindPatientAI() const;

    // Estado de la escena
    UFUNCTION(BlueprintPure, Category="ENARM|Setup")
    bool IsSceneReady() const;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    UPatientAIComponent* m_PatientAI;

    bool m_SceneReady;
};
