// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGun.h"
#include "ZCharacter.h"
#include "ZCharacterItemStatusComponent.h"
#include "ZPlayerController.h"
#include "ZBulletProjectile.h"
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

	FireMode = EFireMode::SingleShot;

	WeaponCategory = EWeaponCategory::Gun;

	EffectAttachSocketName = TEXT("muzzle");

	MontageTable.Add(TEXT("FireAim"), nullptr);
}

void AZGun::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (BulletClass)
	//{
	//	if (IsWantsToFire())
	//	{
	//		FireTimer += DeltaTime;
	//		if (FireTimer >= FireDelay)
	//		{
	//			Fire();
	//			FireTimer = 0.f;
	//		}
	//	}
	//	else
	//	{
	//		if (FireTimer != 0.f)
	//		{
	//			FireTimer = 0.f;
	//		}
	//	}
	//}


	/* 연사모드가 있는 총기류를 제외한 나머지 총기들의 기본 shot 동작 */
	if (BulletClass)
	{
		if (bIsFiring)
		{
			FireTimer += DeltaTime;
			if (FireTimer >= FireDelay)
			{
				bIsFiring = false;
				FireTimer = 0.f;
		
				if (EFireMode::AutoFire == GetFireMode())
				{
					if (IsWantsToFire())
					{
						Fire();
					}
				}

			}

		}

	}



}

void AZGun::InitItemData(const FZItemData * const NewItemData)
{
	Super::InitItemData(NewItemData);

	auto NewGunData = static_cast<const FZGunData*>(NewItemData);

	FireDelay = NewGunData->FireDelay;
	MaxAmmo = NewGunData->MaxAmmo;
	CurrentAmmo = MaxAmmo;
	UseAmmoName = NewGunData->UseAmmoName;
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

UAnimMontage * const AZGun::GetAnimMontage() const
{
	if(ItemOwner->IsAiming())
	{
		return FindMontage(TEXT("FireAim"));
	}

	return Super::GetAnimMontage();
}

void AZGun::Fire()
{
	//if (IsReloading())
	//{
	//	return;
	//}

	//if (CheckNeedToReload())
	//{
	//	if (EmptySound)
	//	{
	//		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), EmptySound, GetActorLocation(), GetActorRotation());
	//	}

	//	if (bWantsToFire)
	//	{
	//		SetWantsToFire(false);
	//	}

	//	return;
	//}

	//if (!IsWantsToFire())
	//{
	//	SetWantsToFire(true);
	//}

	//FVector MuzzleLocation = WeaponMesh->GetSocketLocation(TEXT("muzzle"));
	//FVector LaunchDirection = FVector::ZeroVector;

	//FHitResult Hit = WeaponTrace(100000.f);
	//if (Hit.bBlockingHit)
	//{
	//	LaunchDirection = Hit.ImpactPoint - MuzzleLocation;
	//}
	//else
	//{
	//	LaunchDirection = Hit.TraceEnd - MuzzleLocation;
	//}

	////DrawDebugLine(GetWorld(), MuzzleLocation, LaunchDirection * 100000.f, FColor::Red, false, 0.5f);

	//FActorSpawnParameters SpawnParams;
	//SpawnParams.Owner = this;
	//SpawnParams.Instigator = ItemOwner;

	//AZBulletProjectile* Projectile = GetWorld()->SpawnActor<AZBulletProjectile>(BulletClass, MuzzleLocation, LaunchDirection.Rotation(), SpawnParams);
	//if (Projectile)
	//{
	//	Projectile->SetDamage(Damage);
	//	Projectile->FireInDirection(LaunchDirection.GetSafeNormal());
	//	if (CurrentAmmo > 0)
	//	{
	//		--CurrentAmmo;
	//		if (FireEffect)
	//		{
	//			UGameplayStatics::SpawnEmitterAttached(FireEffect, WeaponMesh, EffectAttachSocketName);
	//		}
	//		
	//		if (FireSound)
	//		{
	//			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, GetActorLocation(), GetActorRotation());
	//		}

	//	}
	//	else
	//	{
	//		ZLOG(Warning, TEXT("Reload!"));
	//	}
	//}


	bIsFiring = true;

	//if (EFireMode::SingleShot == FireMode)
	//{
	//	FireEnd();
	//}



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
