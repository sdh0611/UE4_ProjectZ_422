// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGun.h"
#include "ZCharacter.h"
#include "ZCharacterItemStatusComponent.h"
#include "ZPlayerController.h"
#include "ZBulletProjectile.h"
#include "ZGameInstance.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

AZGun::AZGun()
{
	bIsReloading = false;
	bWantsToFire = false;
	bIsFiring = false;

	MaxAmmo = 30;
	CurrentAmmo = 0.f;
	FireTimer = 0.f;
	// Code for test
	FireDelay = 0.15f;
	BulletSpeed = 10000.f;
	BulletLifeSpan = 3.f;

	MaxSpread = 0.2f;
	SpreadIncrement = 0.2f;

	SpreadDecrement = 0.005f;

	FireMode = EFireMode::SingleShot;

	WeaponCategory = EWeaponCategory::Gun;

	EffectAttachSocketName = TEXT("muzzle");

	MontageTable.Add(TEXT("FireAim"), nullptr);
	MontageTable.Add(TEXT("Reload"), nullptr);
}

void AZGun::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BulletClass)
	{
		if (bIsFiring)
		{
			FireTimer += DeltaTime;
			if (FireTimer >= FireDelay)
			{
				bIsFiring = false;
				FireTimer = 0.f;
		
				if (EFireMode::SingleShot != GetFireMode())
				{
					if (IsWantsToFire())
					{
						Fire();
					}
				}

			}

		}

	}

	if (!IsWantsToFire())
	{
		if (CurrentSpread > 0.f)
		{
			CurrentSpread -= (SpreadDecrement);
			if (CurrentSpread < 0.f)
			{
				CurrentSpread = 0.f;
			}
		}
	}

}

void AZGun::InitItemData(const FZItemData * const NewItemData)
{
	Super::InitItemData(NewItemData);

	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	check(nullptr != MyGameInstance);

	auto NewGunData = MyGameInstance->GetGunDataByName(NewItemData->ItemName);
	if (nullptr == NewGunData)
	{
		ZLOG(Error, TEXT("Invalid gun data."));
		return;
	}

	FireDelay = NewGunData->FireDelay;
	MaxAmmo = NewGunData->MaxAmmo;
	CurrentAmmo = MaxAmmo;
	UseAmmoName = NewGunData->UseAmmoName;
	BulletSpeed = NewGunData->BulletSpeed;
	GunType = GetGunTypeFromString(NewGunData->GunType);

}

void AZGun::Reload()
{
	SetIsReloading(false);
	if (CurrentAmmo >= MaxAmmo)
	{
		return;
	}
	ZLOG_S(Warning);

	while (IsCanReload())
	{
		auto Ammo = ItemOwner->GetItemStatusComponent()->GetItemByName(UseAmmoName);
		if (nullptr == Ammo)
		{
			ZLOG(Error, TEXT("Ammo not exist."));
			break;
		}

		int32 ReloadAmmo = MaxAmmo - CurrentAmmo;
		if (Ammo->GetCurrentQuantityOfItem() < ReloadAmmo)
		{
			ReloadAmmo = Ammo->GetCurrentQuantityOfItem();
		}

		Ammo->AdjustQuantity(-ReloadAmmo);

		CurrentAmmo += ReloadAmmo;
	}

	OnItemInfoChanged.Broadcast();
}

void AZGun::SetIsReloading(bool NewState)
{
	bIsReloading = NewState;
}

void AZGun::SetCurrentAmmo(int32 NewAmmo)
{
	CurrentAmmo = FMath::Clamp<int32>(NewAmmo, 0, NewAmmo);

}

void AZGun::SetMaxAmmo(int32 NewAmmo)
{
	MaxAmmo = FMath::Clamp<int32>(NewAmmo, 0, NewAmmo);
}

void AZGun::SetWantsToFire(bool NewState)
{
	bWantsToFire = NewState;
}

void AZGun::SetFireMode(EFireMode NewMode)
{
	FireMode = NewMode;
	OnItemInfoChanged.Broadcast();
}

bool AZGun::IsCanReload() const
{
	return CurrentAmmo < MaxAmmo;
}

bool AZGun::IsReloading() const
{
	return bIsReloading;
}

bool AZGun::IsWantsToFire() const
{
	return bWantsToFire;
}

int32 AZGun::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

int32 AZGun::GetMaxAmmo() const
{
	return MaxAmmo;
}

const FString & AZGun::GetUseAmmoName() const
{
	return UseAmmoName;
}

EGunType AZGun::GetGunType() const
{
	return GunType;
}

EFireMode AZGun::GetFireMode() const
{
	return FireMode;
}

UAnimMontage * const AZGun::GetFireAnimMontage() const
{
	if(ItemOwner->IsAiming())
	{
		return FindMontage(TEXT("FireAim"));
	}

	return Super::GetFireAnimMontage();
}

void AZGun::Fire()
{
	bIsFiring = true;

	/* Recoil */
	CurrentSpread = FMath::Clamp<float>(CurrentSpread + SpreadIncrement, 0.f, MaxSpread);

	float SpreadYaw = FMath::RandRange(-CurrentSpread, CurrentSpread);

	ItemOwner->AddControllerPitchInput(-CurrentSpread);
	ItemOwner->AddControllerYawInput(SpreadYaw);
	
	Super::Fire();
}

void AZGun::FireEnd()
{
	SetWantsToFire(false);
}

bool AZGun::CheckNeedToReload()
{
	return CurrentAmmo < 1;
}
