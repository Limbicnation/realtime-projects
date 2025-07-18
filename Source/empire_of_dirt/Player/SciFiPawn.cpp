// Fill out your copyright notice in the Description page of Project Settings.


#include "SciFiPawn.h"
#include "Bullet.h"

#include "Components/InteractionComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/Character.h"


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

	// In SciFiPawn.cpp constructor
	BulletSpeed = FVector(5000.0f, 0.0f, 0.0f);  // Forward direction with 5000 speed

	// Default Bullet mesh scale
	BulletScale = 100.f;
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

	if (BulletClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BulletClass is nullptr"));
		return;
	}

	if (GetWorld() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetWorld() returned nullptr"));
		return;
	}

	// Define the Trace Distance
	float TraceDistance = 10000.f;

	// Define muzzle location (in front of camera)
	FVector CameraLocation = Camera->GetComponentLocation();
	FRotator CameraRotation = Camera->GetComponentRotation();

	// Create muzzle offset from camera (this simulates a weapon muzzle position)
	FVector MuzzleOffset = FVector(100.0f, 0.0f, 0.0f);
	FVector MuzzleLocation = CameraLocation + CameraRotation.RotateVector(MuzzleOffset);

	// Add small additional offset to prevent collision with the character
	MuzzleLocation += CameraRotation.Vector() * 50.0f;

	// Set spawn parameters with better collision handling
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	// Perform the line trace (for hit detection)
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + Camera->GetForwardVector() * TraceDistance;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams))
	{
		if (HitResult.GetActor())
		{
			// Spawn the bullet at the muzzle location with camera rotation
			// (not at hit location, to maintain consistent behavior)
			ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletClass, MuzzleLocation, CameraRotation, SpawnParams);

			if (Bullet != nullptr)
			{
				// Set velocity for the bullet to travel toward the hit point
				FVector DirectionToHit = (HitResult.ImpactPoint - MuzzleLocation).GetSafeNormal();

				// Use BulletSpeed's magnitude (size) instead of the actual vector
				float BulletSpeedMagnitude = BulletSpeed.Size();
				Bullet->SetVelocity(DirectionToHit * BulletSpeedMagnitude);

				// Play fire sound if available
				if (FireSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, FireSound, MuzzleLocation);
				}
			}

			// Change the material of the hit object (keep this functionality)
			AActor* Mesh = Cast<AActor>(HitResult.GetActor());
			UStaticMeshComponent* StaticMeshComponent = Mesh->FindComponentByClass<UStaticMeshComponent>();
			UMaterialInterface* MaterialInterface = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/_Game/MaterialInstance/MI_QuadTruchetWeave.MI_QuadTruchetWeave'"));
			if (StaticMeshComponent && MaterialInterface)
			{
				StaticMeshComponent->SetMaterial(0, MaterialInterface);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Could not get mesh. Type is %s"), *HitResult.GetActor()->StaticClass()->GetFName().ToString()));
			}
		}
	}
	else
	{
		// No hit, spawn the bullet from the muzzle with camera direction
		ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletClass, MuzzleLocation, CameraRotation, SpawnParams);
		if (Bullet != nullptr)
		{
			// Use BulletSpeed's magnitude (size) instead of the actual vector
			float BulletSpeedMagnitude = BulletSpeed.Size();
			// Use the camera's forward vector for direction, multiplied by the speed magnitude
			Bullet->SetVelocity(CameraRotation.Vector() * BulletSpeedMagnitude);

			// Play fire sound if available
			if (FireSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, MuzzleLocation);
			}
		}
	}

	// Spawn muzzle flash if available
	if (MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, MuzzleLocation, CameraRotation);
	}
}