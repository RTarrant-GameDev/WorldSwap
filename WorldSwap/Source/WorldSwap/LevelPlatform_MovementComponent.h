// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LevelPlatform_MovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WORLDSWAP_API ULevelPlatform_MovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULevelPlatform_MovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = "Moving")
	bool PlatformMoves;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	bool PlatformRotates;

	UPROPERTY(EditAnywhere, Category = "Moving")
	FVector PlatformVelocity;

	UPROPERTY(EditAnywhere, Category = "Moving")
	float MoveDistance;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	FRotator RotationVelocity;

	FVector StartingLocation;

	void MovePlatform(float deltaTime);
	void RotatePlatform(float deltaTime);

	bool ShouldPlatformReturn() const;
	float GetDistanceMoved() const;
};
