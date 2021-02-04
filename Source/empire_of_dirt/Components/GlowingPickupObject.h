// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GlowingPickupObject.generated.h"

UCLASS()
class EMPIRE_OF_DIRT_API AGlowingPickupObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGlowingPickupObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SM_Pickup;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BT_Pickup;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
