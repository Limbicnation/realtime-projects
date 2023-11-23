#include "FacePlayerActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFacePlayerActor::AFacePlayerActor()
{
    // Set this actor to call Tick() every frame.
    PrimaryActorTick.bCanEverTick = true;

    FacePlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FACE PLAYER MESH"));

    // Initialize variables
    RotationSoundCue = nullptr; // Assign this in the Unreal Editor or dynamically load it
    RotationSoundTriggerAngle = 45.0f; // Example value, adjust as needed
    LastPlayedSoundAngle = 0.0f;
}

// Called when the game starts or when spawned
void AFacePlayerActor::BeginPlay()
{
    Super::BeginPlay();
    // Initialize LastPlayedSoundAngle if needed
}

// Called every frame
void AFacePlayerActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (USceneComponent* RootComp = GetRootComponent())
    {
        if (APawn* LocalPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
        {
            FRotator NewRotation = (RootComp->GetComponentLocation() - LocalPawn->GetActorLocation()).GetSafeNormal().Rotation();
            RootComp->SetWorldRotation(NewRotation);

            // Check if we should play the sound
            if (ShouldPlaySound(NewRotation))
            {
                UGameplayStatics::PlaySoundAtLocation(this, RotationSoundCue, GetActorLocation());
                LastPlayedSoundAngle = NewRotation.Yaw; // Update the last played sound angle
            }
        }
    }
}

bool AFacePlayerActor::ShouldPlaySound(const FRotator& NewRotation) const
{
    // Check the difference in the Yaw angle
    float DeltaAngle = FMath::Abs(NewRotation.Yaw - LastPlayedSoundAngle);
    return DeltaAngle >= RotationSoundTriggerAngle;
}
