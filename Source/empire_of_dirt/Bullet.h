// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Bullet.generated.h"

UCLASS()
class EMPIRE_OF_DIRT_API ABullet : public AActor
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	ABullet();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	FVector BulletSpeed = FVector(10000.0f); // This is the public speed variable we added for the bullet class

	USceneComponent* RootComp;

	float BulletExpiry = 0;

protected:
	// Called when the game starts or when spawned
	// set to 'void' when error keeps happening
	void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Componets", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* BulletMesh;

	UPROPERTY(EditAnywhere, Category = "Componets", meta = (AllowPrivateAccess = true))
	class UProjectileMovementComponent* BulletMovement;


public:
	// Called every frame
	void Tick(float DeltaTime) override;
};
