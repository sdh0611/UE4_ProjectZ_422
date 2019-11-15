// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGrenade.h"
#include "ZGrenadeProjectile.h"
#include "ZCharacter.h"
#include "ZGameInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "ZCharacterItemStatusComponent.h"
#include "Kismet/GameplayStatics.h"

AZGrenade::AZGrenade()
{
	ExplosionRadius = 100.f;
	ExplosionDelay = 3.f;
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

	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	check(nullptr != MyGameInstance);

	auto NewGrenadeData = MyGameInstance->GetGrenadeDataByName(NewItemData->ItemName);
	if (nullptr == NewGrenadeData)
	{
		ZLOG(Error, TEXT("Invalid grenade data."));
		return;
	}

	ExplosionRadius = NewGrenadeData->ExplosionRadius;
	ExplosionDelay = NewGrenadeData->ExplosionDelay;

}

void AZGrenade::OnUsed()
{
	if (::IsValid(ItemOwner))
	{
		auto ItemStatusComponent = ItemOwner->GetItemStatusComponent();
		if (::IsValid(ItemStatusComponent))
		{
			ItemStatusComponent->EquipWeapon(this);
		}
	}

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

	if (FireSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, GetActorLocation(), GetActorRotation());
	}

	Super::Fire();
}

bool AZGrenade::ServerThrowGrenade_Validate()
{
	return true;
}

void AZGrenade::ServerThrowGrenade_Implementation()
{
	ZLOG_S(Error);
	FVector HandLocation = ItemOwner->GetMesh()->GetSocketLocation(ItemOwner->GetMainWeaponSocketName());
	FVector LaunchDirection = FVector::ZeroVector;

	FHitResult Hit = WeaponTrace(TraceDistance, bToggleDebug);
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

	AZGrenadeProjectile* Projectile = GetWorld()->SpawnActor<AZGrenadeProjectile>(GrenadeProjectileClass, HandLocation, LaunchDirection.Rotation(), SpawnParams);
	if (Projectile)
	{
		Projectile->SetDamage(Damage);
		Projectile->SetFireDelay(ExplosionDelay);
		Projectile->FireInDirection(LaunchDirection.GetSafeNormal());
		Projectile->SetDamageCauser(ItemOwner->GetController());
	}

	SetIsThrown(false);

	AdjustQuantity(-1);
}

void AZGrenade::ThrowGrenade()
{
	//ZLOG_S(Error);
	//if (::IsValid(ItemOwner))
	//{
	//	ZLOG_S(Error);
	//	auto PC = ItemOwner->GetController<APlayerController>();
	//	if (PC && PC->IsLocalPlayerController())
	//	{
	//		ZLOG_S(Error);
	//		ServerThrowGrenade();
	//	}
	//}

	if (!HasAuthority())
	{
		return;
	}

	ZLOG_S(Error);
	FVector HandLocation = ItemOwner->GetMesh()->GetSocketLocation(ItemOwner->GetMainWeaponSocketName());
	FVector LaunchDirection = FVector::ZeroVector;

	FHitResult Hit = WeaponTrace(TraceDistance, bToggleDebug);
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

	AZGrenadeProjectile* Projectile = GetWorld()->SpawnActor<AZGrenadeProjectile>(GrenadeProjectileClass, HandLocation, LaunchDirection.Rotation(), SpawnParams);
	if (Projectile)
	{
		Projectile->SetDamage(Damage);
		Projectile->SetFireDelay(ExplosionDelay);
		Projectile->FireInDirection(LaunchDirection.GetSafeNormal());
		Projectile->SetDamageCauser(ItemOwner->GetController());
	}

	SetIsThrown(false);

	AdjustQuantity(-1);
}

void AZGrenade::SetIsThrown(bool NewState)
{
	bIsThrown = NewState;
}

bool AZGrenade::IsThrown() const
{
	return bIsThrown;
}
