//  Copyright Narrative Tools 2022.


#include "DS_Conversation.h"
#include "Dialogue.h"

UDS_Conversation::UDS_Conversation()
{
	Offset = FVector(0.f, 500.f, 0.f);
 }

FTransform UDS_Conversation::GetShotTransform_Implementation(class UDialogue* Dialogue, class AActor* SpeakingActor, class AActor* ListeningActor)
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
	
		const FVector ShotOffset = 
			(Dialogue->OwningPawn->GetActorForwardVector() * Offset.X) + 
			(Dialogue->OwningPawn->GetActorRightVector() * Offset.Y) + 
			(Dialogue->OwningPawn->GetActorUpVector() * Offset.Z);

		const FVector FinalLoc = FocalPoint + ShotOffset;
		const FRotator FinalRot = FRotationMatrix::MakeFromX(FocalPoint - FinalLoc).Rotator();

		ShotTransform.SetLocation(FinalLoc);
		ShotTransform.SetRotation(FinalRot.Quaternion());
	}

	return ShotTransform;
}
