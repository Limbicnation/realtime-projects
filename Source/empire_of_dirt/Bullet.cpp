// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Classes/GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");

	SetRootComponent(BulletMesh);

	// in initialize the Scene Component for Spawning the bullet class
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	
	RootComponent = RootComp;
	
	BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>("BulletMovement");
	BulletMovement->InitialSpeed = BulletSpeed.Z; // Use the Public speed variable instead of hardcoding
	BulletMovement->MaxSpeed = BulletSpeed.Z; // Use the public speed variable instead of hardcoding
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult HitResult;

	FVector StartTrace = this->GetActorLocation();
	FVector EndTrace = (BulletSpeed * DeltaTime) + StartTrace;
	EndTrace.Z += this->GetActorRotation().Pitch;

	FCollisionQueryParams CollisionParams;
	// If the line trace hits the player itself, it will just ignore it
	CollisionParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, CollisionParams))
	{
		if (HitResult.GetActor())
		{
			DrawDebugSolidBox(GetWorld(), HitResult.ImpactPoint, FVector(500.f), FColor::Blue, true);
			AActor* Mesh = Cast<AActor>(HitResult.GetActor());

			// Get the static mesh component
			UStaticMeshComponent* StaticMeshComponent = Mesh->FindComponentByClass<UStaticMeshComponent>();

			// Get the static mesh material
			UMaterialInterface* MaterialInterface = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/_Game/MaterialInstance/MI_QuadTruchetWeave.MI_QuadTruchetWeave'")); 
 
			if (StaticMeshComponent && MaterialInterface)
			{
				// Set the material on the first material slot of the mesh
				StaticMeshComponent->SetMaterial(0, MaterialInterface);
			}

			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Could not get mesh. Type is %s"), *HitResult.GetActor()->StaticClass()->GetFName().ToString()));
			}
			
		}
		Destroy();
	}
	else
	{
		BulletExpiry += DeltaTime;
		
		// Draw Debug Line and specify the true argument to make the debug line persistent
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(FColor(0.0f, -BulletExpiry * 80.0f, 100.0f)),true);

		SetActorLocation(EndTrace);
		
		BulletSpeed += FVector(0.0f, 0.0f, -200.0f) * DeltaTime;
	}
	if(BulletExpiry > 3)
	{
		Destroy();
	}
	
}
