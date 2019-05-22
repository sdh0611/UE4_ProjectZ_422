// Fill out your copyright notice in the Description page of Project Settings.


#include "ZCharacterAnimInstance.h"
#include "ZCharacter.h"
#include "ZWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ConstructorHelpers.h"

UZCharacterAnimInstance::UZCharacterAnimInstance()
{
	CurrentPawnSpeed = 0.f;
	CurrentPawnDirection = 0.f;
	AimYaw = 0.f;
	AimPitch = 0.f;
	bIsFalling = false;
	bIsCrouching = false;
	bIsEquipWeapon = false;
	bIsAiming = false;
	bIsSprinting = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		MONTAGE_FIRE_RIFLE_HIP(TEXT("AnimMontage'/Game/Animation/AnimStarterPack/Animations/Montage/Fire_Rifle_Hip_Montage.Fire_Rifle_Hip_Montage'"));
	if (MONTAGE_FIRE_RIFLE_HIP.Succeeded())
	{
		MontageTable.Add(TEXT("FireRifleHip"), MONTAGE_FIRE_RIFLE_HIP.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		MONTAGE_FIRE_RIFLE_AIM(TEXT("AnimMontage'/Game/Animation/AnimStarterPack/Animations/Montage/Fire_Rifle_Ironsights_Montage.Fire_Rifle_Ironsights_Montage'"));
	if (MONTAGE_FIRE_RIFLE_AIM.Succeeded())
	{
		MontageTable.Add(TEXT("FireRifleAim"), MONTAGE_FIRE_RIFLE_AIM.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		MONTAGE_RELOAD_RIFLE(TEXT("AnimMontage'/Game/Animation/AnimStarterPack/Animations/Montage/Reload_Rifle_Hip_Montage.Reload_Rifle_Hip_Montage'"));
	if (MONTAGE_RELOAD_RIFLE.Succeeded())
	{
		MontageTable.Add(TEXT("ReloadRifle"), MONTAGE_RELOAD_RIFLE.Object);
	}



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
			AimOffset에 사용하기 위한 Yaw, Pitch 계산
		*/
		FRotator DeltaRot = Pawn->GetControlRotation() - Pawn->GetActorRotation();
		FRotator CurrentRot = FMath::RInterpTo(FRotator(AimPitch, AimYaw, 0.f), DeltaRot, DeltaSeconds, 15.f);
		AimYaw = FMath::ClampAngle(CurrentRot.Yaw, -90.f, 90.f);
		AimPitch = FMath::ClampAngle(CurrentRot.Pitch, -90.f, 90.f);

		/*
			Character의 현재 상태에 대해서 갱신하는 부분.
		*/
		auto Character = Cast<AZCharacter>(Pawn);
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
		return;
	}
	MontageTable[MontageName];

	Montage_Play(MontageTable[MontageName]);
}

void UZCharacterAnimInstance::PlayFireMontage()
{
	if (IsAiming())
	{
		PlayMontage(TEXT("FireRifleAim"));
	}
	else
	{
		PlayMontage(TEXT("FireRifleHip"));
	}
}

void UZCharacterAnimInstance::SetIsEquipWeapon(bool NewState)
{
	bIsEquipWeapon = NewState;
}

void UZCharacterAnimInstance::SetIsAiming(bool NewState)
{
	bIsAiming = NewState;
}

void UZCharacterAnimInstance::SetIsSprinting(bool NewState)
{
	bIsSprinting = NewState;
}

bool UZCharacterAnimInstance::IsEquipWeapon() const
{
	return bIsEquipWeapon;
}

bool UZCharacterAnimInstance::IsAiming() const
{
	return bIsAiming;
}

bool UZCharacterAnimInstance::IsSprinting() const
{
	return bIsSprinting;
}

void UZCharacterAnimInstance::AnimNotify_ReloadCheck()
{
	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		auto Player = Cast<AZCharacter>(Pawn);
		check(nullptr != Player);
		Player->GetCurrentWeapon()->Reload();

	}
}
