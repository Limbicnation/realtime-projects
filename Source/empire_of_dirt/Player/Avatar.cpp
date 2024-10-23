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

    // Create components
    CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>("CubeMesh");
    ForwardArrowComponent = CreateDefaultSubobject<UArrowComponent>("ForwardArrowComponent");
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
    FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
    PlayerMovement = CreateDefaultSubobject<UFloatingPawnMovement>("PlayerMovement");

    // Set up component hierarchy
    SetRootComponent(CubeMesh);
    ForwardArrowComponent->SetupAttachment(GetRootComponent());
    CameraBoom->SetupAttachment(GetMesh());
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

    // Configure movement
    PlayerMovement->MaxSpeed = 3500.0f;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 250.f, 0.f);
    GetCharacterMovement()->JumpZVelocity = 600.f;
    GetCharacterMovement()->AirControl = 0.2f;

    // Configure camera
    CameraBoom->TargetArmLength = 300.f;
    CameraBoom->bUsePawnControlRotation = true;
    FollowCamera->bUsePawnControlRotation = false;

    // Configure rotation
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Initialize movement values
    BaseTurnRate = 45.f;
    BaseLookUpRate = 45.f;
    bIsSprinting = false;
    SprintingValue = 32.0f;
    WalkingValue = 2.0f;
}

void AAvatar::BeginPlay()
{
    Super::BeginPlay();
    UpdateMovementVectors();
}

void AAvatar::UpdateMovementVectors()
{
    if (Controller)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FRotationMatrix RotMatrix(YawRotation);
        CachedForwardVector = RotMatrix.GetUnitAxis(EAxis::X);
        CachedRightVector = RotMatrix.GetUnitAxis(EAxis::Y);
    }
}

void AAvatar::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (ForwardArrowComponent && CameraBoom)
    {
        UpdateMovementVectors();
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
    if (Controller && Value != 0.f)
    {
        const float ScaledValue = Value / WalkingValue * (bIsSprinting ? SprintingValue : 1.f);
        AddMovementInput(CachedForwardVector, ScaledValue);
    }
}

void AAvatar::MoveRight(float Value)
{
    if (Controller && Value != 0.f)
    {
        const float ScaledValue = Value / WalkingValue * (bIsSprinting ? WalkingValue : 1.f);
        AddMovementInput(CachedRightVector, ScaledValue);
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
    if (!bIsSprinting)
    {
        bIsSprinting = true;
        #if WITH_EDITOR
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You are Sprinting!"));
        #endif
    }
}

void AAvatar::EndSprinting()
{
    if (bIsSprinting)
    {
        bIsSprinting = false;
        #if WITH_EDITOR
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You are no longer sprinting!"));
        #endif
    }
}

void AAvatar::Shoot()
{
    const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName("BarrelSocket");
    if (!BarrelSocket)
    {
        UE_LOG(LogTemp, Warning, TEXT("BarrelSocket not found."));
        return;
    }

    const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetMesh());

    // Play sound and spawn effects
    if (ShootSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ShootSound, GetActorLocation());
    }

    if (MuzzleFlash)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(), 
            MuzzleFlash, 
            SocketTransform.GetLocation(), 
            SocketTransform.GetRotation().Rotator()
        );
    }

    // Spawn bullet
    if (BulletClass && GetWorld())
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();

        if (ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(
            BulletClass, 
            SocketTransform.GetLocation(), 
            SocketTransform.GetRotation().Rotator(), 
            SpawnParams))
        {
            Bullet->SetVelocity(SocketTransform.GetRotation().Vector() * 3000.0f);
        }
    }

    // Play animation
    if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
    {
        if (FireMontage)
        {
            AnimInstance->Montage_Play(FireMontage);
        }
    }
}
