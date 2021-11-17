// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AvatarAnimInstance.h"
#include "Avatar.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAvatarAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (Avatar == nullptr)
	{
		Avatar = Cast<AAvatar>(TryGetPawnOwner());
	}
	if (Avatar)
	{
		// get the speed of character velocity
		FVector Velocity{ Avatar->GetVelocity() };
		Velocity.Z = 0;
		Speed = Velocity.Size();

		// is the character in air?
		bIsInAir = Avatar->GetCharacterMovement()->IsFalling();

		// Is the character accelerating?
		if (Avatar->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
		{
			bIsAccelerating = true;
		}
		else
		{
			bIsAccelerating = false;
		}
	}
}

void UAvatarAnimInstance::NativeInitializeAnimation()
{
	Avatar = Cast<AAvatar>(TryGetPawnOwner());
}
