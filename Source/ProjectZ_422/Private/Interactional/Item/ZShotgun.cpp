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

	BulletLifeSpan = 0.1f;

	for (int32 i = 0; i < ShotNumber; ++i)
	{
		DirList.Add(FVector::ZeroVector);
	}
}

void AZShotgun::Fire()
{
	if (bIsFiring)
	{
		return;
	}

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

	SetWantsToFire(false);

	FVector MuzzleLocation = WeaponMesh->GetSocketLocation(TEXT("muzzle"));
	FVector LaunchDirection = FVector::ZeroVector;
	FVector EndPoint = FVector::ZeroVector;

	FHitResult Hit = WeaponTrace(TraceDistance, bToggleDebug);
	EndPoint = (Hit.bBlockingHit ? Hit.ImpactPoint : Hit.TraceEnd);
	//EndPoint = Hit.TraceEnd;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = ItemOwner;
	
	/* 모든 산탄이 정상적으로 생성되었는지 체크함. */
	bool bSuccess = true;

	/* Shot spread logic부분. */
	float RandomShotRangeY = 0.f;
	float RandomShotRangeZ = 0.f;

	/* 거리에 비례해서 펴져야하기 떄문에 HitDistance에 임의 상수를 곱해서 ShotRange에 더해줌. */
	float NewShotRange = ShotRange + Hit.Distance * 0.02f;

	for (const auto& Dir : DirList)
	{
		RandomShotRangeY = FMath::RandRange(-NewShotRange, NewShotRange);
		RandomShotRangeZ = FMath::RandRange(-NewShotRange, NewShotRange);
		
		LaunchDirection = (EndPoint + Dir + FVector(0.f, RandomShotRangeY, RandomShotRangeZ)) - MuzzleLocation;

		AZBulletProjectile* Projectile = GetWorld()->SpawnActor<AZBulletProjectile>(BulletClass, MuzzleLocation, LaunchDirection.Rotation(), SpawnParams);
		if (nullptr == Projectile)
		{
			/* 산탄 생성 실패이므로 중단. */
			ZLOG(Error, TEXT("Failed to spawn bullet.."));
			bSuccess = false;
			break;
		}

		Projectile->SetDamage(Damage / ShotNumber);
		Projectile->FireInDirection(LaunchDirection.GetSafeNormal());
		Projectile->SetBulletSpeed(BulletSpeed);
		Projectile->SetBulletLifeSpan(BulletLifeSpan);
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

	Super::Fire();
}
