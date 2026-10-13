// Fill out your copyright notice in the Description page of Project Settings.


#include "FallBoundary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFallBoundary::AFallBoundary()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	RootComponent = BoxComponent;
	BoxComponent->InitBoxExtent(FVector(2.5f, 2.5f, 2.5f));
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFallBoundary::OverlapBegin);
}

// Called when the game starts or when spawned
void AFallBoundary::BeginPlay()
{
	Super::BeginPlay();
}

void AFallBoundary::OverlapBegin(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, const 
	FHitResult& SweepResult
){

	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		AFallBoundary::Collide();
	}
}

// Called every frame
void AFallBoundary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFallBoundary::Collide()
{
	Cast<AWorldSwapGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->GameOver();
}

