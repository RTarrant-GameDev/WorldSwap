// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameStateSubSystem.h"
#include "UIManager.generated.h"

class UUserWidget;
class UGameStateSubSystem;

UCLASS(BlueprintType, Blueprintable)
class WORLDSWAP_API UUIManager : public UObject
{
	GENERATED_BODY()
	
public:
    void Initialize(UWorld* World);

    UFUNCTION()
    void HandleStateChanged(EGameState NewState);
private:
    void ProcessStateChange();

    void ShowStartMenu();
    void ShowHUD();
    void ShowPauseMenu();
    void ShowGameOverMenu();
    void ShowLevelCompleteMenu();

    virtual UWorld* GetWorld() const override;

    UPROPERTY()
    UUserWidget* CurrentWidget = nullptr;

    EGameState PendingState;
};
