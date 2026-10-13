// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameStateSubSystem.h"

void UUIManager::Initialize(UWorld* World)
{
    CachedWorld = World;

    UGameInstance* GI = UGameplayStatics::GetGameInstance(World);
    if (!GI)
    {
        return;
    }

    UGameStateSubSystem* SM = GI->GetSubsystem<UGameStateSubSystem>();
    if (!SM)
    {
        return;
    }

    SM->OnGameplayStateChanged.AddDynamic(this, &UUIManager::HandleStateChanged);
}

void UUIManager::HandleStateChanged(EGameState NewState)
{
    UE_LOG(LogTemp, Warning, TEXT("UIManager received state: %d (%p)"),
        (int32)NewState, this);
    switch (NewState)
    {
    case EGameState::Start:
        UE_LOG(LogTemp, Warning, TEXT("Start Menu Initiating"));
        ShowStartMenu();
        break;

    case EGameState::Gameplay:
        ShowHUD();
        break;

    case EGameState::Pause:
        ShowPauseMenu();
        break;

    case EGameState::GameOver:
        ShowGameOverMenu();
        break;

    default:
        break;
    }
}

static void ReplaceWidget(UWorld* World, UUserWidget*& CurrentWidget, const TCHAR* Path)
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }

    UClass* WidgetClass = LoadClass<UUserWidget>(nullptr, Path);
    if (!WidgetClass || !World)
    {
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("WidgetClass = %s"),
        WidgetClass ? *WidgetClass->GetName() : TEXT("NULL"));
    CurrentWidget = CreateWidget<UUserWidget>(World, WidgetClass);
    if (CurrentWidget)
    {
        CurrentWidget->AddToViewport();
    }
}

void UUIManager::ShowStartMenu()
{
    ReplaceWidget(CachedWorld, CurrentWidget, TEXT("/Game/Widgets/BP_MainMenuWidget.BP_MainMenuWidget_C"));
}

void UUIManager::ShowHUD()
{
    ReplaceWidget(CachedWorld, CurrentWidget, TEXT("/Game/Widgets/BP_PlayerHUDWidget.BP_PlayerHUDWidget_C"));
}

void UUIManager::ShowPauseMenu()
{
    ReplaceWidget(CachedWorld, CurrentWidget, TEXT("/Game/Widgets/BP_PauseMenuWidget.BP_PauseMenuWidget_C"));
}

void UUIManager::ShowGameOverMenu()
{
    ReplaceWidget(CachedWorld, CurrentWidget, TEXT("/Game/Widgets/BP_GameOverMenuWidget.BP_GameOverMenuWidget_C"));
}
