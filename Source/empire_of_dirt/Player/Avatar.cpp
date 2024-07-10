#include "Avatar.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/SkeletalMeshSocket.h"
#include "DrawDebugHelpers.h"
#include "Bullet.h"

AAvatar::AAvatar()
{
    PrimaryActorTick.bCanEverTick = true;

    CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>("CubeMesh");
    SetRootComponent(CubeMesh);

    PlayerMovement = CreateDefaultSubobject<UFloatingPawnMovement>("PlayerMovement");
    PlayerMovement->MaxSpeed = 3500;

    ForwardArrowComponent = CreateDefaultSubobject<UArrowComponent>("ForwardArrowComponent");
    ForwardArrowComponent->SetupAttachment(GetRootComponent());

    // Initialize CameraBoom and FollowCamera
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
    CameraBoom->SetupAttachment(GetMesh()); // Attach to the skeletal mesh
    CameraBoom->TargetArmLength = 300.f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 250.f, 0.f);
    GetCharacterMovement()->JumpZVelocity = 600.f;
    GetCharacterMovement()->AirControl = 0.2f;

    BaseTurnRate = 45.f;
    BaseLookUpRate = 45.f;
    bIsSprinting = false;
    SprintingValue = 32.0f;
    WalkingValue = 2.0f;
}

void AAvatar::BeginPlay()
{
    Super::BeginPlay();
}

void AAvatar::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Align camera boom rotation with the arrow component
    if (ForwardArrowComponent)
    {
        CameraBoom->SetWorldRotation(ForwardArrowComponent->GetComponentRotation());
    }
}

void AAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AAvatar::Shoot);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AAvatar::BeginSprinting);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AAvatar::EndSprinting);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAxis("MoveForward", this, &AAvatar::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AAvatar::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &AAvatar::LookUp);
    PlayerInputComponent->BindAxis("Turn", this, &AAvatar::Turn);
}

void AAvatar::MoveForward(float Value)
{
    if (Controller != nullptr && Value != 0.f)
    {
        if (bIsSprinting)
        {
            Value *= SprintingValue;
        }
        AddMovementInput(GetActorForwardVector(), Value / WalkingValue);
    }
}

void AAvatar::MoveRight(float Value)
{
    if (Controller && Value != 0.f)
    {
        if (bIsSprinting)
        {
            Value *= WalkingValue;
        }
        AddMovementInput(GetActorRightVector(), Value / WalkingValue);
    }
}

void AAvatar::LookUp(float Value)
{
    AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AAvatar::Turn(float Value)
{
    AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
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

void AAvatar::Shoot()
{
    UE_LOG(LogTemp, Warning, TEXT("shoot"));

    if (ShootSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ShootSound, GetActorLocation());
    }

    const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName("BarrelSocket");
    if (!BarrelSocket)
    {
        UE_LOG(LogTemp, Warning, TEXT("BarrelSocket not found."));
        return;
    }

    const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetMesh());

    if (MuzzleFlash)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleFlash, SocketTransform.GetLocation(), SocketTransform.GetRotation().Rotator());
    }

    FVector SpawnLocation = SocketTransform.GetLocation();
    FRotator SpawnRotation = SocketTransform.GetRotation().Rotator();

    if (BulletClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();

        ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletClass, SpawnLocation, SpawnRotation, SpawnParams);
        if (Bullet)
        {
            FVector BulletVelocity = SpawnRotation.Vector() * 3000.0f; // Set the bullet velocity based on the rotation vector
            Bullet->SetVelocity(BulletVelocity);
        }
    }

    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && FireMontage)
    {
        AnimInstance->Montage_Play(FireMontage);
        if (!AnimInstance->Montage_IsPlaying(FireMontage))
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to play fire montage"));
        }
    }
}




