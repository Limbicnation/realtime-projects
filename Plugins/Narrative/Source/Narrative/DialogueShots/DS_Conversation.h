//  Copyright Narrative Tools 2022.

#pragma once

#include "CoreMinimal.h"
#include "NarrativeDialogueShot.h"
#include "DS_Conversation.generated.h"

/**
 * A shot centered on the middle of the conversation, but with a fixed offset in the players space.
 * Allows you to easily set up birds eye shots, side on shots, etc. 
 */
UCLASS()
class NARRATIVE_API UDS_Conversation : public UNarrativeDialogueShot
{
	GENERATED_BODY()

protected:

	UDS_Conversation();

	/**
	The offset from the conversation, for example (0, 0, 1000) would set up a birds eye view shot, (0, 500, 0) would be a side on shot etc.
	*/
	UPROPERTY(EditAnywhere, Category = "Shot Settings")
	FVector Offset;

	virtual FTransform GetShotTransform_Implementation(class UDialogue* Dialogue, class AActor* SpeakingActor, class AActor* ListeningActor) override;


};