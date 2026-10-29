// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "WorldSwapGameInstance.h"
#include "GameStateSubSystem.h"

void AMainMenuGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (UWorldSwapGameInstance* GI =
        Cast<UWorldSwapGameInstance>(GetGameInstance()))
    {
        if (!GI->GetUIManager())
        {
            GI->CreateUIManager(GetWorld());
        }

        if (UGameStateSubSystem* SM =
            GI->GetSubsystem<UGameStateSubSystem>())
        {
            SM->SetGameState(EGameState::Start);
        }
    }
}
