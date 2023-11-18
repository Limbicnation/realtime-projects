// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SpawnBox.generated.h"

UCLASS()
class EMPIRE_OF_DIRT_API ASpawnBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the actor stops playing
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Will spawn an actor of the specified class
	UFUNCTION(BlueprintCallable)
	bool SpawnActor();

	// Change if actors are spawned
	UFUNCTION(BlueprintCallable)
	void EnableActorSpawning(bool Enable);

private:
	UFUNCTION()
	void SpawnActorScheduled();

	// Will schedule an actor spawn
	void ScheduleActorSpawn();

	// Destroy the spawned actor after a certain delay
	void DestroySpawnedActor();

public:
	// Actor class to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ActorClassToBeSpawned;

	// Average time between spawns (without random)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AvgSpawnTime = 5.f;

	// Random +/- offset of the spawn time
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RandomSpawnTimeOffset = 1.f;

	// Delay before destroying the spawned actor (in seconds)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DestroyDelay = 10.0f; // Added variable for destroy delay

private:
	// Box in which we spawn the random Actors
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* SpawnBox;

	UPROPERTY(EditAnywhere)
	bool ShouldSpawn = true;

	// Helper for timing
	FTimerHandle SpawnTimerHandle;

	// Pointer to the spawned actor
	AActor* SpawnedActor = nullptr;

	// Timer handle for destroying the spawned actor
	FTimerHandle DestroyTimerHandle; // Added timer handle for destruction
};
