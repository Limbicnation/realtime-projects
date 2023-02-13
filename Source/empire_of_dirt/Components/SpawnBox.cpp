// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SpawnBox.h"

// Sets default values
ASpawnBox::ASpawnBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Setup members
	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	RootComponent = SpawnBox;

}

// Called when the game starts or when spawned
void ASpawnBox::BeginPlay()
{
	Super::BeginPlay();

	// schedule first spawn
	if (ShouldSpawn)
	{
		ScheduleActorSpawn();
	}
	
}

void ASpawnBox::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Remove all timers associated with this objects instance
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

bool ASpawnBox::SpawnActor()
{	// Enable spawning
	bool SpawnedActor = true;
	if (ActorClassToBeSpawned)
	{
		// Calculate the extends of the box
		FBoxSphereBounds BoxBounds = SpawnBox->CalcBounds(GetActorTransform());

		// Compute random position within the box bounds
		FVector SpawnLocation = BoxBounds.Origin;
		SpawnLocation.X += -BoxBounds.BoxExtent.X + 2 * BoxBounds.BoxExtent.X * FMath::FRand();
		SpawnLocation.Y += -BoxBounds.BoxExtent.Y + 2 * BoxBounds.BoxExtent.Y * FMath::FRand();
		SpawnLocation.Z += -BoxBounds.BoxExtent.Z + 2 * BoxBounds.BoxExtent.Z * FMath::FRand();

		// Spawn the actor
		GetWorld()->SpawnActor(ActorClassToBeSpawned, &SpawnLocation);

	}

	return SpawnedActor;
}

void ASpawnBox::EnableActorSpawning(bool Enable)
{
	// Update internal 
	ShouldSpawn = Enable;

	// Check if timer action is done
	if (Enable)
	{
		ScheduleActorSpawn();
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	}

}

void ASpawnBox::ScheduleActorSpawn()
{
	// 1. Compute time offset to spawn
	float DeltaToNextSpawn = AvgSpawnTime + (-RandomSpawnTimeOffset + 2 * RandomSpawnTimeOffset * FMath::FRand());

	// 2. Schedule spawning
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ASpawnBox::SpawnActorScheduled, DeltaToNextSpawn, false);
}

void ASpawnBox::SpawnActorScheduled()
{
	// Spawn and Reschedule if required
	if (SpawnActor())
	{
		if (ShouldSpawn)
		{
			ScheduleActorSpawn();
		}
	}
	else
	{
		// TODO... log error
	}
}

