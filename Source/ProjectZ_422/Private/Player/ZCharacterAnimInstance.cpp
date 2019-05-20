// Fill out your copyright notice in the Description page of Project Settings.


#include "ZCharacterAnimInstance.h"
#include "ZCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ConstructorHelpers.h"

UZCharacterAnimInstance::UZCharacterAnimInstance()
{
	CurrentPawnSpeed = 0.f;
	CurrentPawnDirection = 0.f;
	bIsFalling = false;
	bIsCrouching = false;
	bIsEquipWeapon = false;
}

void UZCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		FVector Speed = Pawn->GetVelocity();
		Speed.Z = 0.f;
		CurrentPawnSpeed = Speed.Size();
		CurrentPawnDirection = CalculateDirection(Pawn->GetVelocity(), Pawn->GetActorRotation());

		auto Character = Cast<AZCharacter>(Pawn);
		if (Character)
		{
			bIsFalling = Character->GetCharacterMovement()->IsFalling();
			bIsCrouching = Character->GetCharacterMovement()->IsCrouching();
		}
	}
}

void UZCharacterAnimInstance::SetIsEquipWeapon(bool NewState)
{
	bIsEquipWeapon = NewState;
}

bool UZCharacterAnimInstance::IsEquipWeapon() const
{
	return bIsEquipWeapon;
}
