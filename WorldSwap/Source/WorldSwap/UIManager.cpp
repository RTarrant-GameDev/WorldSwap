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
        UE_LOG(LogTemp, Error, TEXT("UIManager::Initialize: No GameInstance"));
        return;
    }

    UGameStateSubSystem* SM = GI->GetSubsystem<UGameStateSubSystem>();
    if (!SM)
    {
        UE_LOG(LogTemp, Error, TEXT("UIManager::Initialize: No GameStateSubSystem"));
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

void UUIManager::SetUIOnlyMode()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    PC->DisableInput(PC);
    PC->SetShowMouseCursor(true);

    FInputModeUIOnly InputMode;
    InputMode.SetWidgetToFocus(CurrentWidget->TakeWidget());
    PC->SetInputMode(InputMode);
}

void UUIManager::SetGameOnlyMode()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC) return;

    PC->EnableInput(PC);
    PC->SetShowMouseCursor(false);

    FInputModeGameOnly InputMode;
    PC->SetInputMode(InputMode);
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
        SetUIOnlyMode();
        break;
    case EGameState::Gameplay:
        SetGameOnlyMode();
        ShowHUD();
        break;
    case EGameState::Pause:
        ShowPauseMenu();
        SetUIOnlyMode();
        break;
    case EGameState::GameOver:
        ShowGameOverMenu();
        SetUIOnlyMode();
        break;
    case EGameState::LevelComplete:
        ShowLevelCompleteMenu();
        SetUIOnlyMode();
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