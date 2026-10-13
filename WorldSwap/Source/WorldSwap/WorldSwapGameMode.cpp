// Copyright Epic Games, Inc. All Rights Reserved.

#include "WorldSwapGameMode.h"
#include "WorldSwapGameInstance.h"
#include "UIManager.h"
#include "GameStateSubSystem.h"
#include "Kismet/GameplayStatics.h"

AWorldSwapGameMode::AWorldSwapGameMode()
{
	// stub
}

void AWorldSwapGameMode::SetWorldSwapSet(int32 swapState)
{
	WorldSwapState = swapState;
	GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Blue, FString::Printf(TEXT("WorldSwapState: %s"), *FString::FromInt(WorldSwapState)));
}

int32 AWorldSwapGameMode::GetWorldSwapState()
{
	return WorldSwapState;
}

void AWorldSwapGameMode::StartGame()
{
	UWorldSwapGameInstance* GI = Cast<UWorldSwapGameInstance>(GetGameInstance());

	GI->GetSubsystem<UGameStateSubSystem>()->SetGameState(2);

	APlayerController* PC = GetPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = false;
		PC->SetPause(false);
	}
}

void AWorldSwapGameMode::PauseGame()
{
	UWorldSwapGameInstance* GI = Cast<UWorldSwapGameInstance>(GetGameInstance());

	GI->GetSubsystem<UGameStateSubSystem>()->SetGameState(3);

	APlayerController* PC = GetPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = true;
		PC->SetPause(true);
	}
}

void AWorldSwapGameMode::ResumeGame()
{
	UWorldSwapGameInstance* GI = Cast<UWorldSwapGameInstance>(GetGameInstance());

	GI->GetSubsystem<UGameStateSubSystem>()->SetGameState(2);

	APlayerController* PC = GetPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = false;
		PC->SetPause(false);
	}
}

void AWorldSwapGameMode::ReturnToMainMenu()
{
	UWorldSwapGameInstance* GI = Cast<UWorldSwapGameInstance>(GetGameInstance());

	GI->GetSubsystem<UGameStateSubSystem>()->SetGameState(1);
	APlayerController* PC = GetPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = true;
		PC->SetPause(true);
	}
}

void AWorldSwapGameMode::GameOver()
{
	UWorldSwapGameInstance* GI = Cast<UWorldSwapGameInstance>(GetGameInstance());

	GI->GetSubsystem<UGameStateSubSystem>()->SetGameState(4);

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Blue, TEXT("Congratulations, you're dead"));
	}

	APlayerController* PC = GetPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = true;
		PC->SetPause(true);
	}
}

void AWorldSwapGameMode::RestartGame()
{
	UWorldSwapGameInstance* GI = Cast<UWorldSwapGameInstance>(GetGameInstance());

	FName CurrentLevel = *GetWorld()->GetMapName();
	UGameplayStatics::OpenLevel(GetWorld(), CurrentLevel);
	
	GI->GetSubsystem<UGameStateSubSystem>()->SetGameState(2); 

	APlayerController* PC = GetPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = false;
		PC->SetPause(false);
	}
}

APlayerController* AWorldSwapGameMode::GetPlayerController()
{
	return UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void AWorldSwapGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// Get the persistent Game Instance

	UWorldSwapGameInstance* GI = GetGameInstance<UWorldSwapGameInstance>();

	UGameStateSubSystem* SM = GetGameInstance()->GetSubsystem<UGameStateSubSystem>();
	if (GI)
	{
		if (!GI->bHasInitializedFirstTimeSetup)
		{
			if (!GI->GetUIManager())
			{
				GI->CreateUIManager(GetWorld());
			}

			GI->GetUIManager()->Initialize(GetWorld());
			// ─── RUN YOUR ONE-TIME GLOBAL SETUP LOGIC HERE ───
			UE_LOG(LogTemp, Log, TEXT("Running global setup for the absolute first time!"));

			SM->SetGameState(1);

			// Mark it as done so it never triggers again during this session
			GI->bHasInitializedFirstTimeSetup = true;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Level reloaded, skipping one-time global setup."));
			SM->SetGameState(2);
		}
	}
}