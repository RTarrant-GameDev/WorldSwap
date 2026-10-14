#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameStateSubSystem.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
    Default = 0 UMETA(DisplayName = "Default"),
    Start = 1 UMETA(DisplayName = "Start"),
    Gameplay = 2 UMETA(DisplayName = "Gameplay"),
    Pause = 3 UMETA(DisplayName = "Pause"),
    GameOver = 4 UMETA(DisplayName = "GameOver"),
    LevelComplete = 5 UMETA(DisplayName = "LevelComplete")
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
    void SetGameState(int32 stateNum);

    UFUNCTION(BlueprintCallable)
    EGameState GetState() const { return CurrentState; }

private:
    EGameState CurrentState = EGameState::Default;
};
