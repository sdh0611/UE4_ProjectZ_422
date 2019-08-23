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

	//static ConstructorHelpers::FClassFinder<AZProjectile>
	//	CLASS_PROJECTILE(TEXT("Class'/Script/ProjectZ_422.ZProjectile'"));
	//if (CLASS_PROJECTILE.Succeeded())
	//{
	//	ProjectileClass = CLASS_PROJECTILE.Class;
	//}

	// -1 : Player에게 습득되지 않은 상태
	WeaponInventoryIndex = -1;
	bIsEquipped = false;

	Damage = 1.f;
	FireDelay = 0.f;

	WeaponCategory = EWeaponCategory::Invalid;
	WeaponType = EWeaponType::Invalid;

	MaxQuantityOfItem = 1;
}

void AZWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AZWeapon::OnRemoved()
{
	ZLOG_S(Warning);

	// Actor로부터 떼어냄.
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	Super::OnRemoved();
}

void AZWeapon::OnDropped()
{
	ZLOG_S(Warning);
	// Actor로부터 떼어냄.
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	Super::OnDropped();
}

void AZWeapon::InitItemData(const FZItemData * const NewItemData)
{
	Super::InitItemData(NewItemData);

	auto NewWeaponData = static_cast<const FZWeaponData*>(NewItemData);

	Damage = NewWeaponData->Damage;
	FireDelay = NewWeaponData->FireDelay;
	auto SKMesh = GetGameInstance<UZGameInstance>()->GetSkeletalMesh(ItemName);
	check(nullptr != SKMesh);
	WeaponMesh->SetSkeletalMesh(SKMesh);

	//MaxAmmo = NewWeaponData->MaxAmmo;
	WeaponType = GetWeaponTypeFromString(NewWeaponData->WeaponType);

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
	bIsEquipped = NewState;
}

void AZWeapon::SetWeaponCategory(EWeaponCategory NewWeaponCategory)
{
	WeaponCategory = NewWeaponCategory;
}

void AZWeapon::SetWeaponType(EWeaponType NewWeaponType)
{
	WeaponType = NewWeaponType;
}

int32 AZWeapon::GetWeaponInventoryIndex() const
{
	return WeaponInventoryIndex;
}

bool AZWeapon::IsEquipped() const
{
	return bIsEquipped;
}

EWeaponCategory AZWeapon::GetWeaponCategory() const
{
	return WeaponCategory;
}

EWeaponType AZWeapon::GetWeaponType() const
{
	return WeaponType;
}

FHitResult AZWeapon::WeaponTrace(float Distance, bool bDrawDebugLine)
{
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
	GetWorld()->LineTraceSingleByChannel(Hit, StartLoc, EndLoc, WEAPON_TRACE, TraceParams);
	
	if (bDrawDebugLine)
	{
		DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Red, false, 1.f);
	}

	return Hit;
}

void AZWeapon::Fire()
{
	//ZLOG(Warning, TEXT("Weapon Fire!!"));

	OnWeaponFired.Broadcast();
	OnItemInfoChanged.Broadcast();
}

void AZWeapon::FireEnd()
{

}
