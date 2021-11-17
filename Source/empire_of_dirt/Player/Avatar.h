// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/InputComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Engine.h"
#include "Avatar.generated.h"


UCLASS()
class EMPIRE_OF_DIRT_API AAvatar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAvatar();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CubeMesh;

	UPROPERTY(EditAnywhere)
	class UFloatingPawnMovement * PlayerMovement;

	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraMesh;
	UCameraComponent* Camera;
	int CameraComponent;

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	/** Handle Player Movement */

	void MoveForward(float Val);
	void MoveRight(float Val);

	void LookUp(float Val);
	void Turn(float Val);

	/** Called via input to turn at a given rate
	* @parm Rate This is a normalized rate i.e. 1.0 mean 100% rate
	*/
	void TurnRate(float Rate);

	/** Called via input to Look up at a given rate
    * @parm Rate This is a normalized rate i.e. 1.0 mean 100% rate
    */
	void LookUpRate(float Rate);

	/** Handle Player sprinting */

	void BeginSprinting();

	void EndSprinting();

	bool bIsSprinting = false;

	UPROPERTY(EditAnywhere)
	float SprintingValue;

	UPROPERTY(EditAnywhere)
	float WalkingValue;

	/* Handle shooting*/

	void shoot();

	UPROPERTY(EditAnywhere, Category = "Shooting")
	TSubclassOf<class ABullet> BulletClass;


public:
	// Called every frame
	void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Camera that follows the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Base turn rate in deg/sec Other scaling may effect final turn rate */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	/** Base look up/down rate in deg/sec Other scaling may effect final turn rate */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	/** Variable for shooting Sound Cues for Player Character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class USoundCue* ShootSound;


public:
	/** Returns camera boom sub object */
	FORCEINLINE USpringArmComponent* GetCameraBoom() const {return CameraBoom;}
	/** Returns follow camera  sub object */
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera;}

};
