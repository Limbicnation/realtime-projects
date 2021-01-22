// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "TextFacingPlayer.generated.h"

UCLASS()
class EMPIRE_OF_DIRT_API ATextFacingPlayer : public AActor
{
	GENERATED_BODY()

public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloatingText")
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloatingText")
	UTextRenderComponent* MyText;

	// Sets default values for this actor's properties
	ATextFacingPlayer();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MakeTextFacePlayer();

	UPROPERTY(EditAnywhere, Category=Movement)
	FVector Dimensions;

	UPROPERTY(EditAnywhere, Category=Movement)
	FVector AxisVector;

	UPROPERTY(EditAnywhere, Category = Movement)
	float AngleAxis;

	UPROPERTY(EditAnywhere, Category=Movement)
	float Multiplier;
};
