// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameStateSubSystem.h"

void UUIManager::Initialize(UWorld* World)
{
    UE_LOG(LogTemp, Warning, TEXT("UIManager::Initialize, World = %s"), *GetNameSafe(World));

    UGameInstance* GI = UGameplayStatics::GetGameInstance(World);
    if (!GI)
    {
        UE_LOG(LogTemp, Error, TEXT("UIManager::Initialize: No GameInstance"));
        return;
    }

    UGameStateSubSystem* SM = GI->GetSubsystem<UGameStateSubSystem>();
    if (!SM)
    {
        UE_LOG(LogTemp, Error, TEXT("UIManager::Initialize: No GameStateSubSystem"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("UIManager: binding to OnGameplayStateChanged"));
    SM->OnGameplayStateChanged.AddDynamic(this, &UUIManager::HandleStateChanged);
}

void UUIManager::HandleStateChanged(EGameState NewState)
{
    UE_LOG(LogTemp, Warning, TEXT("UIManager::HandleStateChanged: NewState = %d"), (int32)NewState);
    PendingState = NewState;

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("HandleStateChanged: World invalid"));
        return;
    }

    World->GetTimerManager().SetTimerForNextTick(this, &UUIManager::ProcessStateChange);
}

static void ReplaceWidget(UUserWidget*& CurrentWidget, const TCHAR* Path, UWorld* World)
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
    UE_LOG(LogTemp, Warning, TEXT("UIManager::ProcessStateChange: PendingState = %d"), (int32)PendingState);

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("ProcessStateChange: World invalid"));
        return;
    }

    switch (PendingState)
    {
    case EGameState::Start:
        UE_LOG(LogTemp, Warning, TEXT("Showing Start Menu"));
        ShowStartMenu();
        break;
    case EGameState::Gameplay:
        UE_LOG(LogTemp, Warning, TEXT("Showing HUD"));
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

UWorld* UUIManager::GetWorld() const
{
    if (const UObject* Outer = GetOuter())
    {
        return Outer->GetWorld();
    }

    return nullptr;
}