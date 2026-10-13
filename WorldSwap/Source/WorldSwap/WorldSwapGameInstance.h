// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WorldSwapGameInstance.generated.h"

class UUIManager;

UCLASS()
class WORLDSWAP_API UWorldSwapGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:
    virtual void Init() override;

    UUIManager* GetUIManager() const { return UIManager; }

    UFUNCTION()
    void CreateUIManager(UWorld* World);

    // This variable persists through level reloads, crashes, and resets
    UPROPERTY(BlueprintReadWrite, Category = "Game Progress")
    bool bHasInitializedFirstTimeSetup = false;

private:
    UPROPERTY()
    UUIManager* UIManager = nullptr;
};
