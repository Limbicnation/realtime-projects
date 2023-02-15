// Copyright notice goes here

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickableActor_Base.generated.h"

UCLASS()
class EMPIRE_OF_DIRT_API APickableActor_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	APickableActor_Base();

protected:
	virtual void BeginPlay() override;

	// Detecting overlap collision
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickable Item")
	class USphereComponent* CollisionComp;

	// visually representing the packable item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickable Item")
	class UStaticMeshComponent* MeshComp;

	// giving the packable item a rotational value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickable Item")
	class URotatingMovementComponent* RotationComp;

	// Set up the collision callback function to be called when a collision is detected
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:

	// Random scale for static Mesh Component
	FVector RandomScale;
};
