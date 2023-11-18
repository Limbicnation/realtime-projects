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

	// Schedule first spawn
	if (ShouldSpawn)
	{
		ScheduleActorSpawn();
	}
}

void ASpawnBox::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Remove all timers associated with this object's instance
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

bool ASpawnBox::SpawnActor()
{
	// Enable spawning
	bool bSpawnedActor = true;
	if (ActorClassToBeSpawned)
	{
		// Calculate the extents of the box
		FBoxSphereBounds BoxBounds = SpawnBox->CalcBounds(GetActorTransform());

		// Compute random position within the box bounds
		FVector SpawnLocation = BoxBounds.Origin;
		SpawnLocation.X += -BoxBounds.BoxExtent.X + 2 * BoxBounds.BoxExtent.X * FMath::FRand();
		SpawnLocation.Y += -BoxBounds.BoxExtent.Y + 2 * BoxBounds.BoxExtent.Y * FMath::FRand();
		SpawnLocation.Z += -BoxBounds.BoxExtent.Z + 2 * BoxBounds.BoxExtent.Z * FMath::FRand();

		// Spawn the actor
		SpawnedActor = GetWorld()->SpawnActor(ActorClassToBeSpawned, &SpawnLocation);

		// Get the child static mesh component of the spawned actor
		UStaticMeshComponent* ChildStaticMesh = Cast<UStaticMeshComponent>(SpawnedActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		if (ChildStaticMesh)
		{
			// Generate a different random scale for each instance of the spawned actor
			ChildStaticMesh->SetWorldScale3D(FVector(FMath::RandRange(0.5f, 5.0f), FMath::RandRange(0.5f, 5.0f), FMath::RandRange(0.5f, 5.0f)));

			// Get the current velocity of the static mesh component
			FVector CurrentVelocity = ChildStaticMesh->GetPhysicsLinearVelocity();

			// Check if the static mesh component is on the ground
			if (CurrentVelocity.Z < 0.01f)
			{
				float ForceMagnitude = 1000.0f;
				FVector Force = FVector(ForceMagnitude, 0.0f, 0.0f);
				ChildStaticMesh->AddForce(Force, NAME_None, true);
			}

			// Schedule destruction after the specified delay
			GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ASpawnBox::DestroySpawnedActor, DestroyDelay, false);
		}
		else
		{
			// Handle the case where the ChildStaticMesh is nullptr (e.g., the spawned actor doesn't have a Static Mesh Component)
			bSpawnedActor = false;
		}
	}

	return bSpawnedActor;
}

void ASpawnBox::EnableActorSpawning(bool Enable)
{
	// Update internal flag
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
		// TODO: Log error
	}
}

void ASpawnBox::DestroySpawnedActor()
{
	// Destroy the spawned actor if it exists
	if (SpawnedActor)
	{
		SpawnedActor->Destroy();
		SpawnedActor = nullptr; // Reset the pointer to nullptr
	}
}
