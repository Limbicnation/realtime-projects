// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "TextFacingPlayer.generated.h"

UCLASS()
class SICKPLUGIN_API ATextFacingPlayer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATextFacingPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UTextRenderComponent MyText;

	void MakeTextFacePlayer();


};
