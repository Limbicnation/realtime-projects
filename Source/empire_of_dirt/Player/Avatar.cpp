// Fill out your copyright notice in the Description page of Project Settings.


#include "Avatar.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Bullet.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AAvatar::AAvatar() :
	BaseTurnRate(45.f),
	BaseLookUpRate(45.f)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CreateDefaultSubobject<UFloatingPawnMovement>("PawnMovement");


	/** Add Character movement */

	PlayerMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatPawnMovement");
	PlayerMovement->MaxSpeed = 3500;


	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>("CubeMesh");

	SprintingValue = 32.0f;

	WalkingValue = 2.0f;

	/** Setup camera boom (pulls in toward the character if there is a collision) */

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;

	/** Setup follow camera*/

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera-> SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm

	// Don't rotate, when the controller rotates. Camera rotates independent of Character rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f); // at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f; // These are values to tweak and to get them right for your game
	GetCharacterMovement()->AirControl = 0.2f; // These are values to tweak and to get them right for your game


	//Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	//CameraComponent->SetupAttachment(GetMesh());
	//Camera->bUsePawnControlRotation = true;

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

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AAvatar::shoot);

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
	if ((Controller != nullptr) && (value != 0.f))
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
			value *= WalkingValue;
		/** Add movement in that direction */
		AddMovementInput(GetActorRightVector(), value / WalkingValue);
	}
}

void AAvatar::LookUp(float value)
{
	if (value != 0.f)
	{
		AddControllerPitchInput(value * BaseLookUpRate * GetWorld()->GetDeltaSeconds()); // deg/sec * sec/frame
	}
}

void AAvatar::Turn(float value)
{
	if (value != 0.f)
	{
		AddControllerYawInput(value * BaseTurnRate * GetWorld()->GetDeltaSeconds()); // deg/sec * sec/frame);
	}
}

void AAvatar::TurnRate(float Rate)
{
	if (Rate != 0.f)
	{   // calculate delta for this frame from the rate information
		AddControllerYawInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds()); // deg/sec * sec/frame);
	}
}

void AAvatar::LookUpRate(float Rate)
{
	// calculate delta for this frame from the rate value
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds()); // deg/sec * sec/frame
}

void AAvatar::BeginSprinting()
{
	bIsSprinting = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You are Sprinting!"));
}

void AAvatar::EndSprinting()
{
	bIsSprinting = false;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You are no longer sprinting!"));
}

void AAvatar::shoot()
{
	if (BulletClass) 
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.bNoFail = true;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;

		FTransform BulletSpawnTransform;
		BulletSpawnTransform.SetLocation(GetActorForwardVector() * 500.f + GetActorLocation() + 100.f);
		BulletSpawnTransform.SetRotation(GetActorRotation().Quaternion());
		BulletSpawnTransform.SetScale3D(FVector(1.f));

		GetWorld()->SpawnActor<ABullet>(BulletClass, BulletSpawnTransform, SpawnParams);
	}

}
