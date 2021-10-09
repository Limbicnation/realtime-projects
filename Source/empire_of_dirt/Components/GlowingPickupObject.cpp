// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GlowingPickupObject.h"
#include "Classes/Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Classes/Components/ShapeComponent.h"


// Sets default values
AGlowingPickupObject::AGlowingPickupObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Pickup = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMPickup"));

	BT_Pickup = CreateDefaultSubobject<UBoxComponent>(TEXT("BTPickup"));
	BT_Pickup->SetGenerateOverlapEvents(true);
	BT_Pickup->OnComponentBeginOverlap.AddDynamic(this, &AGlowingPickupObject::EnteredObjectRadius);
	BT_Pickup->OnComponentEndOverlap.AddDynamic(this, &AGlowingPickupObject::LeftObjectRadius);

	PickupRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PickupRoot"));
	RootComponent = PickupRoot;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	PickupMesh->SetupAttachment(PickupRoot); FAttachmentTransformRules (EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget, true);

	PickupBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupBox"));
	PickupBox->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	PickupBox->SetGenerateOverlapEvents(true);
	PickupBox->OnComponentBeginOverlap.AddDynamic(this, &AGlowingPickupObject::OnPlayerEnteredPickupBox);
	PickupMesh->SetupAttachment(PickupRoot); FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
}

void AGlowingPickupObject::OnPlayerEnteredPickupBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                    bool bFromSweep, const FHitResult& SweepResult)
{
	/* To Do -> Add pickup to inventory*/
	Destroy();
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
	Pickup->SetRenderCustomDepth(IsGlowing);
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
