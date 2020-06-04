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
	// Sets default values for this actor's properties
	ATextFacingPlayer();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

public:
	// Called every frame
	void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloatingText")
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloatingText")
	UTextRenderComponent* MyText;

	void MakeTextFacePlayer();
};
