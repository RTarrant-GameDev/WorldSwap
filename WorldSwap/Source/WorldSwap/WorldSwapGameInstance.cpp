// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldSwapGameInstance.h"
#include "GameStateSubSystem.h"
#include "WorldSwapGameMode.h"
#include "UIManager.h"

void UWorldSwapGameInstance::Init()
{
    Super::Init();
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
    LivesCount = LivesCountToSet;
}

void UWorldSwapGameInstance::AddToLivesCount()
{
    LivesCount++;
}

void UWorldSwapGameInstance::SubtractFromLivesCount()
{
    LivesCount--;
}

int32 UWorldSwapGameInstance::GetLivesCount()
{
    return LivesCount;
}

void UWorldSwapGameInstance::HandleMapLoaded(UWorld* LoadedWorld)
{
    // Ensure UIManager exists and is initialized
    if (!UIManager)
    {
        CreateUIManager(LoadedWorld);
    }
    else
    {
        UIManager->Initialize(LoadedWorld);
    }

    UGameStateSubSystem* SM = GetSubsystem<UGameStateSubSystem>();
    if (!SM)
        return;

    // FIRST TIME LAUNCH → SHOW MAIN MENU
    if (!bHasInitializedFirstTimeSetup)
    {
        SM->SetGameState(1); // Main menu
        bHasInitializedFirstTimeSetup = true;
        return;
    }

    // AFTER RESTART → SHOW GAMEPLAY HUD
    SM->SetGameState(2);

    // Only respawn player if this was a "restart after game over"
    if (bRestartAfterReload)
    {
        APlayerController* PC = LoadedWorld->GetFirstPlayerController();
        if (PC)
        {
            AWorldSwapGameMode* GM = LoadedWorld->GetAuthGameMode<AWorldSwapGameMode>();
            GM->RestartPlayer(PC);

            PC->bShowMouseCursor = false;
            PC->SetPause(false);
        }

        bRestartAfterReload = false;
    }
}
