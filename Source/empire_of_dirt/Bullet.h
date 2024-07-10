#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Bullet.generated.h"

UCLASS()
class EMPIRE_OF_DIRT_API ABullet : public AActor
{
    GENERATED_BODY()

public:
    ABullet();

    // Expose key properties to Blueprint
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TArray<FString> MaterialInstancePaths;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
    float InitialSpeed = 2500.0f; // Default value of 2500

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
    float MaxSpeed = 5000.0f; // Maximum speed of the bullet

    UFUNCTION(BlueprintCallable, Category = "Bullet")
    void SetVelocity(const FVector& NewVelocity);

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = true))
    UStaticMeshComponent* BulletMesh;

    UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = true))
    UProjectileMovementComponent* BulletMovement;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet", meta = (AllowPrivateAccess = true))
    float DestroyDelay = 3.0f; // Time after which the bullet is destroyed

    UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = true))
    FVector BulletSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet", meta = (AllowPrivateAccess = true))
    USceneComponent* RootComp;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

    float BulletExpiry;
};
