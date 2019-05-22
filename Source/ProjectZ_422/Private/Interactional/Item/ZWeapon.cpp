// Fill out your copyright notice in the Description page of Project Settings.


#include "ZWeapon.h"
#include "ZCharacter.h"
#include "ZProjectile.h"
#include "ZPlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"


AZWeapon::AZWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemType = EItemType::Weapon;

	// Code to test
	ItemName = TEXT("Weapon");

	// Create  skeletal mesh component
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	
	// Set skeletal mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_WEAPON(TEXT("SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4'"));
	if (SK_WEAPON.Succeeded())
	{
		WeaponMesh->SetSkeletalMesh(SK_WEAPON.Object);
	}

	static ConstructorHelpers::FClassFinder<AZProjectile>
		PROJECTILE(TEXT("Class'/Script/ProjectZ_422.ZProjectile'"));
	if (PROJECTILE.Succeeded())
	{
		ProjectileClass = PROJECTILE.Class;
	}

	// -1 : Player에게 습득되지 않은 상태
	WeaponInventoryIndex = -1;
	bIsEquipped = false;
	bWantsToFire = false;
	bIsReloading = false;
	FireTimer = 0.f;
	// Code for test
	FireDelay = 0.15f;

	CurrentAmmo = 0;
	MaxAmmo = 30;

	MaxQuantityOfItem = 1;
}

void AZWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ProjectileClass)
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

void AZWeapon::OnRemoved()
{
	ZLOG_S(Warning);

	// Actor로부터 떼어냄.
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	//SetIsEquipped(false);
	//// WeaponInventory Index 초기화
	//SetWeaponInventoryIndex(-1);

	Super::OnRemoved();
}

void AZWeapon::OnDropped()
{
	ZLOG_S(Warning);
	// Actor로부터 떼어냄.
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	//// WeaponInventory Index 초기화
	//SetWeaponInventoryIndex(-1);

	Super::OnDropped();
}

void AZWeapon::Reload()
{
	SetIsReloading(false);
	if (CurrentAmmo >= MaxAmmo)
	{
		return;
	}
	ZLOG_S(Warning);

	CurrentAmmo = MaxAmmo;
}

bool AZWeapon::IsCanReload() const
{
	return CurrentAmmo < MaxAmmo;
}



void AZWeapon::SetWeaponInventoryIndex(int32 NewIndex)
{
	//	Weaponinventory 범위를 넘어선 경우.
	if (NewIndex < -1 || NewIndex > 5)
	{
		ZLOG(Error, TEXT("Invalid value."));
		return;
	}

	WeaponInventoryIndex = NewIndex;
}

void AZWeapon::SetIsEquipped(bool NewState)
{
	if (!NewState)
	{
		SetWantsToFire(false);
	}

	bIsEquipped = NewState;
}

void AZWeapon::SetWantsToFire(bool NewState)
{
	if (NewState)
	{
		Fire();
	}

	bWantsToFire = NewState;
}

void AZWeapon::SetIsReloading(bool NewState)
{
	bIsReloading = NewState;
}

int32 AZWeapon::GetWeaponInventoryIndex() const
{
	return WeaponInventoryIndex;
}

bool AZWeapon::IsEquipped() const
{
	return bIsEquipped;
}

bool AZWeapon::IsWantsToFire() const
{
	return bWantsToFire;
}

bool AZWeapon::IsReloading() const
{
	return bIsReloading;
}

void AZWeapon::Fire()
{
	if (IsReloading())
	{
		return;
	}

	if (CheckNeedToReload())
	{
		return;
	}

	ZLOG(Warning, TEXT("Weapon Fire!!"));

	FVector MuzzleLocation = WeaponMesh->GetSocketLocation(TEXT("muzzle"));
	FVector LaunchDirection = FVector::ZeroVector;

	FVector CamLoc;
	FRotator CamRot;
	auto Controller = ItemOwner->GetController();
	if (Controller)
	{
		Controller->GetPlayerViewPoint(CamLoc, CamRot);
	}

	FVector StartLoc = CamLoc;
	FVector EndLoc = StartLoc + 10000.f * CamRot.Vector();

	FCollisionQueryParams TraceParams(TEXT("WeaponTrace"), true, this);
	TraceParams.bReturnPhysicalMaterial = false;

	FHitResult Hit;
	GetWorld()->LineTraceSingleByProfile(Hit, StartLoc, EndLoc, TEXT("WeaponTrace"), TraceParams);
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

	AZProjectile* Projectile = GetWorld()->SpawnActor<AZProjectile>(ProjectileClass, MuzzleLocation, LaunchDirection.Rotation(), SpawnParams);
	if (Projectile)
	{
		Projectile->FireInDirection(LaunchDirection.GetSafeNormal());
		if (CurrentAmmo > 0)
		{
			--CurrentAmmo;
		}
		else
		{
			ZLOG(Warning, TEXT("Reload!"));
		}
	}

	OnWeaponFired.Broadcast();

	DrawDebugLine(GetWorld(), MuzzleLocation, LaunchDirection * 1000.f, FColor::Red, false, 0.5f);
}

bool AZWeapon::CheckNeedToReload()
{
	return CurrentAmmo < 1;
}
