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

	FHitResult Hit = WeaponTrace(TraceDistance, bToggleDebug);
	
	FVector LaunchDirection = FVector::ZeroVector;
	//FVector MuzzleLocation = WeaponMesh->GetSocketLocation(TEXT("muzzle"));
	FVector MuzzleLocation = Hit.TraceStart;
	
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

			MulticastSpawnFireEffectAndSound();

			FVector EndPoint = Hit.bBlockingHit ? Hit.ImpactPoint : Hit.TraceEnd;
			MulticastSpawnTrail(EndPoint);

			PlayCameraShake();
		}
		else
		{
			ZLOG(Warning, TEXT("Reload!"));
		}
	}

	Super::Fire();
}

void AZAR::ChangeFireMode()
{
	uint8 CurrentFireMode = static_cast<uint8>(FireMode);

	SetFireMode(static_cast<EFireMode>((++CurrentFireMode) % 2));
}
