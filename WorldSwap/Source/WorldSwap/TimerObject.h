// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerObject.generated.h"

UCLASS()
class WORLDSWAP_API ATimerObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimerObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimerValue = 300.f;

	float TimeRemaining;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	FTimerHandle CountdownTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Time")
	void StartTimer();

	UFUNCTION(BlueprintCallable, Category = "Time")
	void ResetTimer();

	UFUNCTION(BlueprintCallable, Category = "Time")
	void PauseTimer();

	UFUNCTION(BlueprintCallable, Category = "Time")
	void UnpauseTimer();

	UFUNCTION(BlueprintCallable, Category = "Time")
	void StopTimer();

	UFUNCTION(BlueprintCallable, Category = "Time")
	float GetTimeRemaining() const;

	UFUNCTION(BlueprintCallable, Category = "Time")
	void SetTimeRemaining(float TimerValueToSet);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	FString DisplayTimeToTimer(float TimeToDisplay);

	void CountdownTimer();
};
