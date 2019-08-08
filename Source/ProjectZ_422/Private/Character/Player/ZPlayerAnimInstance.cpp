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
		PlayMontage(TEXT("FireRifleAim"));
	}
	else
	{
		PlayMontage(TEXT("FireRifleHip"));
	}
}

void UZPlayerAnimInstance::PlayThrowGrenadeMontage()
{
	/*
		해당 애니메이션은 Anim Blueprint에서 추가해줬음.
	*/
	ZLOG_S(Error);
	PlayMontage(TEXT("ThrowGrenade"));
}

void UZPlayerAnimInstance::BindFireMontage(AZWeapon * NewWeapon)
{
	if (NewWeapon->OnWeaponFired.IsBoundToObject(this))
	{
		return;
	}


	switch (NewWeapon->GetWeaponType())
	{
		case EWeaponType::Pistol:
		{
			/*
				권총류
				->임시로 라이플 애니메이션으로 바인딩
			*/
			//NewWeapon->OnWeaponFired.AddUObject(this, &UZCharacterAnimInstance::PlayFireRifleMontage);
			SetIsEquipGun(true);
			break;
		}
		case EWeaponType::Knife:
		{
			/*
				근접무기
				->임시로 라이플 애니메이션으로 바인딩
			*/
			NewWeapon->OnWeaponFired.AddUObject(this, &UZPlayerAnimInstance::PlayFireRifleMontage);
			SetIsEquipGun(false);
			break;
		}
		case EWeaponType::Grenade:
		{
			/*
				투척물
			*/
			NewWeapon->OnWeaponFired.AddUObject(this, &UZPlayerAnimInstance::PlayThrowGrenadeMontage);
			SetIsEquipGun(false);
			break;
		}
		case EWeaponType::Invalid:
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
