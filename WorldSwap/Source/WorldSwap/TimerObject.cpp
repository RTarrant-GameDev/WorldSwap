// Fill out your copyright notice in the Description page of Project Settings.

#include "TimerObject.h"
#include "WorldSwapGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATimerObject::ATimerObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATimerObject::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ATimerObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATimerObject::StartTimer()
{
	// Ensure the timer is fully cleared before starting
	GetWorldTimerManager().ClearTimer(CountdownTimerHandle);

	// Reset the remaining time if you want a fresh start
	TimeRemaining = TimerValue;

	// Now start the timer
	GetWorldTimerManager().SetTimer(
		CountdownTimerHandle,
		this,
		&ATimerObject::CountdownTimer,
		1.0f,
		true
	);
}

void ATimerObject::ResetTimer()
{
	StopTimer();
	TimeRemaining = TimerValue;
}

void ATimerObject::PauseTimer()
{
	if (GetWorldTimerManager().IsTimerActive(CountdownTimerHandle))
	{
		GetWorldTimerManager().PauseTimer(CountdownTimerHandle);
	}
}

void ATimerObject::UnpauseTimer()
{
	if (GetWorldTimerManager().IsTimerPaused(CountdownTimerHandle))
	{
		GetWorldTimerManager().UnPauseTimer(CountdownTimerHandle);
	}
}

void ATimerObject::StopTimer()
{
	GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
	TimeRemaining = TimerValue;
}

void ATimerObject::CountdownTimer()
{
	--TimeRemaining;

	if (TimeRemaining <= 0.0f) {
		AWorldSwapGameMode* GM = Cast<AWorldSwapGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GM)
		{
			GM->GameOver();
		}
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
	}
}

float ATimerObject::GetTimeRemaining() const
{
	return TimeRemaining;
}

void ATimerObject::SetTimeRemaining(float TimerValueToSet)
{
	TimeRemaining = TimerValueToSet;
}

FString ATimerObject::DisplayTimeToTimer(float TimeToDisplay)
{
	//Could easily wrap lines 34-39 into it's own function, 
	// but it's only ever going to used for one level, so not much point
	int32 TotalSeconds = FMath::RoundToInt(TimeToDisplay);
	int32 Minutes = TotalSeconds / 60;
	int32 SecondsRemaining = TotalSeconds % 60;

	FString MMSS = FString::Printf(TEXT("%02d:%02d"), Minutes, SecondsRemaining);

	return MMSS;
}

