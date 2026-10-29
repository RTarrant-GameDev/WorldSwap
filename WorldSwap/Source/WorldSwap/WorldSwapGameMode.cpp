// Copyright Epic Games, Inc. All Rights Reserved.

#include "WorldSwapGameMode.h"
#include "WorldSwapGameInstance.h"
#include "LevelCollectable.h"
#include "UIManager.h"
#include "GameStateSubSystem.h"
#include "Kismet/GameplayStatics.h"

AWorldSwapGameMode::AWorldSwapGameMode()
{
	// stub
}

void AWorldSwapGameMode::HandlePlayerDeath()
{
	SubtractFromLivesCount();

	if (GetWSGI()->GetLivesCount() <= 0)
	{
		GameOver();
	}
	else
	{
		RestartGame(false);
	}
}

void AWorldSwapGameMode::AddToLivesCount()
{
	GetWSGI()->AddToLivesCount();
}

void AWorldSwapGameMode::SubtractFromLivesCount()
{
	GetWSGI()->SubtractFromLivesCount();
}

int32 AWorldSwapGameMode::GetLivesCount()
{
	return GetWSGI()->GetLivesCount();
}

void AWorldSwapGameMode::SetWorldSwapState(int32 swapState)
{
	WorldSwapState = swapState;

	ResetWorldSwapTimer();
}

int32 AWorldSwapGameMode::GetWorldSwapState()
{
	return WorldSwapState;
}

void AWorldSwapGameMode::StartGame()
{
	TimerRef->StartTimer();
	ScheduleNextWorldSwap();

	GetWSGI()->SetLivesCount(3); // Reset lives ONLY here
	if (UGameStateSubSystem* SM = GetGameStateSM())
	{
		SM->SetGameState(EGameState::Gameplay);
	}
}

void AWorldSwapGameMode::PauseGame()
{
	TimerRef->PauseTimer();

	GetWorld()->GetTimerManager().PauseTimer(
		WorldSwapTimerHandle);

	if (UGameStateSubSystem* SM = GetGameStateSM())
	{
		SM->SetGameState(EGameState::Pause);
	}
}

void AWorldSwapGameMode::ResumeGame()
{
	TimerRef->UnpauseTimer();
	GetWorld()->GetTimerManager().UnPauseTimer(
		WorldSwapTimerHandle);
	if (UGameStateSubSystem* SM = GetGameStateSM())
	{
		SM->SetGameState(EGameState::Gameplay);
	}
}

void AWorldSwapGameMode::ReturnToMainMenu()
{
	GetWorld()->GetTimerManager().ClearTimer(
		WorldSwapTimerHandle);

	GetWSGI()->LoadMainMenu();
}

void AWorldSwapGameMode::GameOver()
{
	if (UGameStateSubSystem* SM = GetGameStateSM())
	{
		SM->SetGameState(EGameState::GameOver);
	}
	
	TimerRef->ResetTimer();
	GetWorld()->GetTimerManager().ClearTimer(
		WorldSwapTimerHandle);
}

void AWorldSwapGameMode::LevelComplete()
{
	TimerRef->ResetTimer();
	GetWorld()->GetTimerManager().ClearTimer(
		WorldSwapTimerHandle);

	if (UGameStateSubSystem* SM = GetGameStateSM())
	{
		SM->SetGameState(EGameState::LevelComplete);
	}

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
		GetWSGI()->SetLivesCount(3);
		GetWSGI()->bRestartAfterReload = true;
		UGameplayStatics::OpenLevel(GetWorld(), FName(GetWorld()->GetMapName()));
		if (UGameStateSubSystem* SM = GetGameStateSM())
		{
			SM->SetGameState(EGameState::Gameplay);
		}
	}
}

APlayerController* AWorldSwapGameMode::GetPlayerController()
{
	return UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void AWorldSwapGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AWorldSwapGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning,
		TEXT("MAP = %s"),
		*GetWorld()->GetMapName());

	InitTimer();
	InitCollectables();
	InitUI();
	InitGameState();
	StartGame();
}

UWorldSwapGameInstance* AWorldSwapGameMode::GetWSGI() const
{
	return Cast<UWorldSwapGameInstance>(GetGameInstance());
}

