// Fill out your copyright notice in the Description page of Project Settings.


#include "ZCharacterAnimInstance.h"
#include "ZBaseCharacter.h"
#include "ZWeapon.h"
#include "ZGun.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ConstructorHelpers.h"

UZCharacterAnimInstance::UZCharacterAnimInstance()
{
	CurrentPawnSpeed = 0.f;
	CurrentPawnDirection = 0.f;

	bIsFalling = false;
	bIsCrouching = false;
	bIsSprinting = false;
	   
	CurrentPlayMontage = nullptr;
}

void UZCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		/*
			Character�� ���� �ӵ� �� ������ ���ϴ� �κ�.
		*/
		FVector Speed = Pawn->GetVelocity();
		Speed.Z = 0.f;
		CurrentPawnSpeed = Speed.Size();
		CurrentPawnDirection = CalculateDirection(Pawn->GetVelocity(), Pawn->GetActorRotation());

		/*
			Character�� ���� ���¿� ���ؼ� �����ϴ� �κ�.
		*/
		auto Character = Cast<AZBaseCharacter>(Pawn);
		if (Character)
		{
			bIsFalling = Character->GetCharacterMovement()->IsFalling();
			bIsCrouching = Character->GetCharacterMovement()->IsCrouching();
			//if (nullptr == Character->GetCurrentWeapon())
			//{
			//	if (CurrentPlayMontage)
			//	{
			//		if (!Montage_IsPlaying(CurrentPlayMontage))
			//		{
			//			SetIsEquipWeapon(false);
			//		}
			//	}
			//}

		}

	}
}

void UZCharacterAnimInstance::PlayMontage(const FString & MontageName)
{
	if (!MontageTable.Contains(MontageName))
	{
		ZLOG(Error, TEXT("No montage %s"), *MontageName);
		return;
	}
	ZLOG(Error, TEXT("Play Montage : %s"), *MontageName);
	CurrentPlayMontage = MontageTable[MontageName];
	
	Montage_Play(MontageTable[MontageName]);
}

void UZCharacterAnimInstance::SetIsSprinting(bool NewState)
{
	bIsSprinting = NewState;
}

bool UZCharacterAnimInstance::IsSprinting() const
{
	return bIsSprinting;
}
