//  Copyright Narrative Tools 2022.

#include "DS_Speaker.h"
#include "Dialogue.h"


UDS_Speaker::UDS_Speaker()
{
	SpeakerOffset = FVector(150.f, 65.f, 0.f);
	TiltYawDegrees = 5.f;
}

FTransform UDS_Speaker::GetShotTransform_Implementation(class UDialogue* Dialogue, class AActor* SpeakingActor, class AActor* ListeningActor)
{
	FTransform ShotTransform;

	if (SpeakingActor && Dialogue)
	{
		FVector SpeakerEyesLoc;
		FRotator SpeakerEyesRot;
		SpeakingActor->GetActorEyesViewPoint(SpeakerEyesLoc, SpeakerEyesRot);

		const FVector FwdOffset = SpeakingActor->GetActorForwardVector() * SpeakerOffset.X;
		const FVector RightOffset = Dialogue->OwningPawn->GetActorRightVector() * SpeakerOffset.Y;
		const FVector UpOffset = SpeakingActor->GetActorUpVector() * SpeakerOffset.Z;

		const FVector FinalLoc = SpeakerEyesLoc + FwdOffset + RightOffset + UpOffset;
		FRotator FinalRot = FRotationMatrix::MakeFromX(SpeakerEyesLoc - FinalLoc).Rotator();

		const float Direction = Dialogue->OwningPawn == SpeakingActor ? 1.f : -1.f;
		FinalRot.Yaw += TiltYawDegrees * Direction;

		ShotTransform.SetLocation(FinalLoc);
		ShotTransform.SetRotation(FinalRot.Quaternion());
	}

	return ShotTransform;
}

