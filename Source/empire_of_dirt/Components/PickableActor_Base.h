// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickableActor_Base.generated.h"

UCLASS()
class EMPIRE_OF_DIRT_API APickableActor_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickableActor_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Detecting overlap collision
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickableItem)
	class USphereComponent* CollisionComp;

	// visually representing the packable item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickableItem)
	class UStaticMeshComponent* MeshComp;
	// giving the packable item a rotational value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickableItem)
	class URotatingMovementComponent* RotationComp;


public:


};

