// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateSubSystem.h"
#include "Kismet/GameplayStatics.h"

void UGameStateSubSystem::SetGameState(EGameState NewState)
{
    if (CurrentState != NewState)
    {
        OnStateExited.Broadcast(CurrentState);

        CurrentState = NewState;

        OnStateEntered.Broadcast(NewState);
        OnGameplayStateChanged.Broadcast(NewState);
    }

    if (CurrentState == EGameState::GameOver && NewState == EGameState::Gameplay) {
        FName CurrentLevel = *GetWorld()->GetMapName();
        UGameplayStatics::OpenLevel(GetWorld(), CurrentLevel);
    }
}
