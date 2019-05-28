// Fill out your copyright notice in the Description page of Project Settings.


#include "ZWeapon.h"
#include "ZCharacter.h"
#include "ZProjectile.h"
#include "ZPlayerController.h"
#include "ZGameInstance.h"
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

	// -1 : Player���� ������� ���� ����
	WeaponInventoryIndex = -1;
	bIsEquipped = false;
	bWantsToFire = false;
	bIsReloading = false;
	FireTimer = 0.f;
	// Code for test
	FireDelay = 0.15f;
	Damage = 1.f;

	CurrentAmmo = 0;
	MaxAmmo = 30;
	WeaponType = EWeaponType::AR;

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

	// Actor�κ��� ���.
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	//SetIsEquipped(false);
	//// WeaponInventory Index �ʱ�ȭ
	//SetWeaponInventoryIndex(-1);

	Super::OnRemoved();
}

void AZWeapon::OnDropped()
{
	ZLOG_S(Warning);
	// Actor�κ��� ���.
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	//// WeaponInventory Index �ʱ�ȭ
	//SetWeaponInventoryIndex(-1);

	Super::OnDropped();
}

void AZWeapon::InitItemData(const FZItemData * const NewItemData)
{
	Super::InitItemData(NewItemData);

	auto NewWeaponData = static_cast<const FZWeaponData*>(NewItemData);

	Damage = NewWeaponData->Damage;
	FireDelay = NewWeaponData->FireDelay;
	auto SKMesh = Cast<UZGameInstance>(GetGameInstance())->GetSkeletalMesh(ItemName);
	check(nullptr != SKMesh);
	WeaponMesh->SetSkeletalMesh(SKMesh);

	MaxAmmo = NewWeaponData->MaxAmmo;
	WeaponType = GetWeaponTypeFromString(NewWeaponData->WeaponType);

}

void AZWeapon::InitWeaponData(const FZWeaponData * const NewWeaponData)
{

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
	//	Weaponinventory ������ �Ѿ ���.
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
	GetWorld()->LineTraceSingleByChannel(Hit, StartLoc, EndLoc, ECollisionChannel::ECC_GameTraceChannel4,TraceParams);
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
		Projectile->SetDamage(Damage);
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
