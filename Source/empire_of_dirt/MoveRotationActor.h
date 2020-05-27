// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoveRotationActor.generated.h"

UCLASS()
class EMPIRE_OF_DIRT_API AMoveRotationActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMoveRotationActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Movement")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MeshMovement;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float PitchValue;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float YawValue;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RollValue;
	
};
