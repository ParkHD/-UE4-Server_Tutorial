// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformTrigger.h"

#include "MovingPlatform_SMActor.h"
#include "Components/BoxComponent.h"

// Sets default values
APlatformTrigger::APlatformTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>("TrrigerVolume");
	RootComponent = TriggerVolume;

}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();

	TriggerVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &APlatformTrigger::OnBeginOverlapEvent);
	TriggerVolume->OnComponentEndOverlap.AddUniqueDynamic(this, &APlatformTrigger::OnEndOverlapEvent);
}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlatformTrigger::OnBeginOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for(auto Platform :  PlatformsToTrigger)
	{
		Platform->AddActiveTrigger();
	}
}

void APlatformTrigger::OnEndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	for (auto Platform : PlatformsToTrigger)
	{
		Platform->RemoveActiveTrigger();
	}
}

