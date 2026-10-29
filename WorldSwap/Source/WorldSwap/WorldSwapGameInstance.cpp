// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldSwapGameInstance.h"
#include "GameStateSubSystem.h"
#include "WorldSwapGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UIManager.h"

void UWorldSwapGameInstance::Init()
{
    Super::Init();

    Levels = {
        TEXT("Level1"),
        TEXT("Level2"),
        TEXT("Level3"),
        TEXT("Level4"),
        TEXT("Level5"),
        TEXT("Level6")
    };

    for (const FName& Level : Levels)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Stored Level = %s"),
            *Level.ToString());
    }
}

void UWorldSwapGameInstance::CreateUIManager(UWorld* World)
{
    if (!UIManager)
    {
        UIManager = NewObject<UUIManager>(this);
    }

    if (UIManager)
    {
        UIManager->Initialize(World);
    }
}

void UWorldSwapGameInstance::SetLivesCount(int32 LivesCountToSet)
{
    LivesCount = FMath::Max(0, LivesCountToSet);
}

void UWorldSwapGameInstance::AddToLivesCount()
{
    ++LivesCount;
}

void UWorldSwapGameInstance::SubtractFromLivesCount()
{
    LivesCount = FMath::Max(0, LivesCount - 1);
}

int32 UWorldSwapGameInstance::GetLivesCount()
{
    return LivesCount;
}

void UWorldSwapGameInstance::StartGameFromFirstLevel()
{
    LoadLevel(TEXT("Level1"));
}

void UWorldSwapGameInstance::LoadBonusLevel()
{
    LoadLevel(TEXT("FirstLevel"));
}

void UWorldSwapGameInstance::LoadNextLevel()
{
    const int32 NextIndex = CurrentLevelIndex + 1;

    if (!Levels.IsValidIndex(NextIndex))
    {
        LoadMainMenu();
        return;
    }

    LoadLevel(Levels[NextIndex].ToString());
}

void UWorldSwapGameInstance::LoadLevel(FString LevelName)
{
    UE_LOG(LogTemp, Warning,
        TEXT("LoadLevel: %s"),
        *LevelName);

    CurrentLevelIndex =
        Levels.IndexOfByKey(FName(*LevelName));

    UE_LOG(LogTemp, Warning,
        TEXT("CurrentLevelIndex set to %d"),
        CurrentLevelIndex);

    if (UIManager)
    {
        UIManager->Shutdown();
    }

    UGameplayStatics::OpenLevel(
        GetWorld(),
        FName(*LevelName));
}

void UWorldSwapGameInstance::LoadMainMenu()
{
    if (UIManager)
    {
        UIManager->Shutdown();
    }

    UGameplayStatics::OpenLevel(
        GetWorld(),
        TEXT("MainMenu")
    );
}

void UWorldSwapGameInstance::HandleMapLoaded(UWorld* LoadedWorld)
{
    UE_LOG(LogTemp, Warning,
        TEXT("HandleMapLoaded Called"));

    if (!LoadedWorld)
    {
        return;
    }

    // Determine which level we just loaded
    const FName CurrentMapName(
        *UGameplayStatics::GetCurrentLevelName(
            LoadedWorld,
            true));

    CurrentLevelIndex =
        Levels.IndexOfByKey(CurrentMapName);

    UE_LOG(LogTemp, Warning,
        TEXT("Loaded Level: %s (Index %d)"),
        *CurrentMapName.ToString(),
        CurrentLevelIndex);

    if (!UIManager)
    {
        CreateUIManager(LoadedWorld);
    }
    else
    {
        UIManager->Initialize(LoadedWorld);
    }

    if (bRestartAfterReload)
    {
        APlayerController* PC =
            LoadedWorld->GetFirstPlayerController();

        if (PC)
        {
            if (AWorldSwapGameMode* GM =
                LoadedWorld->GetAuthGameMode<AWorldSwapGameMode>())
            {
                GM->RestartPlayer(PC);
            }

            PC->bShowMouseCursor = false;
            PC->SetPause(false);
        }

        bRestartAfterReload = false;
    }
}
