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
	bIsDead = false;

	CurrentPlayMontage = nullptr;
}

void UZCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		/*
			Character의 현재 속도 및 방향을 구하는 부분.
		*/
		FVector Speed = Pawn->GetVelocity();
		Speed.Z = 0.f;
		CurrentPawnSpeed = Speed.Size();
		CurrentPawnDirection = CalculateDirection(Pawn->GetVelocity(), Pawn->GetActorRotation());

		/*
			Character의 현재 상태에 대해서 갱신하는 부분.
		*/
		auto Character = Cast<AZBaseCharacter>(Pawn);
		if (Character)
		{
			bIsFalling = Character->GetCharacterMovement()->IsFalling();
			bIsCrouching = Character->GetCharacterMovement()->IsCrouching();
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

void UZCharacterAnimInstance::StopCurrentMontage()
{
	if (!Montage_IsPlaying(CurrentPlayMontage))
	{
		return;
	}

	Montage_Stop(CurrentPlayMontage->BlendOut.GetBlendTime(), CurrentPlayMontage);
}

void UZCharacterAnimInstance::ResumeCurrentMontage()
{
	if (Montage_IsPlaying(CurrentPlayMontage))
	{
		return;
	}

	Montage_Resume(CurrentPlayMontage);
}

void UZCharacterAnimInstance::SetIsSprinting(bool NewState)
{
	bIsSprinting = NewState;
}

void UZCharacterAnimInstance::SetIsDead(bool NewState)
{
	bIsDead = NewState;
}

bool UZCharacterAnimInstance::IsSprinting() const
{
	return bIsSprinting;
}

bool UZCharacterAnimInstance::IsMontagePlaying(const FString& MontageName) const
{
	auto Montage = FindMontageByName(MontageName);
	if (nullptr == Montage)
	{
		return false;
	}

	return Montage_IsPlaying(Montage);
}

UAnimMontage * const UZCharacterAnimInstance::FindMontageByName(const FString & MontageName) const
{
	if (!MontageTable.Contains(MontageName))
	{
		return nullptr;
	}

	return MontageTable[MontageName];
}

UAnimMontage * const UZCharacterAnimInstance::GetCurrentPlayMontage() const
{
	return CurrentPlayMontage;
}
