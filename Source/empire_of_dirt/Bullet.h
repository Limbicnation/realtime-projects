#pragma once

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "Bullet.generated.h"

/**
 * A projectile fired from a weapon.
 */
UCLASS()
class EMPIRE_OF_DIRT_API ABullet : public AActor
{
	GENERATED_BODY()

public:

	// we declare a BlueprintReadWrite variable in the parent Blueprint class:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Materials")
	TArray<FString> MaterialInstancePaths;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class ABullet> BulletClass;

	// Set the velocity of the bullet
	UFUNCTION(BlueprintCallable, Category = "Bullet")
	void SetVelocity(const FVector& NewVelocity);

	/** Sets default values for this actor's properties. */
	ABullet();

	/** The bullet's static mesh component. */
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* BulletMesh;

	/** The bullet's movement component. */
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = true))
	UProjectileMovementComponent* BulletMovement;

	/** The speed of the bullet. */
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = true))
	FVector BulletSpeed = FVector(10000.0f);

protected:
	/** Called when the game starts or when spawned. */
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet")
	float DestroyDelay; // Add this line to declare DestroyDelay

public:
	/** Called every frame. */
	virtual void Tick(float DeltaTime) override;

private:
	/** The root component of the bullet. */
	USceneComponent* RootComp;

	/** The amount of time that has passed since the bullet was fired. */
	float BulletExpiry = 0;
};
