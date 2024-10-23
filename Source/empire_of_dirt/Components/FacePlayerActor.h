#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Sound/SoundCue.h"
#include "FacePlayerActor.generated.h"
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
USplineMeshComponent* FacePlayerMesh;
UCLASS()
class AFacePlayerActor : public AActor
{
    GENERATED_BODY()
    #include "CoreMinimal.h"
    #include "GameFramework/Actor.h"
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

    private:
        UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
        USplineMeshComponent* FacePlayerMesh;

        UPROPERTY(EditAnywhere, Category = "Sound")
        USoundBase* RotationSoundCue;

        UPROPERTY(EditAnywhere, Category = "Sound")
        float RotationSoundTriggerAngle;

        UPROPERTY(EditAnywhere, Category = "Rotation")
        float RotationSpeed;

        UPROPERTY(EditAnywhere, Category = "Sound")
        float MinTimeBetweenSounds;

        float LastPlayedSoundAngle;
        float LastSoundPlayTime;
        FRotator TargetRotation;

        bool ShouldPlaySound(const FRotator& NewRotation) const;
    };
    
public:    
    AFacePlayerActor();

protected:
    virtual void BeginPlay() override;

public:    
    virtual void Tick(float DeltaTime) override;

    // Checks if the sound should be played based on the actor's rotation
    bool ShouldPlaySound(const FRotator& NewRotation) const;

    // Spline Mesh Component - Made EditAnywhere to allow mesh assignment in editor
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    USplineMeshComponent* FacePlayerMesh;

    // Sound cue for rotation sound effect
    UPROPERTY(EditAnywhere, Category = "Sound")
    USoundCue* RotationSoundCue;

    // Angle difference required to trigger the rotation sound
    UPROPERTY(EditAnywhere, Category = "Sound", meta = (ClampMin = "0.0", ClampMax = "180.0"))
    float RotationSoundTriggerAngle;

    // How fast the actor rotates to face the player (degrees per second)
    UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0.0", ClampMax = "360.0"))
    float RotationSpeed;

    // Minimum time between sound plays (in seconds)
    UPROPERTY(EditAnywhere, Category = "Sound")
    float MinTimeBetweenSounds;

    // Scale of the mesh
    UPROPERTY(EditAnywhere, Category = "Transform")
    FVector MeshScale;

private:
    float LastPlayedSoundAngle;
    float LastSoundPlayTime;
    FRotator TargetRotation;
};
