// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldSwapGameInstance.h"
#include "GameStateSubSystem.h"
#include "UIManager.h"

void UWorldSwapGameInstance::Init()
{
    Super::Init();

    if (!UIManager)
    {
        UIManager = NewObject<UUIManager>(this);
    }
}

void UWorldSwapGameInstance::CreateUIManager(UWorld* World)
{
    UIManager = NewObject<UUIManager>(this);
}