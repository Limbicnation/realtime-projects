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

protected:
	// Called when the game starts or when spawned
	// set to 'void' when error keeps happening
	void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Componets")
	UStaticMeshComponent* BulletMesh;

	UPROPERTY(EditAnywhere, Category = "Componets")
	class UProjectileMovementComponent* BulletMovement;


public:
	// Called every frame
	void Tick(float DeltaTime) override;
};
