// Fill out your copyright notice in the Description page of Project Settings.


#include "SciFiPawn.h"
#include "Bullet.h"

#include "Components/InteractionComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


// Sets default Values
ASciFiPawn::ASciFiPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>("PlayerMesh");
	SetRootComponent(PlayerMesh);

	// Handle Camera clipping
	CameraArm = CreateDefaultSubobject<USpringArmComponent>("CameraSpringArm");
	CameraArm->SetupAttachment(PlayerMesh);
	CameraArm->TargetArmLength = 500.f;

	Camera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	Camera->SetupAttachment(CameraArm);

	SetRootComponent(PlayerMesh);

	/* Interaction default values*/

	InteractionCheckFrequency = 0.f;
	InteractionCheckDistance = 1000.f;


	PlayerMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatPawnMovement");
	PlayerMovement->MaxSpeed = 2500.f;

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;

	SprintingValue = 16.0f;

	WalkingValue = 2.0f;

	BulletOffset = 600.0f;

	BulletSpeed = FVector(0.0f, 0.0f, 5000.0f);

	// Default Bullet mesh scale
	BulletScale = 1.f;
}

// Called when the game starts or when spawned
void ASciFiPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASciFiPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PerformInteractionCheck();
}

void ASciFiPawn::PerformInteractionCheck()
{
	if (GetController() == nullptr)
	{
		return;
	}

	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector EyesLoc;
	FRotator EyesRot;

	GetController()->GetPlayerViewPoint(EyesLoc, EyesRot);

	FVector TraceStart = EyesLoc;
	FVector TraceEnd = (EyesRot.Vector() * InteractionCheckDistance) + TraceStart;
	FHitResult TraceHit;

	FCollisionQueryParams QueryParams;
	/*QueryParams.AddIgnoredActor(this);*/

	if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		//Check if we hit an interactable object
		if (TraceHit.GetActor())
		{
			if (UInteractionComponent* InteractionComponent = Cast<UInteractionComponent>(
				TraceHit.GetActor()->GetComponentByClass(UInteractionComponent::StaticClass())))
			{
				float Distance = (TraceStart - TraceHit.ImpactPoint).Size();
				if (InteractionComponent != GetInteractable() && Distance <= InteractionComponent->InteractionDistance)
				{
					FoundNewInteractable(InteractionComponent);
				}
				else if (Distance > InteractionComponent->InteractionDistance && GetInteractable())
				{
					CouldntFindInteractable();
				}
			}
		}
	}
}

void ASciFiPawn::CouldntFindInteractable()
{
}

void ASciFiPawn::FoundNewInteractable(UInteractionComponent* Interactable)
{
	UE_LOG(LogTemp, Warning, TEXT("you found an interactable"));
}


// Called to bind functionality to input
void ASciFiPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASciFiPawn::Shoot);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &ASciFiPawn::BeginSprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &ASciFiPawn::EndSprint);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASciFiPawn::Jump);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASciFiPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASciFiPawn::MoveRight);

	PlayerInputComponent->BindAxis("MoveUp", this, &ASciFiPawn::MoveUp);
	PlayerInputComponent->BindAxis("MoveDown", this, &ASciFiPawn::MoveDown);

	PlayerInputComponent->BindAxis("Turn", this, &ASciFiPawn::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ASciFiPawn::LookUp);
}

void ASciFiPawn::Jump()
{
	UE_LOG(LogTemp, Warning, TEXT("Jumped!"));
}

void ASciFiPawn::MoveForward(float Value)
{
	if (Value != 0.f)
	{
		if (bIsSprinting)
			Value *= SprintingValue;

		// add movement in the forward direction
		AddMovementInput(GetActorForwardVector(), Value / WalkingValue);
	}
}

void ASciFiPawn::MoveUp(float Value)
{
	if (Value != 0.f)

		if (bIsSprinting)
			Value *= SprintingValue;

	// add movement in upward direction
	{
		AddMovementInput(GetActorUpVector(), Value / WalkingValue);
	}
}

void ASciFiPawn::MoveDown(float Value)
{
	if (Value != 0.f)
	{
		if (bIsSprinting)
			Value *= SprintingValue;

		// add movement in downward direction
		AddMovementInput(GetActorUpVector(), Value / WalkingValue);
	}
}

void ASciFiPawn::MoveRight(float Value)
{
	if (Value != 0.f)
	{
		if (bIsSprinting)
			Value *= SprintingValue;

		// add movement in that right direction
		AddMovementInput(GetActorRightVector(), Value / WalkingValue);
	}
}

void ASciFiPawn::Turn(float Value)
{
	if (Value != 0.f)
	{
		AddControllerYawInput(Value);
	}
}

void ASciFiPawn::LookUp(float Value)
{
	if (Value != 0.f)
	{
		AddControllerPitchInput(Value);
	}
}

void ASciFiPawn::BeginSprint()
{
	bIsSprinting = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You are moving fast!"));
}


void ASciFiPawn::EndSprint()
{
	bIsSprinting = false;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You are no longer moving fast!"));
}

void ASciFiPawn::Shoot()
{
	// Log Temp warning for firing a weapon
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire Weapon"));
	}
	if (BulletClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		// No way that the spawner can fail
		SpawnParams.bNoFail = true;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;

		// Bullet Transforms
		FTransform BulletSpawnTransform;
		BulletSpawnTransform.SetLocation(GetActorForwardVector() * BulletOffset + GetActorLocation());
		BulletSpawnTransform.SetRotation(GetActorRotation().Quaternion());
		BulletSpawnTransform.SetScale3D(FVector(BulletScale));

		//GetWorld()->SpawnActor<ABullet>(BulletClass, BulletSpawnTransform, SpawnParams);
		ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletClass, BulletSpawnTransform, SpawnParams);
		//Set the velocity of the Bullet
		FVector BulletSpeed = GetActorForwardVector() * BulletSpeed;
		
		if (Bullet)
		{
			Bullet->BulletSpeed = BulletSpeed; 
		}
	}
}
