// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelCollectable.h"
#include "WorldSwapGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelCollectable::ALevelCollectable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetGenerateOverlapEvents(true);
	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ALevelCollectable::OverlapBegin);
}

// Called when the game starts or when spawned
void ALevelCollectable::BeginPlay()
{
	Super::BeginPlay();
}

void ALevelCollectable::OverlapBegin(
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

// Called every frame
void ALevelCollectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelCollectable::Collide()
{
	AWorldSwapGameMode* GM =
		Cast<AWorldSwapGameMode>(
			UGameplayStatics::GetGameMode(GetWorld()));

	if (CollectableType == ECollectableType::Life) {
		if (GM)
		{
			GM->AddToLivesCount();
		}
	}
	else if (CollectableType == ECollectableType::Key) {
		if (GM) {
			GM->CollectablePickedUp();
		}
	}
	this->Destroy();
}

