// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TimerObject.h"
#include "GameFramework/GameModeBase.h"
#include "LevelEndpoint.h"
#include "WorldSwapGameMode.generated.h"

class UGameStateSubSystem;
class UWorldSwapGameInstance;
/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class AWorldSwapGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	AWorldSwapGameMode();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WorldSwap")
	int32 WorldSwapState; //Set to 0/WorldSwapState "1" by default

public:
	void HandlePlayerDeath();
#pragma region Lives
	UFUNCTION(BlueprintCallable, Category = "Lives")

	void AddToLivesCount();
	void SubtractFromLivesCount();

	UFUNCTION(BlueprintCallable, Category = "Lives")
	int32 GetLivesCount();
#pragma endregion 
	UFUNCTION(BlueprintCallable, Category = "WorldSwap")
	void SetWorldSwapState(int32 swapState);

	int32 GetWorldSwapState();

	UFUNCTION(BlueprintCallable, Category = "Menus")
	void StartGame();

	UFUNCTION(BlueprintCallable, Category = "Menus")
	void PauseGame();

	UFUNCTION(BlueprintCallable, Category = "Menus")
	void ResumeGame();

	UFUNCTION(BlueprintCallable, Category = "Menus")
	void ReturnToMainMenu();

	UFUNCTION(BlueprintCallable, Category = "Menus")
	void GameOver();

	UFUNCTION(BlueprintCallable, Category = "Menus")
	void LevelComplete();

	UFUNCTION(BlueprintCallable, Category = "Menus")
	void RestartGame(bool wasGameOver);

	APlayerController* GetPlayerController();

	void CollectablePickedUp();
	
	UPROPERTY()
	ATimerObject* TimerRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collectables")
	int32 CurrentCollectablesCollected = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collectables")
	int32 TotalCollectablesRequired = 0;
protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void BeginPlay() override;

	void EndPlay(const EEndPlayReason::Type EndPlayReason);

	UWorldSwapGameInstance* GetWSGI() const;
	UGameStateSubSystem* GetGameStateSM() const;

	void InitCollectables();
	void InitTimer();
	void InitUI();
	void InitGameState();

protected:

	UPROPERTY()
	ALevelEndpoint* LevelEndpointRef;

	UFUNCTION(BlueprintCallable, Category = "Timer")
	int32 GetWorldSwapTimeRemaining() const;
private:

	UPROPERTY(EditAnywhere, Category = "World Swap")
	int32 MinWorldSwapTime = 5;

	UPROPERTY(EditAnywhere, Category = "World Swap")
	int32 MaxWorldSwapTime = 15;

	void ScheduleNextWorldSwap();

	UFUNCTION()
	void HandleWorldSwapTimerExpired();

	void UpdateWorldSwapCountdown();

	float GetWorldSwapDuration() const;

	float CurrentWorldSwapDuration = 0.f;

	float WorldSwapTimeRemaining = 0.f;

	FTimerHandle WorldSwapTimerHandle;

	FTimerHandle WorldSwapUITickHandle;

public:

	UFUNCTION(BlueprintCallable)
	void ResetWorldSwapTimer();
};



