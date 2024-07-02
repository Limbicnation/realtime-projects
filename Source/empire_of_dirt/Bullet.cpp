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

ABullet::ABullet()
{
    PrimaryActorTick.bCanEverTick = true;

    BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");
    SetRootComponent(BulletMesh);

    RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
    RootComponent = RootComp;

    BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>("BulletMovement");
    BulletMovement->InitialSpeed = 0.0f;
    BulletMovement->MaxSpeed = 5000.0f;
    BulletMovement->bInitialVelocityInLocalSpace = true;

    BulletExpiry = 0.0f;
    DestroyDelay = 3.0f;
    BulletSpeed = FVector(5000.0f, 0.0f, 0.0f);
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

    FName ComponentTag = FName("MyObelisk");

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

            int32 MaterialIndex = FMath::RandRange(0, MaterialInstancePaths.Num() - 1);

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
