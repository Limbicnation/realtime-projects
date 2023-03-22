// Copyright Narrative Tools 2022. 

#include "NarrativeDialogueShot.h"
#include "Dialogue.h"

FTransform UNarrativeDialogueShot::GetShotTransform_Implementation(class UDialogue* Dialogue, class AActor* SpeakingActor, class AActor* ListeningActor)
{
	//Position the camera to be looking at the speaker 
	if (SpeakingActor)
	{
		const AActor* NPCActor = Dialogue->NPCActor;
		const AActor* OwningPawn = Dialogue->OwningPawn;

		FVector ListeningActorEyesViewLoc;
		FRotator ListeningActorEyesViewRot;
		ListeningActor->GetActorEyesViewPoint(ListeningActorEyesViewLoc, ListeningActorEyesViewRot);

		FVector SpeakingActorEyesViewLoc;
		FRotator SpeakingActorEyesViewRot;
		SpeakingActor->GetActorEyesViewPoint(SpeakingActorEyesViewLoc, SpeakingActorEyesViewRot);

		//A normalized vector describing the direction our player is looking at the NPC in
		const FVector EyeLineVect = (OwningPawn->GetActorLocation() - NPCActor->GetActorLocation()).GetSafeNormal() * 200.f;
		const FVector CameraRotationPoint = (SpeakingActorEyesViewLoc + ListeningActorEyesViewLoc) * 0.5f;

		return FTransform();
	}


	return FTransform();
}
