// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelPlatform_MovementComponent.h"

// Sets default values for this component's properties
ULevelPlatform_MovementComponent::ULevelPlatform_MovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULevelPlatform_MovementComponent::BeginPlay()
{
	Super::BeginPlay();

	StartingLocation = GetOwner()->GetActorLocation();
	
}


// Called every frame
void ULevelPlatform_MovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PlatformMoves) {
		MovePlatform(DeltaTime);
	} 

	if (PlatformRotates) {
		RotatePlatform(DeltaTime);
	}
}

void ULevelPlatform_MovementComponent::MovePlatform(float deltaTime)
{
	if (ShouldPlatformReturn()) {
		FVector MoveDirction = PlatformVelocity.GetSafeNormal();
		StartingLocation = StartingLocation + MoveDirction * MoveDistance;
		GetOwner()->SetActorLocation(StartingLocation);
		PlatformVelocity = -PlatformVelocity;
	}
	else {
		FVector currentLocation = GetOwner()->GetActorLocation();
		currentLocation += (PlatformVelocity * deltaTime);
		GetOwner()->SetActorLocation(currentLocation);
	}
}

void ULevelPlatform_MovementComponent::RotatePlatform(float deltaTime)
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation += (RotationVelocity * deltaTime);
	GetOwner()->AddActorLocalRotation(RotationVelocity * deltaTime);
}

bool ULevelPlatform_MovementComponent::ShouldPlatformReturn() const
{
	return (GetDistanceMoved() > MoveDistance);
}

float ULevelPlatform_MovementComponent::GetDistanceMoved() const
{
	return FVector::Dist(StartingLocation, GetOwner()->GetActorLocation());
}

