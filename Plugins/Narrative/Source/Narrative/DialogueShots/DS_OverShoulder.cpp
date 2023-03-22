//  Copyright Narrative Tools 2022.

#include "DS_OverShoulder.h"
#include "Dialogue.h"

UDS_OverShoulder::UDS_OverShoulder()
{
	SpeakerOffset = FVector(150.f);
}

FTransform UDS_OverShoulder::GetShotTransform_Implementation(class UDialogue* Dialogue, class AActor* SpeakingActor, class AActor* ListeningActor)
{
	FTransform ShotTransform;

	if (SpeakingActor && ListeningActor && Dialogue)
	{
		FVector SpeakerEyesLoc;
		FRotator SpeakerEyesRot;
		SpeakingActor->GetActorEyesViewPoint(SpeakerEyesLoc, SpeakerEyesRot);

		FVector ListenerEyesLoc;
		FRotator ListenerEyesRot;
		ListeningActor->GetActorEyesViewPoint(ListenerEyesLoc, ListenerEyesRot);

		const FVector FocalPoint = (SpeakerEyesLoc + ListenerEyesLoc) * 0.5f;

		const FVector FwdOffset = ListeningActor->GetActorForwardVector() * SpeakerOffset.X;
		const FVector RightOffset = Dialogue->OwningPawn->GetActorRightVector() * SpeakerOffset.Y;
		const FVector UpOffset = ListeningActor->GetActorUpVector() * SpeakerOffset.Z;

		const FVector FinalLoc = ListenerEyesLoc + FwdOffset + RightOffset + UpOffset;
		FRotator FinalRot = FRotationMatrix::MakeFromX(FocalPoint - FinalLoc).Rotator();

		const float Direction = Dialogue->OwningPawn == SpeakingActor ? 1.f : -1.f;
		FinalRot.Yaw += TiltYawDegrees * Direction;

		ShotTransform.SetLocation(FinalLoc);
		ShotTransform.SetRotation(FinalRot.Quaternion());
	}

	return ShotTransform;
}
