// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameStateSubSystem.h"

void UUIManager::Initialize(UWorld* World)
{
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
    PendingState = NewState;

    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    World->GetTimerManager().SetTimerForNextTick(this, &UUIManager::ProcessStateChange);
}

static void ReplaceWidget(UUserWidget*& CurrentWidget, const TCHAR* Path, UWorld* World)
{
    ULocalPlayer* LP = World->GetFirstLocalPlayerFromController();
    if (!LP)
    {
        UE_LOG(LogTemp, Error, TEXT("ReplaceWidget: No LocalPlayer yet — deferring"));
        return;
    }


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

    UE_LOG(LogTemp, Warning, TEXT("ReplaceWidget: World = %s, IsValid = %d"),
        *GetNameSafe(World),
        IsValid(World));

    UE_LOG(LogTemp, Warning, TEXT("ReplaceWidget: WidgetClass = %s, IsValid = %d"),
        *GetNameSafe(WidgetClass),
        IsValid(WidgetClass));

    CurrentWidget = CreateWidget<UUserWidget>(World, WidgetClass);
    if (CurrentWidget)
    {
        CurrentWidget->AddToViewport();
    }
}

void UUIManager::ShowStartMenu()
{
    ReplaceWidget(CurrentWidget, TEXT("/Game/Widgets/BP_MainMenuWidget.BP_MainMenuWidget_C"), GetWorld());
}

void UUIManager::ShowHUD()
{
    ReplaceWidget(CurrentWidget, TEXT("/Game/Widgets/BP_PlayerHUDWidget.BP_PlayerHUDWidget_C"), GetWorld());
}

void UUIManager::ShowPauseMenu()
{
    ReplaceWidget(CurrentWidget, TEXT("/Game/Widgets/BP_PauseMenuWidget.BP_PauseMenuWidget_C"), GetWorld());
}

void UUIManager::ShowGameOverMenu()
{
    ReplaceWidget(CurrentWidget, TEXT("/Game/Widgets/BP_GameOverMenuWidget.BP_GameOverMenuWidget_C"), GetWorld());
}

void UUIManager::ShowLevelCompleteMenu()
{
    ReplaceWidget(CurrentWidget, TEXT("/Game/Widgets/BP_LevelCompleteWidget.BP_LevelCompleteWidget_C"), GetWorld());
}

void UUIManager::ProcessStateChange()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("ProcessStateChange: World invalid"));
        return;
    }

    switch (PendingState)
    {
    case EGameState::Start:
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
    case EGameState::LevelComplete:
        ShowLevelCompleteMenu();
        break;
    }
}
