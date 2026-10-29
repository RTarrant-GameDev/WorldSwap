// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelObstacle.h"
#include "Kismet/GameplayStatics.h"
#include "WorldSwapGameMode.h"

// Sets default values
ALevelObstacle::ALevelObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetGenerateOverlapEvents(true);
	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ALevelObstacle::OverlapBegin);
}

// Called when the game starts or when spawned
void ALevelObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALevelObstacle::OverlapBegin(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult
){
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		Collide();
	}
}

void ALevelObstacle::Collide()
{
	AWorldSwapGameMode* GM = Cast<AWorldSwapGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->HandlePlayerDeath();
	}
}

// Called every frame
void ALevelObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

