//  Copyright Narrative Tools 2022.

#pragma once

#include "CoreMinimal.h"
#include "NarrativeDialogueShot.h"
#include "DS_OverShoulder.generated.h"

/**
 * A shot centered on the middle of the conversation, but offset from the listeners shoulder. Automatically adheres to the 180 degree rule
 */
UCLASS()
class NARRATIVE_API UDS_OverShoulder : public UNarrativeDialogueShot
{
	GENERATED_BODY()

protected:

	UDS_OverShoulder();

	/**
	The cameras offset from the listeners shoulder. Note that the Y (left to right) direction is relative to the players character,
	not the person listening. This is because otherwise the Y direction would flip each time and each character would be on the same side
	of the screen, which you generally want to avoid with a cinematic shot - each character should be on opposite sides of the screen. 
	*/
	UPROPERTY(EditAnywhere, Category = "Shot Settings")
	FVector SpeakerOffset;

	//The tilt in degrees, automatically applied in different directions for each speaker (so each character stays on their own side of the screen)
	UPROPERTY(EditAnywhere, Category = "Shot Settings")
	float TiltYawDegrees;

	virtual FTransform GetShotTransform_Implementation(class UDialogue* Dialogue, class AActor* SpeakingActor, class AActor* ListeningActor) override;

	
};
