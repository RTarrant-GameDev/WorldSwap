#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameStateSubSystem.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
    Default,
    Start,
    Gameplay,
    Pause,
    GameOver,
    LevelComplete,
    LevelSelection
};

// -------------------------------
// ADD THESE — they were missing
// -------------------------------
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayStateChanged, EGameState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayStateEntered, EGameState, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayStateExited, EGameState, State);
// -------------------------------

UCLASS()
class WORLDSWAP_API UGameStateSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

    UPROPERTY(BlueprintAssignable)
    FOnGameplayStateChanged OnGameplayStateChanged;

    UPROPERTY(BlueprintAssignable)
    FOnGameplayStateEntered OnStateEntered;

    UPROPERTY(BlueprintAssignable)
    FOnGameplayStateExited OnStateExited;

    UFUNCTION(BlueprintCallable)
    void SetGameState(EGameState NewState);

    UFUNCTION(BlueprintCallable)
    EGameState GetState() const { return CurrentState; }

private:
    EGameState CurrentState = EGameState::Default;
};
