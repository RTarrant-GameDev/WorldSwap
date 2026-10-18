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

void AWorldSwapGameMode::HandlePlayerDeath()
{
	UWorldSwapGameInstance* GI = GetGameInstance<UWorldSwapGameInstance>();

	SubtractFromLivesCount();

	if (GI->GetLivesCount() <= 0)
	{
		GameOver();
	}
	else
	{
		RestartGame(false);
	}
}

void AWorldSwapGameMode::SetLivesCount(int32 LivesCountToSet)
{
	GetGameInstance<UWorldSwapGameInstance>()->SetLivesCount(LivesCountToSet);
}

void AWorldSwapGameMode::AddToLivesCount()
{
	GetGameInstance<UWorldSwapGameInstance>()->AddToLivesCount();;
}

void AWorldSwapGameMode::SubtractFromLivesCount()
{
	GetGameInstance<UWorldSwapGameInstance>()->SubtractFromLivesCount();
}

int32 AWorldSwapGameMode::GetLivesCount()
{
	return GetGameInstance<UWorldSwapGameInstance>()->LivesCount;
}

void AWorldSwapGameMode::SetWorldSwapSet(int32 swapState)
{
	WorldSwapState = swapState;
}

int32 AWorldSwapGameMode::GetWorldSwapState()
{
	return WorldSwapState;
}

void AWorldSwapGameMode::StartGame()
{
	TimerRef->StartTimer();
	UWorldSwapGameInstance* GI = Cast<UWorldSwapGameInstance>(GetGameInstance());

	GI->LivesCount = 3; // Reset lives ONLY here

	GI->GetSubsystem<UGameStateSubSystem>()->SetGameState(2);
}

void AWorldSwapGameMode::PauseGame()
{
	TimerRef->PauseTimer();

	UWorldSwapGameInstance* GI = Cast<UWorldSwapGameInstance>(GetGameInstance());

	GI->GetSubsystem<UGameStateSubSystem>()->SetGameState(3);
}

void AWorldSwapGameMode::ResumeGame()
{
	TimerRef->UnpauseTimer();

	UWorldSwapGameInstance* GI = Cast<UWorldSwapGameInstance>(GetGameInstance());

	GI->GetSubsystem<UGameStateSubSystem>()->SetGameState(2);
}

void AWorldSwapGameMode::ReturnToMainMenu()
{
	UWorldSwapGameInstance* GI = Cast<UWorldSwapGameInstance>(GetGameInstance());

	GI->GetSubsystem<UGameStateSubSystem>()->SetGameState(1);
	SetLivesCount(3);
	GI->bRestartAfterReload = false;
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("/Game/FirstLevel")));
}

void AWorldSwapGameMode::GameOver()
{
	UWorldSwapGameInstance* GI = Cast<UWorldSwapGameInstance>(GetGameInstance());
	GI->GetSubsystem<UGameStateSubSystem>()->SetGameState(4);
	
	TimerRef->ResetTimer();
}

void AWorldSwapGameMode::LevelComplete()
{
	TimerRef->ResetTimer();
	UWorldSwapGameInstance* GI = Cast<UWorldSwapGameInstance>(GetGameInstance());

	GI->GetSubsystem<UGameStateSubSystem>()->SetGameState(5);

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Blue, TEXT("Level completed!"));
	}
}

void AWorldSwapGameMode::RestartGame(bool wasGameOver)
{
	if (!wasGameOver) {
		APlayerController* PC = GetPlayerController();
		if (!PC) return;

		APawn* Pawn = PC->GetPawn();
		if (Pawn)
		{
			Pawn->Destroy();
		}

		RestartPlayer(PC); // built-in UE respawn
	}
	else {
		UWorldSwapGameInstance* GI = GetGameInstance<UWorldSwapGameInstance>();
		SetLivesCount(3);
		GI->bRestartAfterReload = true;
		UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("/Game/FirstLevel")));
		GI->GetSubsystem<UGameStateSubSystem>()->SetGameState(2);
	}
}

APlayerController* AWorldSwapGameMode::GetPlayerController()
{
	return UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void AWorldSwapGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UWorldSwapGameInstance* GI = GetGameInstance<UWorldSwapGameInstance>();
}

void AWorldSwapGameMode::BeginPlay()
{
	Super::BeginPlay();

	TimerRef = Cast<ATimerObject>(UGameplayStatics::GetActorOfClass(this, ATimerObject::StaticClass()));
	if (!TimerRef)
	{
		UE_LOG(LogTemp, Error, TEXT("TimerRef is NULL in BeginPlay!"));
	}

	UWorldSwapGameInstance* GI = GetGameInstance<UWorldSwapGameInstance>();
	if (!GI)
		return;

	UWorld* World = GetWorld();
	if (!World)
		return;

	// Ensure UIManager exists and is initialized
	if (!GI->GetUIManager())
	{
		GI->CreateUIManager(World);
	}
	else
	{
		GI->GetUIManager()->Initialize(World);
	}

	UGameStateSubSystem* SM = GI->GetSubsystem<UGameStateSubSystem>();
	if (!SM)
		return;

	// FIRST TIME EVER → MAIN MENU
	if (!GI->bHasInitializedFirstTimeSetup)
	{
		SetLivesCount(3);
		SM->SetGameState(1);
		APlayerController* PC = GetPlayerController();
		if (PC)
		{
			PC->bShowMouseCursor = true;
		}
		GI->bHasInitializedFirstTimeSetup = true;
		return;
	}

	// AFTER RESTART → GAMEPLAY
	if (GI->bRestartAfterReload)
	{
		SM->SetGameState(2); // Gameplay

		// Now the new TimerRef is valid → start fresh
		TimerRef->ResetTimer();
		TimerRef->StartTimer();

		GI->bRestartAfterReload = false;
		return;
	}
}
