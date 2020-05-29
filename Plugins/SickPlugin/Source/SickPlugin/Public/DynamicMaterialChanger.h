// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine.h"
#include "DynamicMaterialChanger.generated.h"


UCLASS()
class SICKPLUGIN_API ADynamicMaterialChanger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADynamicMaterialChanger();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

public:
	// Called every frame
	void Tick(float DeltaTime) override;

	// Dummy root component for the mesh
	UPROPERTY(EditAnywhere)
	USceneComponent* CubeRoot;

	// The mesh for the cube
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CubeMesh;

	// The trigger for the cube
	UPROPERTY(EditAnywhere)
	UBoxComponent* CubeTrigger;

	// Function called when the player enters the trigger area
	UFUNCTION()
	void OnPlayerTriggerTransition(UPrimitiveComponent* OverlappedComp, AActor* otherActor,
	                               UPrimitiveComponent* OtherComp,
	                               int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Function called when player exists trigger area
	UFUNCTION()
	void OnPlayerExitTransition(UPrimitiveComponent* OverlappedComp, AActor* otherActor, UPrimitiveComponent* OtherComp,
	                            int32 OtherBodyIndex);

private:

	bool bRustEffectTrigger;

	float TransitionAmount;
};
