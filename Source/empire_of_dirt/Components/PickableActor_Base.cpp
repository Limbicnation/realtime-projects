// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/PickableActor_Base.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"

// Sets default values
APickableActor_Base::APickableActor_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Create the static mesh component
	UStaticMeshComponent* MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetMobility(EComponentMobility::Movable);
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// initialize the sphere component
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	MeshComp->SetupAttachment(CollisionComp);
	SetRootComponent(MeshComp);

	// initialize the sphere component with a default radius of 30.0f
	CollisionComp->InitSphereRadius(30.0f);

	// Set collision comp to overlap with all dynamic
	CollisionComp->BodyInstance.SetCollisionProfileName("OverlapAllDynamic");

	// Set collision comp as root component
	RootComponent = CollisionComp;

	/*MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/

	// Add rotation
	RotationComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotationComp"));

}

// Called when the game starts or when spawned
void APickableActor_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

