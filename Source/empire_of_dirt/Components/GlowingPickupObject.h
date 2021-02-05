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

	UPROPERTY(EditAnywhere)
	USceneComponent* PickupRoot;

	/*static mesh for pickup*/
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditAnywhere)
	UShapeComponent* PickupBox;

	UFUNCTION()
	void OnPlayerEnteredPickupBox(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                              class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                              const FHitResult& SweepResult);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:


	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Pickup;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BT_Pickup;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	/*Toggle item glow*/
	void ToggleGlow(bool IsGlowing);

	UFUNCTION()
	void EnteredObjectRadius(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                         class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                         const FHitResult& SweepResult);

	UFUNCTION()
	void LeftObjectRadius(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                      class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
