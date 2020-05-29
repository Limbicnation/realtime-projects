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

	TransitionAmount = 1.f;
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

	if (bRustEffectTrigger)
	{
		if (TransitionAmount > 0.f)
		{
			TransitionAmount -= DeltaTime;
		}
	}

	if (bRustEffectTrigger)
	{
		if (TransitionAmount < 2.f)
		{
			TransitionAmount += DeltaTime;
		}
	}

	UMaterialInstanceDynamic* TransitionMaterialInstance = CubeMesh->CreateDynamicMaterialInstance(0);

	if (TransitionMaterialInstance != nullptr)
	{
		TransitionMaterialInstance->SetScalarParameterValue(FName("TransitionAmount"), TransitionAmount);
	}
}

void ADynamicMaterialChanger::OnPlayerTriggerTransition(UPrimitiveComponent* OverlappedComp, AActor* otherActor,
                                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                        bool bFromSweep,
                                                        const FHitResult& SweepResult)
{
	bRustEffectTrigger = true;
	GEngine->AddOnScreenDebugMessage(-5, 5.f, FColor::Green, TEXT("You've entered the Box Trigger!"));
}

void ADynamicMaterialChanger::OnPlayerExitTransition(UPrimitiveComponent* OverlappedComp, AActor* otherActor,
                                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bRustEffectTrigger = false;
	GEngine->AddOnScreenDebugMessage(-5, 5.f, FColor::Green, TEXT("You've exited the Box Trigger!"));
}
