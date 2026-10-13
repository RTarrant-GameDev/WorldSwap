// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelPlatform_MovementComponent.h"
#include "WorldSwapGameMode.h"
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

	UPROPERTY(VisibleAnywhere, Category = "Mesh") //This adds it so you can see it in Blueprints
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Materials") //This adds it so you can see it in Blueprints
	UMaterialInterface* ActiveMaterial;

	UPROPERTY(EditAnywhere, Category = "Materials") //This adds it so you can see it in Blueprints
	UMaterialInterface* InactiveMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WorldSwap")
	int32 worldSwapNum; // Will always be set to either 0 (for WorldSwap State 1) or 1 (for WorldSwap State 2)

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool DoesWorldSwapNumMatch();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	AWorldSwapGameMode* GameMode;
};
