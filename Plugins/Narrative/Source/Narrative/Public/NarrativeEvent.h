// Copyright Narrative Tools 2022. 

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NarrativeEvent.generated.h"

/**
 * Subclass this in BP to execute some logic when a particular quest step or dialogue option is reached  */
UCLASS(Abstract, Blueprintable, EditInlineNew, AutoExpandCategories = ("Default"))
class NARRATIVE_API UNarrativeEvent : public UObject
{
	GENERATED_BODY()

public:

	// Allows the Object to get a valid UWorld from it's outer.
	virtual UWorld* GetWorld() const override
	{
		if (HasAllFlags(RF_ClassDefaultObject))
		{
			// If we are a CDO, we must return nullptr instead of calling Outer->GetWorld() to fool UObject::ImplementsGetWorld.
			return nullptr;
		}

		UObject* Outer = GetOuter();

		while (Outer)
		{
			UWorld* World = Outer->GetWorld();
			if (World)
			{
				return World;
			}

			Outer = Outer->GetOuter();
		}

		return nullptr;
	}


	/**
	If true, run this event on the players shared narrative comp instead of their local one
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
	bool bUseSharedComponent = false;

	UFUNCTION(BlueprintNativeEvent, Category = "Event")
	bool ExecuteEvent(APawn* Pawn, APlayerController* Controller, class UNarrativeComponent* NarrativeComponent);
	virtual bool ExecuteEvent_Implementation(APawn* Pawn, APlayerController* Controller, class UNarrativeComponent* NarrativeComponent);

	/**Define the text that will show up on a node if this condition is added to it */
	UFUNCTION(BlueprintNativeEvent, Category = "Event")
	FString GetGraphDisplayText();
	virtual FString GetGraphDisplayText_Implementation();
};
