// Fill out your copyright notice in the Description page of Project Settings.


#include "ZPlayerAnimInstance.h"
#include "ZCharacter.h"
#include "ZWeapon.h"
#include "ZGun.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ConstructorHelpers.h"
#include "Animation/AnimMontage.h"

UZPlayerAnimInstance::UZPlayerAnimInstance()
{
	AimYaw = 0.f;
	AimPitch = 0.f;
	bIsEquipGun = false;
	bIsAiming = false;
	bIsDead = false;

}

void UZPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = Cast<AZCharacter>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		/*
			AimOffset에 사용하기 위한 Yaw, Pitch 계산
		*/
		FRotator DeltaRot = Pawn->GetControlRotation() - Pawn->GetActorRotation();
		FRotator CurrentRot = FMath::RInterpTo(FRotator(AimPitch, AimYaw, 0.f), DeltaRot, DeltaSeconds, 15.f);
		AimYaw = FMath::ClampAngle(CurrentRot.Yaw, -90.f, 90.f);
		AimPitch = FMath::ClampAngle(CurrentRot.Pitch, -90.f, 90.f);

		/* 
			무기 장착여부 체크
		*/
		auto Weapon = Pawn->GetCurrentWeapon();
		if (nullptr == Weapon)
		{
			SetIsEquipGun(false);
		}
		else
		{
			BindFireMontage(Weapon);
		}
		
		

	}
}

void UZPlayerAnimInstance::PlayFireRifleMontage()
{
	if (IsAiming())
	{
		PlayCharacterMontage(TEXT("FireRifleAim"));
	}
	else
	{
		PlayCharacterMontage(TEXT("FireRifleHip"));
	}
}

void UZPlayerAnimInstance::PlayThrowGrenadeMontage()
{
	/*
		해당 애니메이션은 Anim Blueprint에서 추가해줬음.
	*/
	ZLOG_S(Error);
	PlayCharacterMontage(TEXT("ThrowGrenade"));
}

void UZPlayerAnimInstance::BindFireMontage(AZWeapon * NewWeapon)
{
	if (NewWeapon->OnWeaponFired.IsBoundToObject(this))
	{
		return;
	}


	switch (NewWeapon->GetWeaponCategory())
	{
		case EWeaponCategory::Knife:
		{
			/*
				근접무기
				->임시로 라이플 애니메이션으로 바인딩
			*/
			NewWeapon->OnWeaponFired.AddUObject(this, &UZPlayerAnimInstance::PlayFireRifleMontage);
			SetIsEquipGun(false);
			break;
		}
		case EWeaponCategory::Grenade:
		{
			/*
				투척물
			*/
			NewWeapon->OnWeaponFired.AddUObject(this, &UZPlayerAnimInstance::PlayThrowGrenadeMontage);
			SetIsEquipGun(false);
			break;
		}
		case EWeaponCategory::Invalid:
		{
			ZLOG(Error, TEXT("Invalid type."));
			return;
		}
		default:
		{
			/*
				라이플류
			*/
			NewWeapon->OnWeaponFired.AddUObject(this, &UZPlayerAnimInstance::PlayFireRifleMontage);
			SetIsEquipGun(true);
			break;
		}

	}

}

void UZPlayerAnimInstance::PlayFireMontage(class AZWeapon* NewWeapon)
{
	switch (NewWeapon->GetWeaponCategory())
	{
		case EWeaponCategory::Gun:
		{
			PlayFireRifleMontage();
			break;
		}
		case EWeaponCategory::Knife:
		{
			
			break;
		}
		case EWeaponCategory::Grenade:
		{
			PlayThrowGrenadeMontage();
			break;
		}
		default:
		{
			break;
		}
	}

}

void UZPlayerAnimInstance::PlayHitMontage()
{
	auto HitMontage = FindMontageByName(TEXT("Hit"));
	if (nullptr == HitMontage)
	{
		ZLOG(Error, TEXT("Hit montage not exist in montage table."));
		return;
	}

	int32 Index = FMath::RandRange(1, 4);
	FString SectionName = FString::Printf(TEXT("Hit_%d"), Index);
	Montage_JumpToSection(*SectionName, HitMontage);
	Montage_Play(HitMontage);
}

void UZPlayerAnimInstance::SetIsEquipGun(bool NewState)
{
	bIsEquipGun = NewState;
}

void UZPlayerAnimInstance::SetIsAiming(bool NewState)
{
	bIsAiming = NewState;
}

bool UZPlayerAnimInstance::IsEquipGun() const
{
	return bIsEquipGun;
}

bool UZPlayerAnimInstance::IsAiming() const
{
	return bIsAiming;
}

void UZPlayerAnimInstance::AnimNotify_ReloadCheck()
{
	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		auto Player = Cast<AZCharacter>(Pawn);
		check(nullptr != Player);
		Cast<AZGun>(Player->GetCurrentWeapon())->Reload();

	}
}

void UZPlayerAnimInstance::AnimNotify_WeaponSwitchCheck()
{
}

void UZPlayerAnimInstance::AnimNotify_SwitchEndCheck()
{
	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		Cast<AZCharacter>(Pawn)->SetIsSwitchingWeapon(false);
	}
}

void UZPlayerAnimInstance::AnimNotify_ThrowCheck()
{
	if (OnGrenadeThrow.IsBound())
	{
		OnGrenadeThrow.Execute();
	}
}
