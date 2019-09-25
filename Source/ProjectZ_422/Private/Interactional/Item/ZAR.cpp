// Fill out your copyright notice in the Description page of Project Settings.


#include "ZAR.h"
#include "ZBulletProjectile.h"
#include "ZCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AZAR::AZAR()
{

}

void AZAR::Fire()
{
	if (IsReloading())
	{
		return;
	}

	if (CheckNeedToReload())
	{
		if (EmptySound)
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), EmptySound, GetActorLocation(), GetActorRotation());
		}

		if (IsWantsToFire())
		{
			SetWantsToFire(false);
		}

		return;
	}

	if (!IsWantsToFire())
	{
		if (EFireMode::AutoFire == GetFireMode())
		{
			SetWantsToFire(true);
		}
	}

	FVector MuzzleLocation = WeaponMesh->GetSocketLocation(TEXT("muzzle"));
	FVector LaunchDirection = FVector::ZeroVector;
	FHitResult Hit = WeaponTrace(TraceDistance, bToggleDebug);
	if (Hit.bBlockingHit)
	{
		LaunchDirection = Hit.ImpactPoint - MuzzleLocation;
	}
	else
	{
		LaunchDirection = Hit.TraceEnd - MuzzleLocation;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = ItemOwner;

	AZBulletProjectile* Projectile = GetWorld()->SpawnActor<AZBulletProjectile>(BulletClass, MuzzleLocation, LaunchDirection.Rotation(), SpawnParams);
	if (Projectile)
	{
		Projectile->SetDamage(Damage);
		Projectile->FireInDirection(LaunchDirection.GetSafeNormal());
		Projectile->SetBulletSpeed(BulletSpeed);
		Projectile->SetBulletLifeSpan(BulletLifeSpan);
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
			
			SpawnTrail(Hit.bBlockingHit ? Hit.ImpactPoint : Hit.TraceEnd);
			PlayCameraShake();
		}
		else
		{
			ZLOG(Warning, TEXT("Reload!"));
		}
	}

	//if (EFireMode::SingleShot == FireMode)
	//{
	//	FireEnd();
	//}

	Super::Fire();
}

void AZAR::ChangeFireMode()
{
	uint8 CurrentFireMode = static_cast<uint8>(FireMode);

	SetFireMode(static_cast<EFireMode>((++CurrentFireMode) % 2));
}