UGameStateSubSystem* AWorldSwapGameMode::GetGameStateSM() const
{
	if (UWorldSwapGameInstance* GI = GetWSGI())
	{
		return GI->GetSubsystem<UGameStateSubSystem>();
	}

	return nullptr;
}

void AWorldSwapGameMode::InitCollectables()
{
	LevelEndpointRef =
		Cast<ALevelEndpoint>(
			UGameplayStatics::GetActorOfClass(
				GetWorld(),
				ALevelEndpoint::StaticClass()));

	if (LevelEndpointRef) {
		TArray<AActor*> Collectables;

		UGameplayStatics::GetAllActorsOfClass(
			GetWorld(),
			ALevelCollectable::StaticClass(),
			Collectables);

		for (AActor* Actor : Collectables)
		{
			ALevelCollectable* Collectable = Cast<ALevelCollectable>(Actor);

			if (!Collectable)
			{
				continue;
			}

			if (Collectable->CollectableType == ECollectableType::Key)
			{
				TotalCollectablesRequired++;
			}
		}

		CurrentCollectablesCollected = 0;

		if (TotalCollectablesRequired == 0)
		{
			if (LevelEndpointRef)
			{
				LevelEndpointRef->SetActive(true);
			}
		}
	}

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Total keys required: %d"),
		TotalCollectablesRequired);
}

void AWorldSwapGameMode::InitTimer()
{
	TimerRef = Cast<ATimerObject>(UGameplayStatics::GetActorOfClass(this, ATimerObject::StaticClass()));
	if (!TimerRef)
	{
		UE_LOG(LogTemp, Error, TEXT("TimerRef is NULL in BeginPlay!"));
	}
}

void AWorldSwapGameMode::InitUI()
{
	UWorld* World = GetWorld();
	if (!World)
		return;

	// Ensure UIManager exists and is initialized
	if (!GetWSGI()->GetUIManager())
	{
		GetWSGI()->CreateUIManager(World);
	}
	else
	{
		GetWSGI()->GetUIManager()->Initialize(World);
	}
}

void AWorldSwapGameMode::InitGameState()
{
	if (UGameStateSubSystem* SM = GetGameStateSM())
	{
		SM->SetGameState(EGameState::Gameplay);
	}
}

void AWorldSwapGameMode::ScheduleNextWorldSwap()
{
	const int32 RandomDelay =
		FMath::RandRange(
			MinWorldSwapTime,
			MaxWorldSwapTime);

	CurrentWorldSwapDuration =
		FMath::RandRange(5, 15);

	WorldSwapTimeRemaining =
		CurrentWorldSwapDuration;

	GetWorld()->GetTimerManager().SetTimer(
		WorldSwapTimerHandle,
		this,
		&AWorldSwapGameMode::HandleWorldSwapTimerExpired,
		CurrentWorldSwapDuration,
		false);
}

void AWorldSwapGameMode::HandleWorldSwapTimerExpired()
{
	const int32 NewState =
		(WorldSwapState == 0)
		? 1
		: 0;

	SetWorldSwapState(NewState);

	ScheduleNextWorldSwap();
}

void AWorldSwapGameMode::UpdateWorldSwapCountdown()
{
	WorldSwapTimeRemaining =
		GetWorld()->GetTimerManager().GetTimerRemaining(
			WorldSwapTimerHandle);

	if (WorldSwapTimeRemaining < 0.f)
	{
		WorldSwapTimeRemaining = 0.f;
	}
}

int32 AWorldSwapGameMode::GetWorldSwapTimeRemaining() const
{
	if (!GetWorld())
	{
		return 0;
	}

	const float Remaining =
		GetWorld()->GetTimerManager().GetTimerRemaining(
			WorldSwapTimerHandle);

	return FMath::Max(0, FMath::CeilToInt(Remaining));
}

float AWorldSwapGameMode::GetWorldSwapDuration() const
{
	return CurrentWorldSwapDuration;
}

void AWorldSwapGameMode::ResetWorldSwapTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(
		WorldSwapTimerHandle);

	ScheduleNextWorldSwap();
}

void AWorldSwapGameMode::CollectablePickedUp()
{
	CurrentCollectablesCollected++;

	if (CurrentCollectablesCollected >= TotalCollectablesRequired)
	{
		if (LevelEndpointRef)
		{
			LevelEndpointRef->SetActive(true);
		}
	}
}

void AWorldSwapGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearAllTimersForObject(this);
}