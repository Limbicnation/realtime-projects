// Fill out your copyright notice in the Description page of Project Settings.

#include "TextFacingPlayer.h"

// Sets default values
ATextFacingPlayer::ATextFacingPlayer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("RotatingText"));
	MyText->SetWorldSize(200.f);
	MyText->SetTextRenderColor(FColor::Green);
	MyText->SetHorizontalAlignment(EHTA_Center);
}

// Called when the game starts or when spawned
void ATextFacingPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATextFacingPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	MakeTextFacePlayer();
}

void ATextFacingPlayer::MakeTextFacePlayer()
{
	ATextFacingPlayer* Character = Cast<ATextFacingPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));

	//FRotator NewRotation = Character->GetActorRotation();
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += 180.f;
	SetActorRotation(NewRotation);

	//MyText->SetRelativeRotation(NewRotation);
}
