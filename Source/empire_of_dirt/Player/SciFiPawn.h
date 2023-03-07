// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Pawn.h"
#include "SciFiPawn.generated.h"

USTRUCT()
struct FInteractionData
{
	GENERATED_BODY()

	FInteractionData()
	{
		ViewedInteractionComponent = nullptr;
		LastInteractionCheckTime = 0.f;
		bInteractHeld = false;
	}

	//The current interactable component we're viewing, if there is one
	UPROPERTY()
	class UInteractionComponent* ViewedInteractionComponent;

	//The time when we last checked for an interactable
	UPROPERTY()
	float LastInteractionCheckTime;

	//Whether the local player is holding the interact key
	UPROPERTY()
	bool bInteractHeld;
};


UCLASS()
class EMPIRE_OF_DIRT_API ASciFiPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASciFiPawn();

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* PlayerMesh;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* CameraArm;

	UPROPERTY(EditAnywhere)
	class UFloatingPawnMovement* PlayerMovement;


protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;


	/*Interaction*/


	/* Handle Sprinting */

	void BeginSprint();

	void EndSprint();

	// This function will shoot a projectile when called
	void Shoot();

	UPROPERTY(EditAnywhere, Category = "shooting")
	TSubclassOf<class ABullet> BulletClass;

	// This function will set the bullet scale
	UPROPERTY(EditAnywhere)
	float BulletScale;

	bool bIsSprinting = false;


	UPROPERTY(EditAnywhere)
	float SprintingValue;

	UPROPERTY(EditAnywhere)
	float BulletForce;

	UPROPERTY(EditAnywhere)
	float WalkingValue;

public:

	/*Interaction*/

	//How often in seconds to check for an interactable object. Set this to zero if you want to check every tick.
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float InteractionCheckFrequency;

	//How far we'll trace when we check if the player is looking at an interactable object
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float InteractionCheckDistance;

	void PerformInteractionCheck();

	void CouldntFindInteractable();
	void FoundNewInteractable(UInteractionComponent* Interactable);

	void BeginInteract();
	void EndInteract();

	void Interact();

	//Information about the current state of the players interaction
	UPROPERTY()
	FInteractionData InteractionData;

	//Helper function to make grabbing interactable faster
	FORCEINLINE class UInteractionComponent* GetInteractable() const
	{
		return InteractionData.ViewedInteractionComponent;
	}

	FTimerHandle TimerHandle_Interact;

public:

	//True if we're interacting with an item that has an interaction time (for example a lamp that takes 2 seconds to turn on)
	bool IsInteracting() const;

	//Get the time till we interact with the current interactable
	float GetRemainingInteractTime() const;

public:

	// Called every frame
	void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Jump();

	void MoveForward(float value);
	void MoveRight(float value);

	void MoveUp(float value);
	void MoveDown(float value);

	void Turn(float value);
	void LookUp(float value);
};
