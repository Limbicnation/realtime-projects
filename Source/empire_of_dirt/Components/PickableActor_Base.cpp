#include "Components/PickableActor_Base.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SpawnBox.h"
#include "GameFramework/RotatingMovementComponent.h"

// Sets default values
APickableActor_Base::APickableActor_Base()
{
	// Create the static mesh component
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetMobility(EComponentMobility::Movable);
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Set up the collision callback function to be called when a collision is detected
	MeshComp->OnComponentHit.AddDynamic(this, &APickableActor_Base::OnHit);

	// Initialize the sphere component
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->SetupAttachment(MeshComp);

	// Initialize the sphere component with a default radius of 30.0f
	CollisionComp->InitSphereRadius(30.0f);

	// Set collision comp to overlap with all dynamic
	CollisionComp->SetCollisionProfileName("OverlapAllDynamic");

	// Set the sphere component as the root component
	SetRootComponent(CollisionComp);

	// Add rotation
	RotationComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotationComp"));

	// Set this actor to not call Tick() every frame to improve performance
	PrimaryActorTick.bCanEverTick = false;
}

void APickableActor_Base::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Check if the other actor is the spawn box (you will need to replace this with your own logic)
	if (OtherActor->IsA(ASpawnBox::StaticClass()))
	{
		// Add a force to the static mesh component in the X direction
		float ForceMagnitude = 1000.0f;
		FVector Force = FVector(ForceMagnitude, 0.0f, 0.0f);
		MeshComp->AddForce(Force, NAME_None, true);
	}
}

// Called when the game starts or when spawned
void APickableActor_Base::BeginPlay()
{
	Super::BeginPlay();

	// Generate a different random scale for each instance of the static mesh
	MeshComp->SetWorldScale3D(FVector(FMath::RandRange(0.5f, 20.0f), FMath::RandRange(0.5f, 20.0f), FMath::RandRange(0.5f, 20.0f)));
}
