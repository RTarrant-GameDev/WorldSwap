// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelPlatform.h"

// Sets default values
ALevelPlatform::ALevelPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlatformMovementComponent = CreateDefaultSubobject<ULevelPlatform_MovementComponent>(TEXT("PlatformMovementComponent"));

}

// Called when the game starts or when spawned
void ALevelPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ALevelPlatform::GetWorldSwapState()
{
	return worldSwapNum;
}

