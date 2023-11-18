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

	// Initialize the Scene Component for Spawning the bullet class
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>("BulletMovement");
	BulletMovement->InitialSpeed = 0.0f; // Set initial speed to 0
	BulletMovement->MaxSpeed = 5000.0f;   // Adjust the max speed as needed
	BulletMovement->bInitialVelocityInLocalSpace = true; // Important for local space velocity

	BulletExpiry = 0.0f; // Initialize BulletExpiry
	DestroyDelay = 3.0f; // Set the desired destroy delay
	BulletSpeed = FVector(5000.0f, 0.0f, 0.0f); // Initialize BulletSpeed
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
		BulletMovement->InitialSpeed = BulletSpeed.Size(); // Update initial speed based on the desired velocity

		// Check if a line trace hits any actor with the specified component tag
		UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(HitResult.GetComponent());
		if (StaticMeshComponent)
		{
			if (StaticMeshComponent->ComponentHasTag("MyObelisk"))
			{
				AActor* HitActor = StaticMeshComponent->GetOwner();
				if (HitActor && HitActor->ActorHasTag("BP_ObeliskDestroy"))
				{
					HitActor->ProcessEvent(HitActor->FindFunction("DestroyObelisk"), nullptr);
				}
			}

			// ... (Rest of the code for handling other tags and logic)

			// Create an array of the material instance paths
			TArray<FString> MaterialInstancePaths = {
				"/Game/_Game/MaterialInstance/MI_EndlessTunnel_3",
				"/Game/_Game/MaterialInstance/MI_NoiseWorleyChebyshev",
				"/Game/_Game/MaterialInstance/MI_FresnelRefraction",
				"/Game/_Game/MaterialInstance/MI_Twigl_01",
				"/Game/_Game/MaterialInstance/MI_VPCube"
			};

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
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit component is not a static mesh component with tag 'MyObelisk'"));
		}

		if (BulletExpiry > DestroyDelay)
		{
			Destroy();
		}
	}
	else
	{
		BulletExpiry += DeltaTime;

		if (BulletExpiry <= DestroyDelay)
		{
			DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(0.0f, -BulletExpiry * 80.0f, 100.0f), true);
		}

		if (BulletExpiry > DestroyDelay)
		{
			Destroy();
		}
	}
}