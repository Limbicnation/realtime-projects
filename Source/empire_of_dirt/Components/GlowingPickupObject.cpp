// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GlowingPickupObject.h"
#include "Classes/Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AGlowingPickupObject::AGlowingPickupObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SM_Pickup = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMPickup"));

	BT_Pickup = CreateDefaultSubobject<UBoxComponent>(TEXT("BTPickup"));
	BT_Pickup->SetGenerateOverlapEvents(true);
	BT_Pickup->OnComponentBeginOverlap.AddDynamic(this, &AGlowingPickupObject::EnteredObjectRadius);
	BT_Pickup->OnComponentEndOverlap.AddDynamic(this, &AGlowingPickupObject::LeftObjectRadius);
}

// Called when the game starts or when spawned
void AGlowingPickupObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGlowingPickupObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGlowingPickupObject::ToggleGlow(bool IsGlowing)
{
	SM_Pickup->SetRenderCustomDepth(IsGlowing);
}

void AGlowingPickupObject::EnteredObjectRadius(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                               const FHitResult& SweepResult)
{
	ToggleGlow(true);
}

void AGlowingPickupObject::LeftObjectRadius(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ToggleGlow(false);
}
