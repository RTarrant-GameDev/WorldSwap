// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WorldSwapGameMode.generated.h"

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
	UFUNCTION(BlueprintCallable, Category = "WorldSwap")
	void SetWorldSwapSet(int32 swapState);

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
	void RestartGame();

	APlayerController* GetPlayerController();

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};



