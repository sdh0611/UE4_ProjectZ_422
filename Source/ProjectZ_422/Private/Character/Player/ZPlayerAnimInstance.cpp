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

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		MONTAGE_EQUIP_RIFLE(TEXT("AnimMontage'/Game/Animation/AnimStarterPack/Animations/Montage/Equip_Rifle_Standing_Montage.Equip_Rifle_Standing_Montage'"));
	if (MONTAGE_EQUIP_RIFLE.Succeeded())
	{
		MontageTable.Add(TEXT("EquipRifle"), MONTAGE_EQUIP_RIFLE.Object);
	}

	AimYaw = 0.f;
	AimPitch = 0.f;
	bIsEquipGun = false;
	bIsAiming = false;

}

void UZPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		/*
			AimOffset에 사용하기 위한 Yaw, Pitch 계산
		*/
		FRotator DeltaRot = Pawn->GetControlRotation() - Pawn->GetActorRotation();
		FRotator CurrentRot = FMath::RInterpTo(FRotator(AimPitch, AimYaw, 0.f), DeltaRot, DeltaSeconds, 15.f);
		AimYaw = FMath::ClampAngle(CurrentRot.Yaw, -90.f, 90.f);
		AimPitch = FMath::ClampAngle(CurrentRot.Pitch, -90.f, 90.f);

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
	switch (NewWeapon->GetWeaponType())
	{
	case EWeaponType::Pistol:
	{
		/*
			권총류
			->임시로 라이플 애니메이션으로 바인딩
		*/
		//NewWeapon->OnWeaponFired.AddUObject(this, &UZCharacterAnimInstance::PlayFireRifleMontage);
		break;
	}
	case EWeaponType::Knife:
	{
		/*
			근접무기
			->임시로 라이플 애니메이션으로 바인딩
		*/
		NewWeapon->OnWeaponFired.AddUObject(this, &UZPlayerAnimInstance::PlayFireRifleMontage);
		break;
	}
	case EWeaponType::Grenade:
	{
		/*
			투척물
		*/
		NewWeapon->OnWeaponFired.AddUObject(this, &UZPlayerAnimInstance::PlayThrowGrenadeMontage);
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
