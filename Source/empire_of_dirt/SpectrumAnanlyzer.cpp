// Fill out your copyright notice in the Description page of Project Settings.


#include "SpectrumAnanlyzer.h"
#include "TimeSynthComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Math/Rotator.h"
#include "UnitConversion.h"

// Sets default values
ASpectrumAnalyzer::ASpectrumAnalyzer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpectrumBarSpacing = 100.f;
	SpectrumBarFrequency = 250.f;

	PitchValue = 0.f;
	YawValue = 0.f;
	RollValue = 0.f;

	TimeSynthComponent = CreateDefaultSubobject<UTimeSynthComponent>("TimeSynthComponent");
	TimeSynthComponent->bEnableSpectralAnalysis = true;

	SetRootComponent(TimeSynthComponent);

	for (int32 i = 1; i <= 20; ++i)
	{
		// Look at Frequency e.g (50, 150, 250)
		TimeSynthComponent->FrequenciesToAnalyze.Add(1 * SpectrumBarFrequency);

		FString SpectrumBarName = FString::Printf(TEXT("SpectrumBar%dHz"), i * 50);

		UStaticMeshComponent* SpectrumBar = CreateDefaultSubobject<UStaticMeshComponent>(*SpectrumBarName);
		SpectrumBar->SetupAttachment(TimeSynthComponent);
		SpectrumBar->SetStaticMesh(SpectrumBarMesh);
		SpectrumBar->SetRelativeLocation(FVector(i * SpectrumBarSpacing, 0.f, 0.f));
		SpectrumBars.Add(SpectrumBar);


	}
}

void ASpectrumAnalyzer::BeginPlay()
{
	Super::BeginPlay();

	Refresh();

	TimeSynthComponent->PlayClip(TimeSynthClip);
}

// Called when the game starts or when spawned
/*
void ASpectrumAnalyzer::BeginPlay()
{
	Super::BeginPlay();

	
	Refresh();

	TimeSynthComponent->PlayClip(TimeSynthClip); 
} */

void ASpectrumAnalyzer::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	Refresh();
}


void ASpectrumAnalyzer::Refresh()
{
	for (int32 i = 1; i <= 20; ++i)
	{
		if (SpectrumBars.IsValidIndex(i))
		{
			UStaticMeshComponent* SpectrumBar = SpectrumBars[i];
			SpectrumBar->SetStaticMesh(SpectrumBarMesh);
			SpectrumBar->SetRelativeLocation(FVector((i + 1) * SpectrumBarSpacing, 0.f, 0.f));

		}
	}
}

// Called every frame
void ASpectrumAnalyzer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (auto SpecData : TimeSynthComponent->GetSpectralData())
	{
		UStaticMeshComponent* SpectrumBar = SpectrumBars[SpecData.FrequencyHz / 50.f - 1];

		// Update Bar ScaleZ
		FVector BarScaleZ = SpectrumBar->GetComponentScale();
		// Set Bar intensity
		(BarScaleZ.Z = 1.f + SpecData.Magnitude * 0.2f) * BarScaleZ;
		SpectrumBar->SetWorldScale3D(FMath::VInterpTo(SpectrumBar->GetComponentScale(), BarScaleZ, DeltaTime, 5.f));

		// Update Bar ScaleY
		FVector BarScaleY = SpectrumBar->GetComponentScale();
		// Set Bar intensity 
		(BarScaleY.Y = 1.f + SpecData.Magnitude * 0.2f) * BarScaleY;
		SpectrumBar->SetWorldScale3D(FMath::VInterpTo(SpectrumBar->GetComponentScale(), BarScaleY, DeltaTime, 5.f));

		// Update Bar ScaleX
		FVector BarScaleX = SpectrumBar->GetComponentScale();
		// Set Bar intensity
		(BarScaleY.Y = 1.f + SpecData.Magnitude * 0.2f) * BarScaleX;
		SpectrumBar->SetWorldScale3D(FMath::VInterpTo(SpectrumBar->GetComponentScale(), BarScaleX, DeltaTime, 5.f));

		// Rotate Actor
		FQuat QuatRotation = FQuat(FRotator(PitchValue, YawValue, DeltaTime));

		AddActorLocalRotation(QuatRotation, false, nullptr, ETeleportType::None);
	}
}
