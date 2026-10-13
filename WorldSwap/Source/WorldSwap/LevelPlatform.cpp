// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelPlatform.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelPlatform::ALevelPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	PlatformMovementComponent = CreateDefaultSubobject<ULevelPlatform_MovementComponent>(TEXT("PlatformMovementComponent"));
}

// Called when the game starts or when spawned
void ALevelPlatform::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AWorldSwapGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

bool ALevelPlatform::DoesWorldSwapNumMatch()
{
	return worldSwapNum == GameMode->GetWorldSwapState();
}

// Called every frame
void ALevelPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DoesWorldSwapNumMatch() == true) {
		StaticMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		StaticMeshComponent->SetMaterial(0, ActiveMaterial);
	} else {
		StaticMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		StaticMeshComponent->SetMaterial(0, InactiveMaterial);
	}
}

