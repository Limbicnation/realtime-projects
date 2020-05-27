// Fill out your copyright notice in the Description page of Project Settings.


#include "Avatar.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
//#include "GameFramework/FloatingPawnMovement.h"


// Sets default values
AAvatar::AAvatar()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CreateDefaultSubobject<UFloatingPawnMovement>("PawnMovement");


	/** Add Character movement */

	PlayerMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatPawnMovement");
	PlayerMovement->MaxSpeed = 2500.f;


	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>("CubeMesh");

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	//CameraComponent->SetupAttachment(GetMesh());
	Camera->bUsePawnControlRotation = true;

	SprintingValue = 16.0f;

	WalkingValue = 2.0f;
}

// Called when the game starts or when spawned
void AAvatar::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &AAvatar::BeginSprinting);
	InputComponent->BindAction("Sprint", IE_Released, this, &AAvatar::EndSprinting);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAxis("MoveForward", this, &AAvatar::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AAvatar::MoveRight);
	InputComponent->BindAxis("LookUp", this, &AAvatar::LookUp);
	InputComponent->BindAxis("Turn", this, &AAvatar::Turn);
}

void AAvatar::MoveForward(float value)
{
	if (value != 0.f)
	{
		if (bIsSprinting)
			value *= SprintingValue;
		/** Add movement in that direction */
		AddMovementInput(GetActorForwardVector(), value / WalkingValue);
	}
}

void AAvatar::MoveRight(float value)
{
	if (Controller && value)
	{
		if (bIsSprinting)
			value *= SprintingValue;
		/** Add movement in that direction */
		AddMovementInput(GetActorRightVector(), value / WalkingValue);
	}
}

void AAvatar::LookUp(float value)
{
	if (value != 0.f)
	{
		AddControllerPitchInput(value);
	}
}

void AAvatar::Turn(float value)
{
	if (value != 0.f)
	{
		AddControllerYawInput(value);
	}
}

void AAvatar::BeginSprinting()
{
	bIsSprinting = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You are Sprintin!"));
}

void AAvatar::EndSprinting()
{
	bIsSprinting = false;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You are no longer sprinting!"));
}
