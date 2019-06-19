// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGrenade.h"
#include "ZGrenadeProjectile.h"
#include "ZCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "ConstructorHelpers.h"

AZGrenade::AZGrenade()
{
	// Set skeletal mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_WEAPON(TEXT("SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/G67_Grenade/SK_G67.SK_G67'"));
	if (SK_WEAPON.Succeeded())
	{
		WeaponMesh->SetSkeletalMesh(SK_WEAPON.Object);
	}

	static ConstructorHelpers::FClassFinder<AZProjectile>
		PROJECTILE(TEXT("Class'/Script/ProjectZ_422.ZGrenadeProjectile'"));
	if (PROJECTILE.Succeeded())
	{
		ProjectileClass = PROJECTILE.Class;
	}

	ExplosionRadius = 100.f;
	WeaponCategory = EWeaponCategory::Grenade;

	// For test
	MaxQuantityOfItem = 999;

	bIsThrown = false;
}

void AZGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZGrenade::InitItemData(const FZItemData * const NewItemData)
{
	Super::InitItemData(NewItemData);

	auto WeaponData = static_cast<const FZWeaponData*>(NewItemData);
	ExplosionRadius = WeaponData->ExplosionRadius;
}

void AZGrenade::Fire()
{
	if (!IsEquipped())
	{
		return;
	}

	if (IsThrown())
	{
		return;
	}

	SetIsThrown(true);

	Super::Fire();
}

void AZGrenade::ThrowGrenade()
{
	FVector HandLocation = ItemOwner->GetMesh()->GetSocketLocation(AZCharacter::MainWeaponSocketName);
	FVector LaunchDirection = FVector::ZeroVector;

	FHitResult Hit = WeaponTrace(10000.f);
	if (Hit.bBlockingHit)
	{
		LaunchDirection = Hit.ImpactPoint - HandLocation;
	}
	else
	{
		LaunchDirection = Hit.TraceEnd - HandLocation;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = ItemOwner;

	AZGrenadeProjectile* Projectile = GetWorld()->SpawnActor<AZGrenadeProjectile>(ProjectileClass, HandLocation, LaunchDirection.Rotation(), SpawnParams);
	if (Projectile)
	{
		Projectile->SetDamage(Damage);
		Projectile->SetFireDelay(FireDelay);
		Projectile->FireInDirection(LaunchDirection.GetSafeNormal());
	}
	
	SetIsThrown(false);

	Super::OnUsed();
}

void AZGrenade::SetIsThrown(bool NewState)
{
	bIsThrown = NewState;
}

bool AZGrenade::IsThrown() const
{
	return bIsThrown;
}
