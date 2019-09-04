// Fill out your copyright notice in the Description page of Project Settings.


#include "ZShotgun.h"
#include "ZBulletProjectile.h"
#include "ZCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


AZShotgun::AZShotgun()
{
	ShotNumber = 9;

	ShotRange = 4.f;

	for (int32 i = 0; i < ShotNumber; ++i)
	{
		DirList.Add(FVector::ZeroVector);
	}
}

void AZShotgun::Fire()
{
	if (CheckNeedToReload())
	{
		if (EmptySound)
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), EmptySound, GetActorLocation(), GetActorRotation());
		}

		if (bWantsToFire)
		{
			SetWantsToFire(false);
		}

		return;
	}

	if (!IsWantsToFire())
	{
		SetWantsToFire(true);
	}

	FVector MuzzleLocation = WeaponMesh->GetSocketLocation(TEXT("muzzle"));
	FVector LaunchDirection = FVector::ZeroVector;
	FVector EndPoint = FVector::ZeroVector;

	FHitResult Hit = WeaponTrace(100000.f);
	if (Hit.bBlockingHit)
	{
		EndPoint = Hit.ImpactPoint;
		LaunchDirection = Hit.ImpactPoint - MuzzleLocation;
	}
	else
	{
		EndPoint = Hit.TraceEnd;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = ItemOwner;

	/* ��� ��ź�� ���������� �����Ǿ����� üũ��. */
	bool bSuccess = true;

	/* Shot spread logic�κ�. */
	for (const auto& Dir : DirList)
	{
		float RandomShotRange = FMath::RandRange(-ShotRange, ShotRange);

		LaunchDirection = (EndPoint + Dir + RandomShotRange) - MuzzleLocation;

		AZBulletProjectile* Projectile = GetWorld()->SpawnActor<AZBulletProjectile>(BulletClass, MuzzleLocation, LaunchDirection.Rotation(), SpawnParams);
		if (nullptr == Projectile)
		{
			/* ��ź ���� �����̹Ƿ� �ߴ�. */
			bSuccess = false;
			break;
		}

		Projectile->SetDamage(Damage / ShotNumber);
		Projectile->FireInDirection(LaunchDirection.GetSafeNormal());

	}

	if (bSuccess)
	{
		if (CurrentAmmo > 0)
		{
			--CurrentAmmo;
		}

		if (FireEffect)
		{
			UGameplayStatics::SpawnEmitterAttached(FireEffect, WeaponMesh, EffectAttachSocketName);
		}

		if (FireSound)
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, GetActorLocation(), GetActorRotation());
		}
	}

	if (EFireMode::SingleShot == FireMode)
	{
		FireEnd();
	}

	Super::Fire();
}
