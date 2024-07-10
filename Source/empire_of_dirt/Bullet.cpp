#include "Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/FileHelper.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABullet::ABullet()
{
    PrimaryActorTick.bCanEverTick = true;

    RootComp = CreateDefaultSubobject<USceneComponent>("RootComp");
    SetRootComponent(RootComp);

    BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");
    BulletMesh->SetupAttachment(RootComp);

    BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>("BulletMovement");
    BulletMovement->InitialSpeed = 2500.0f;
    BulletMovement->MaxSpeed = 5000.0f;
    BulletMovement->bInitialVelocityInLocalSpace = true;

    BulletMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    BulletMesh->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

    BulletExpiry = 0.0f;
    DestroyDelay = 3.0f;
    BulletSpeed = FVector(5000.0f, 0.0f, 0.0f);

    // Initialize the MaterialInstancePaths array with valid paths
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
}

void ABullet::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FHitResult HitResult;

    FVector StartTrace = this->GetActorLocation();
    FVector EndTrace = (BulletSpeed * DeltaTime) + StartTrace;
    EndTrace.Z += this->GetActorRotation().Pitch;

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, CollisionParams))
    {
        BulletMovement->InitialSpeed = BulletSpeed.Size();

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

            if (MaterialInstancePaths.Num() > 0)
            {
                int32 MaterialIndex = FMath::RandRange(0, MaterialInstancePaths.Num() - 1);
                if (MaterialInstancePaths.IsValidIndex(MaterialIndex))
                {
                    UMaterialInterface* MaterialInterface = LoadObject<UMaterialInterface>(nullptr, *MaterialInstancePaths[MaterialIndex]);

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
        }

        Destroy();
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

void ABullet::SetVelocity(const FVector& NewVelocity)
{
    BulletMesh->SetPhysicsLinearVelocity(NewVelocity);
}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this && OtherComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("Bullet hit: %s"), *OtherActor->GetName());
        // UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), /* Your particle system */, GetActorLocation()); // Commented out until you have a particle system
        Destroy();
    }
}
