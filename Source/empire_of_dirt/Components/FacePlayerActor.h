#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Sound/SoundCue.h"
#include "FacePlayerActor.generated.h"

UCLASS()
class AFacePlayerActor : public AActor
{
    GENERATED_BODY()
    
public:    
    // Sets default values for this actor's properties
    AFacePlayerActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Checks if the sound should be played based on the actor's rotation
    bool ShouldPlaySound(const FRotator& NewRotation) const;

    // Static Mesh Component
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* FacePlayerMesh;

    // Sound cue for rotation sound effect
    UPROPERTY(EditAnywhere, Category = "Sound")
    USoundCue* RotationSoundCue;

    // Angle difference required to trigger the rotation sound
    UPROPERTY(EditAnywhere, Category = "Sound")
    float RotationSoundTriggerAngle;

private:
    // Last rotation angle at which the sound was played
    float LastPlayedSoundAngle;
};
