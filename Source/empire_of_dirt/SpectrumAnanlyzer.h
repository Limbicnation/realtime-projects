// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <ObjectMacros.h>
#include "SpectrumAnanlyzer.generated.h"

UCLASS()
class EMPIRE_OF_DIRT_API ASpectrumAnalyzer : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpectrumAnalyzer();

protected:
	/*
	// Called when the game starts or when spawned    
	/* virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override; */

	void BeginPlay();
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);

	void Refresh();

	UPROPERTY(EditAnywhere, Category = "Audio")
		class UTimeSynthComponent* TimeSynthComponent;

	UPROPERTY(EditAnywhere, Category = "Audio")
		class UTimeSynthClip* TimeSynthClip;

	UPROPERTY(EditAnywhere, Category = "Audio")
		TArray<UStaticMeshComponent*> SpectrumBars;

	UPROPERTY(EditAnywhere, Category = "Audio")
		UStaticMesh* SpectrumBarMesh;

	UPROPERTY(EditAnywhere, Category = "Audio")
		float SpectrumBarFrequency;

	// Amount of spacing between each bar
	UPROPERTY(EditAnywhere, Category = "Audio")
		float SpectrumBarSpacing;

	// Edit bar location alignment 
	UPROPERTY(EditAnywhere, Category = "Audio")
		float BarLocation;

	UPROPERTY(EditAnywhere, Category = "Audio")
		float BarScaleY;

	UPROPERTY(EditAnywhere, Category = "Audio")
		float BarScaleZ;

	UPROPERTY(EditAnywhere, Category = "Audio")
		float BarScaleX;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float YawValue;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float RollValue;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float PitchValue;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
