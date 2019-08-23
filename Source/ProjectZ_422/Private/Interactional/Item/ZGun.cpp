// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGun.h"
#include "ZCharacter.h"
#include "ZPlayerController.h"
#include "ZBulletProjectile.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

AZGun::AZGun()
{
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh>
	//	SK_WEAPON(TEXT("SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4'"));
	//if (SK_WEAPON.Succeeded())
	//{
	//	WeaponMesh->SetSkeletalMesh(SK_WEAPON.Object);
	//}

	//static ConstructorHelpers::FClassFinder<AZProjectile>
	//	CLASS_PROJECTILE(TEXT("Class'/Script/ProjectZ_422.ZBulletProjectile'"));
	//if (CLASS_PROJECTILE.Succeeded())
	//{
	//	ProjectileClass = CLASS_PROJECTILE.Class;
	//}

	bIsReloading = false;
	bWantsToFire = false;

	CurrentAmmo = 0;
	MaxAmmo = 30;

	FireTimer = 0.f;
	// Code for test
	FireDelay = 0.15f;

	WeaponCategory = EWeaponCategory::Gun;
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
		if (IsWantsToFire())
		{
			FireTimer += DeltaTime;
			if (FireTimer >= FireDelay)
			{
				Fire();
				FireTimer = 0.f;
			}
		}
		else
		{
			if (FireTimer != 0.f)
			{
				FireTimer = 0.f;
			}
		}
	}

}

void AZGun::InitItemData(const FZItemData * const NewItemData)
{
	Super::InitItemData(NewItemData);

	auto NewWeaponData = static_cast<const FZWeaponData*>(NewItemData);

	//FireDelay = NewWeaponData->FireDelay;
	MaxAmmo = NewWeaponData->MaxAmmo;
}

void AZGun::Reload()
{
	SetIsReloading(false);
	if (CurrentAmmo >= MaxAmmo)
	{
		return;
	}
	ZLOG_S(Warning);

	CurrentAmmo = MaxAmmo;
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

void AZGun::Fire()
{
	if (!IsWantsToFire())
	{
		SetWantsToFire(true);
	}

	if (IsReloading())
	{
		return;
	}

	if (CheckNeedToReload())
	{
		return;
	}

	FVector MuzzleLocation = WeaponMesh->GetSocketLocation(TEXT("muzzle"));
	FVector LaunchDirection = FVector::ZeroVector;

	FHitResult Hit = WeaponTrace(10000.f);
	if (Hit.bBlockingHit)
	{
		LaunchDirection = Hit.ImpactPoint - MuzzleLocation;
	}
	else
	{
		LaunchDirection = Hit.TraceEnd - MuzzleLocation;
	}

	//DrawDebugLine(GetWorld(), MuzzleLocation, LaunchDirection * 100000.f, FColor::Red, false, 0.5f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = ItemOwner;

	AZBulletProjectile* Projectile = GetWorld()->SpawnActor<AZBulletProjectile>(BulletClass, MuzzleLocation, LaunchDirection.Rotation(), SpawnParams);
	if (Projectile)
	{
		Projectile->SetDamage(Damage);
		Projectile->FireInDirection(LaunchDirection.GetSafeNormal());
		if (CurrentAmmo > 0)
		{
			--CurrentAmmo;
			if (FireEffect)
			{
				UGameplayStatics::SpawnEmitterAttached(FireEffect, WeaponMesh, EffectAttachSocketName);
			}
			
			if (FireSound)
			{
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, GetActorLocation(), GetActorRotation());
			}

		}
		else
		{
			ZLOG(Warning, TEXT("Reload!"));
		}
	}

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
