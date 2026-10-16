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

    // Restart flag used after OpenLevel
    bool bRestartAfterReload = false;

    // Used by InitGame flow
    bool bHasInitializedFirstTimeSetup = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lives")
    int32 LivesCount; //Set to 3 by default

#pragma region Lives
    UFUNCTION(BlueprintCallable, Category = "WorldSwap")
    void SetLivesCount(int32 LivesCountToSet);

    void AddToLivesCount();
    void SubtractFromLivesCount();

    int32 GetLivesCount();
#pragma endregion 

private:
    // Called automatically after a map finishes loading
    UFUNCTION()
    void HandleMapLoaded(UWorld* LoadedWorld);

    UPROPERTY()
    UUIManager* UIManager = nullptr;
};
