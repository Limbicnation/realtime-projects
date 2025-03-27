#include "Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABullet::ABullet()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create root component
    RootComp = CreateDefaultSubobject<USceneComponent>("RootComp");
    SetRootComponent(RootComp);

    // Setup bullet mesh
    BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");
    BulletMesh->SetupAttachment(RootComp);
    BulletMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    BulletMesh->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

    // Setup projectile movement - This will handle all movement
    BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>("BulletMovement");
    BulletMovement->InitialSpeed = 2500.0f;
    BulletMovement->MaxSpeed = 5000.0f;
    BulletMovement->bRotationFollowsVelocity = true;
    BulletMovement->bShouldBounce = false;
    BulletMovement->ProjectileGravityScale = 0.0f; // No gravity for laser/energy projectiles

    // Set default lifetime
    InitialLifeSpan = 3.0f; // Destroy after 3 seconds

    // Initialize the MaterialInstancePaths array
    MaterialInstancePaths = {
        "/Game/_Game/MaterialInstance/MI_EndlessTunnel_3",
        "/Game/_Game/MaterialInstance/MI_NoiseWorleyChebyshev",
        "/Game/_Game/MaterialInstance/MI_FresnelRefraction",
        "/Game/_Game/MaterialInstance/MI_Twigl_01",
        "/Game/_Game/MaterialInstance/MI_VPCube"
    };
}

void ABullet::BeginPlay()
{
    Super::BeginPlay();

    // Initialize the bullet movement with the owner's rotation
    if (BulletMovement)
    {
        // Make sure the velocity is properly initialized
        BulletMovement->Velocity = GetActorForwardVector() * BulletMovement->InitialSpeed;
        BulletMovement->ProjectileGravityScale = 0.0f; // No gravity for sci-fi projectiles
    }

    // Set the bullet's lifetime
    SetLifeSpan(DestroyDelay);
}

void ABullet::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // We're now using the ProjectileMovementComponent for movement
    // No need for custom movement logic in Tick

    // Optional: Add trail effect
    // DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() - GetVelocity() * DeltaTime, 
    //              FColor(0, 100, 255), false, 1.0f);
}

void ABullet::SetVelocity(const FVector& NewVelocity)
{
    if (BulletMovement)
    {
        // Use ProjectileMovementComponent to set the velocity
        BulletMovement->Velocity = NewVelocity;
    }
}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this && OtherComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("Bullet hit: %s"), *OtherActor->GetName());

        // Apply material change effect to hit object
        UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(OtherComponent);
        if (StaticMeshComponent)
        {
            // Check for special tag interactions
            if (StaticMeshComponent->ComponentHasTag("MyObelisk"))
            {
                AActor* HitActor = StaticMeshComponent->GetOwner();
                if (HitActor && HitActor->ActorHasTag("BP_ObeliskDestroy"))
                {
                    HitActor->ProcessEvent(HitActor->FindFunction("DestroyObelisk"), nullptr);
                }
            }

            // Apply random material from the array
            if (MaterialInstancePaths.Num() > 0)
            {
                int32 MaterialIndex = FMath::RandRange(0, MaterialInstancePaths.Num() - 1);
                if (MaterialInstancePaths.IsValidIndex(MaterialIndex))
                {
                    UMaterialInterface* MaterialInterface = LoadObject<UMaterialInterface>(nullptr, *MaterialInstancePaths[MaterialIndex]);

                    // Play impact sound
                    USoundBase* SoundCue = LoadObject<USoundBase>(nullptr, TEXT("/Game/_Game/Assets/Sounds/Glitch/SC_Glitch"));
                    UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundCue, Hit.ImpactPoint);

                    // Apply material to all slots
                    if (MaterialInterface)
                    {
                        for (int32 i = 0; i < StaticMeshComponent->GetNumMaterials(); ++i)
                        {
                            StaticMeshComponent->SetMaterial(i, MaterialInterface);
                        }
                    }
                }
            }
        }

        // Optional: Spawn impact effect
        // UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());

        // Destroy the bullet
        Destroy();
    }
}