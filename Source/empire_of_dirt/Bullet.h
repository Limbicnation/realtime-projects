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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TArray<FString> MaterialInstancePaths;

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
    float DestroyDelay;

    UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = true))
    FVector BulletSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet", meta = (AllowPrivateAccess = true))
    USceneComponent* RootComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet", meta = (AllowPrivateAccess = true))
    float InitialSpeed = 2500.0f; // Set default value to 2500

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

    float BulletExpiry;
};
