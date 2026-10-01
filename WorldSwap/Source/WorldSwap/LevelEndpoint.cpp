// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelEndpoint.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelEndpoint::ALevelEndpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	RootComponent = BoxComponent;
	BoxComponent->InitBoxExtent(FVector(2.5f, 2.5f, 2.5f));
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALevelEndpoint::OverlapBegin);
}

// Called when the game starts or when spawned
void ALevelEndpoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALevelEndpoint::OverlapBegin(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult
){
	ALevelEndpoint::Collide();
}

// Called every frame
void ALevelEndpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelEndpoint::Collide()
{
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Collision detected!"));
	}
}

