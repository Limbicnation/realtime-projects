// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicMaterialChanger.h"

// Sets default values
ADynamicMaterialChanger::ADynamicMaterialChanger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CubeRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CubeRoot"));
	RootComponent = CubeRoot;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	CubeMesh->AttachToComponent(CubeRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	CubeTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("CubeTrigger"));
	CubeTrigger->SetGenerateOverlapEvents(true);
	CubeTrigger->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
	CubeTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADynamicMaterialChanger::OnPlayerTriggerTransition);
	CubeTrigger->OnComponentEndOverlap.AddDynamic(this, &ADynamicMaterialChanger::OnPlayerExitTransition);
	CubeTrigger->AttachToComponent(CubeRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	TransitionAmount = 0.f;
}



// Called when the game starts or when spawned
void ADynamicMaterialChanger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADynamicMaterialChanger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRustEffectTriggered)
	{
		if (TransitionAmount > 0.0f)
		{
			TransitionAmount -= DeltaTime;
		}
	}

	if (bRustEffectTriggered)
	{
		if (TransitionAmount < 1.0f)
		{
			TransitionAmount += DeltaTime;
		}
	}

	UMaterialInstanceDynamic* MI_TeddyBear = CubeMesh->CreateDynamicMaterialInstance(0);

	if (MI_TeddyBear != nullptr)
	{
		MI_TeddyBear->SetScalarParameterValue(FName("TransitionAmount"), TransitionAmount);
	}
}

void ADynamicMaterialChanger::OnPlayerTriggerTransition(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                                        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                        bool bFromSweep, const FHitResult& SweepResult)
{
	bRustEffectTriggered = true;
	
}

void ADynamicMaterialChanger::OnPlayerExitTransition(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                                     class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bRustEffectTriggered = false;
	
}
