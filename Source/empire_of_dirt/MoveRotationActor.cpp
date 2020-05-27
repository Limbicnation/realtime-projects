// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveRotationActor.h"

// Sets default values
AMoveRotationActor::AMoveRotationActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PitchValue = 0.f;
	YawValue = 0.f;
	RollValue = 0.f;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("MyMesh");
}

// Called when the game starts or when spawned
void AMoveRotationActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMoveRotationActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move Actor
	FVector NewLocation = GetActorLocation();
	NewLocation.Z += 1.f * MeshMovement;
	SetActorLocation(NewLocation);

	// Rotate Actor
	FQuat QuatRotation = FQuat(FRotator(PitchValue, YawValue, DeltaTime));

	AddActorLocalRotation(QuatRotation, false, nullptr, ETeleportType::None);
}
