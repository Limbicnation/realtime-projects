#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h" // Include this header
#include "Bullet.generated.h"

UCLASS()
class EMPIRE_OF_DIRT_API ABullet : public AActor
{
    GENERATED_BODY()

public:
    ABullet();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TArray<FString> MaterialInstancePaths;

    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    TSubclassOf<class ABullet> BulletClass;

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
    UProjectileMovementComponent* BulletMovement; // No syntax error should occur here now

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet", meta = (AllowPrivateAccess = true))
    float DestroyDelay;

    UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = true))
    FVector BulletSpeed = FVector(10000.0f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet", meta = (AllowPrivateAccess = true))
    USceneComponent* RootComp;

    float BulletExpiry = 0;
};
