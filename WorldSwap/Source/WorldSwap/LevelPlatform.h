// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelPlatform_MovementComponent.h"
#include "LevelPlatform.generated.h"

class ULevelPlatform_MovementComponent;

UCLASS()
class WORLDSWAP_API ALevelPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelPlatform();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="Platform Movement")
	ULevelPlatform_MovementComponent* PlatformMovementComponent;

protected:
	int32 worldSwapNum; // Will always be set to either 0 (for WorldSwap State 1) or 1 (for WorldSwap State 2)

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int32 GetWorldSwapState();
};
