#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ArrowComponent.h"
#include "Avatar.generated.h"

UCLASS()
class EMPIRE_OF_DIRT_API AAvatar : public ACharacter
{
    GENERATED_BODY()

public:
    AAvatar();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    void MoveForward(float Value);
    void MoveRight(float Value);
    void LookUp(float Value);
    void Turn(float Value);
    void BeginSprinting();
    void EndSprinting();
    void Shoot();
    void UpdateMovementVectors();

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* CubeMesh;

    UPROPERTY(EditAnywhere)
    UFloatingPawnMovement* PlayerMovement;

    UPROPERTY(EditAnywhere)
    USpringArmComponent* CameraBoom;

    UPROPERTY(EditAnywhere)
    UCameraComponent* FollowCamera;

    UPROPERTY(EditAnywhere)
    UArrowComponent* ForwardArrowComponent;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class ABullet> BulletClass;

    UPROPERTY(EditAnywhere, Category = "SFX")
    USoundBase* ShootSound;

    UPROPERTY(EditAnywhere, Category = "FX")
    class UNiagaraSystem* MuzzleFlash;

    UPROPERTY(EditAnywhere, Category = "Animations")
    class UAnimMontage* FireMontage;

    // Cached movement vectors
    FVector CachedForwardVector;
    FVector CachedRightVector;

    bool bIsSprinting;
    float SprintingValue;
    float WalkingValue;
    float BaseTurnRate;
    float BaseLookUpRate;
};
