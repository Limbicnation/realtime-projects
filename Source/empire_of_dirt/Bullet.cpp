// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/FileHelper.h"
#include "GameFramework/ProjectileMovementComponent.h"


void ABullet::SetVelocity(const FVector& NewVelocity)
{
	BulletMesh->SetPhysicsLinearVelocity(NewVelocity);
}

// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");
	SetRootComponent(BulletMesh);

	// initialize the Scene Component for Spawning the bullet class
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	
	RootComponent = RootComp;
	
	BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>("BulletMovement");
	BulletMovement->InitialSpeed = 5000.0f;
	BulletMovement->MaxSpeed = 500.0f;
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

	// Set the tag you want to check for
	FName ComponentTag = FName("MyObelisk");

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, CollisionParams))
	{
		// Check if a line trace hits any actor with the specified component tag
		UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(HitResult.GetComponent());
		if (StaticMeshComponent)
		{
			{
				UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(HitResult.GetComponent());
				if (StaticMeshComponent && StaticMeshComponent->ComponentHasTag("MyObelisk"))
				{
					AActor* HitActor = StaticMeshComponent->GetOwner();
					if (HitActor && HitActor->ActorHasTag("BP_ObeliskDestroy"))
					{
						HitActor->ProcessEvent(HitActor->FindFunction("DestroyObelisk"), nullptr);
					}
				}
				// Check if the hit actor is the desired one
				AActor* HitActor = HitResult.GetActor();
				if (HitActor && HitActor->ActorHasTag("BP_ObeliskDestroy_2"))
				{
					// Call the custom event on the hit actor
					HitActor->ProcessEvent(HitActor->FindFunction("DestroyObelisk"), nullptr);
					UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *HitActor->GetName());
				}
				// Check if the HitResult has an actor and logs a warning message to the console that includes the name of the actor that was hit by the line trace.
				if (HitResult.GetActor())
				{
					UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *HitResult.GetActor()->GetName());
				}
			}
			if (!StaticMeshComponent)
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit component is not a static mesh component with tag 'MyObelisk'"));
			}

			// Create an array of the material instance paths
			UPROPERTY(EditDefaultsOnly, Category = "Materials")
			MaterialInstancePaths = { "/Game/_Game/MaterialInstance/MI_EndlessTunnel_3",
										"/Game/_Game/MaterialInstance/MI_NoiseWorleyChebyshev",
										"/Game/_Game/MaterialInstance/MI_FresnelRefraction" };

			TArray<TAssetPtr<UMaterialInstanceDynamic>> MaterialInstances;

			for (const FString& MaterialInstancePath : MaterialInstancePaths)
			{
				const FString PrimaryAssetPath = FPackageName::ObjectPathToPackageName(MaterialInstancePath);
				UMaterialInterface* MaterialInterface = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *PrimaryAssetPath, nullptr, LOAD_None, nullptr));
				if (MaterialInterface)
				{
					UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(MaterialInterface, nullptr);
					MaterialInstances.Add(MaterialInstance);
				}
			}

			// Get a random index for the material
			int32 MaterialIndex = FMath::RandRange(0, MaterialInstancePaths.Num() - 1);

			// Load the material at the random index
			UMaterialInterface* MaterialInterface = LoadObject<UMaterialInterface>(nullptr, *MaterialInstancePaths[MaterialIndex]);

			// Play a sound cue at the hit location
			USoundBase* SoundCue = LoadObject<USoundBase>(nullptr, TEXT("/Game/_Game/Assets/Sounds/Glitch/SC_Glitch"));
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundCue, HitResult.ImpactPoint);

			if (MaterialInterface)
			{
				for (int32 i = 0; i < StaticMeshComponent->GetNumMaterials(); ++i)
				{
					StaticMeshComponent->SetMaterial(i, MaterialInterface);
				}
			}
		}
	}
	else
	{
		BulletExpiry += DeltaTime;

		// Draw Debug Line and specify the true argument to make the debug line persistent
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(FColor(0.0f, -BulletExpiry * 80.0f, 100.0f)), true);

		BulletMesh->SetWorldLocation(EndTrace);

		ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletClass,EndTrace, FRotator::ZeroRotator);
		if (Bullet != nullptr)
		{
			FVector BulletDirection = EndTrace - StartTrace;
			BulletDirection.Normalize();
			Bullet->SetVelocity(BulletDirection * BulletSpeed);
		}

		BulletSpeed += FVector(0.0f, 0.0f, -200.0f) * DeltaTime;
	}
	
	if(BulletExpiry > 3)
	{
		Destroy();
	}
	
}
