#include "FacePlayerActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"

AFacePlayerActor::AFacePlayerActor()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create and set up the static mesh component
    FacePlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FacePlayerMesh"));
    RootComponent = FacePlayerMesh;
    
    // Configure the static mesh component
    if (FacePlayerMesh)
    {
        FacePlayerMesh->SetVisibility(true);
        FacePlayerMesh->SetHiddenInGame(false);
        FacePlayerMesh->SetCastShadow(true);
        FacePlayerMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        FacePlayerMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
        FacePlayerMesh->SetGenerateOverlapEvents(true);

        static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Cube"));
        if (MeshAsset.Succeeded())
        {
            FacePlayerMesh->SetStaticMesh(MeshAsset.Object);
        }
    }

    // Initialize variables
    RotationSoundCue = nullptr;
    RotationSoundTriggerAngle = 45.0f;
    RotationSpeed = 180.0f;
    MinTimeBetweenSounds = 0.5f;
    LastPlayedSoundAngle = 0.0f;
    LastSoundPlayTime = 0.0f;
}

void AFacePlayerActor::BeginPlay()
{
    Super::BeginPlay();
    
    if (FacePlayerMesh)
    {
        FacePlayerMesh->SetVisibility(true);
        FacePlayerMesh->SetHiddenInGame(false);
    }
    
    TargetRotation = GetActorRotation();
}

void AFacePlayerActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerPawn) return;

    // Get player's forward vector and our direction to player
    FVector PlayerForward = PlayerPawn->GetActorForwardVector();
    FVector DirectionToActor = GetActorLocation() - PlayerPawn->GetActorLocation();
    DirectionToActor.Normalize();

    // Calculate the angle between player's forward vector and direction to this actor
    float DotProduct = FVector::DotProduct(PlayerForward, DirectionToActor);
    float AngleInRadians = FMath::Acos(DotProduct);
    float AngleInDegrees = FMath::RadiansToDegrees(AngleInRadians);

    // Calculate rotation to face player's forward direction
    FRotator PlayerRotation = PlayerPawn->GetActorRotation();
    FRotator DesiredRotation = PlayerRotation;
    DesiredRotation.Pitch = 0.0f; // Keep rotation in XY plane
    DesiredRotation.Roll = 0.0f;

    // Smoothly interpolate to the desired rotation
    FRotator NewRotation = FMath::RInterpTo(
        GetActorRotation(),
        DesiredRotation,
        DeltaTime,
        RotationSpeed
    );

    // Apply rotation
    SetActorRotation(NewRotation);

    // Handle sound based on angle difference
    float CurrentTime = GetWorld()->GetTimeSeconds();
    float TimeSinceLastSound = CurrentTime - LastSoundPlayTime;

    if (AngleInDegrees >= RotationSoundTriggerAngle && TimeSinceLastSound >= MinTimeBetweenSounds)
    {
        if (RotationSoundCue)
        {
            UGameplayStatics::PlaySoundAtLocation(
                this,
                RotationSoundCue,
                GetActorLocation(),
                1.0f,
                1.0f,
                0.0f
            );
            
            LastPlayedSoundAngle = NewRotation.Yaw;
            LastSoundPlayTime = CurrentTime;
        }
    }
}

bool AFacePlayerActor::ShouldPlaySound(const FRotator& NewRotation) const
{
    float DeltaAngle = FMath::Abs(FMath::FindDeltaAngleDegrees(NewRotation.Yaw, LastPlayedSoundAngle));
    return DeltaAngle >= RotationSoundTriggerAngle;
}
