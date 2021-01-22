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

	//PlayerLocation = FVector(300, 0, 1);
	//TextLocation = FVector(400, 0, 0);
	Dimensions = FVector(300, 0, 1);
	AxisVector = FVector(0, 0, 1);
	Multiplier = 50.f;
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

	ATextFacingPlayer* MyPlayer = Cast<ATextFacingPlayer>(
		UGameplayStatics::GetPlayerCharacter(this, 0));

	//MakeTextFacePlayer();
	FRotator NewRotation = FRotator(0, AngleAxis, 0);

	MyText->SetRelativeRotation(NewRotation);

	FVector NewLocation = FVector(0, 50, 0);
	FQuat QuatRotation = FQuat(NewRotation * -1);

	Super::Tick(DeltaTime);
	if (USceneComponent* RootComp = GetRootComponent())
	{
		if (APawn* LocalPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			this->SetActorRotation(
				(RootComp->GetComponentLocation() - LocalPawn->GetActorLocation()).GetSafeNormal().Rotation());
		}
	}
}


/* void ATextFacingPlayer::MakeTextFacePlayer()
{
	ATextFacingPlayer* Character = Cast<ATextFacingPlayer>(
		UGameplayStatics::GetPlayerCharacter(this, 0));

	FRotator NewRotation = FRotator(0, AngleAxis, 0);
	FVector NewLocation = FVector(0, 50, 0);

	//FQuat QuatRotation = FQuat(NewRotation);
	//SetActorLocationAndRotation(NewLocation, QuatRotation, false, 0, ETeleportType::None);

	//FVector NewLocation = Dimensions.RotateAngleAxis(AngleAxis, AxisVector);

	//FRotator NewRotation = Character->GetActorRotation();
	//FRotator NewRotation = FRotator(0, AngleAxis, 0);
	//NewRotation.Yaw = 180.f;

	//MyText->SetRelativeRotation(NewRotation);


	//FQuat QuatRotation = FQuat(NewRotation);

	//SetActorLocationAndRotation(NewLocation, QuatRotation, false, nullptr, ETeleportType::None);
}
*/
