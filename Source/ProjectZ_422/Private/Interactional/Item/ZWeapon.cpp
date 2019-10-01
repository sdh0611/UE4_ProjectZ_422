// Fill out your copyright notice in the Description page of Project Settings.


#include "ZWeapon.h"
#include "ZCharacter.h"
#include "ZProjectile.h"
#include "ZPlayerController.h"
#include "ZGameInstance.h"
#include "ZPlayerAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"


AZWeapon::AZWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemType = EItemType::Weapon;

	// Code to test
	ItemName = TEXT("Weapon");

	// Create  skeletal mesh component
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	//WeaponMesh->SetIsReplicated(true);
	RootComponent = WeaponMesh;

	// -1 : Player에게 습득되지 않은 상태
	WeaponInventoryIndex = -1;
	bIsEquipped = false;

	Damage = 1.f;
	TraceDistance = 100000.f;

	bToggleDebug = false;

	WeaponCategory = EWeaponCategory::Invalid;

	MaxQuantityOfItem = 1;

	MontageTable.Add(TEXT("Fire"), nullptr);
}

void AZWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AZWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AZWeapon, bIsEquipped, COND_OwnerOnly);
	DOREPLIFETIME(AZWeapon, WeaponInventoryIndex);
	DOREPLIFETIME(AZWeapon, AttachSocketName);

}

void AZWeapon::OnRemoved()
{
	ZLOG_S(Warning);

	// Actor로부터 떼어냄.
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	Super::OnRemoved();
}

void AZWeapon::OnDropped(int32 Quantity)
{
	ZLOG_S(Warning);
	// Actor로부터 떼어냄.
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	Super::OnDropped(Quantity);
}

void AZWeapon::InitItemData(const FZItemData * const NewItemData)
{
	Super::InitItemData(NewItemData);

	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	check(nullptr != MyGameInstance);

	auto NewWeaponData = MyGameInstance->GetWeaponDataByName(NewItemData->ItemName);
	if (nullptr == NewWeaponData)
	{
		ZLOG(Error, TEXT("Invalid weapon data."));
		return;
	}

	Damage = NewWeaponData->Damage;
	auto SKMesh = MyGameInstance->GetSkeletalMesh(ItemName);
	if (nullptr == SKMesh)
	{
		ZLOG(Error, TEXT("SKMesh null.."));
		return;
	}

	WeaponMesh->SetSkeletalMesh(SKMesh);
	if (HasAuthority())
	{
		ClientSetSkeletalMesh(SKMesh);
	}
}

void AZWeapon::ApplyItemInfo(FZItemInfo NewItemInfo)
{
	if (NewItemInfo.IsOfType(FZWeaponInfo::TypeID))
	{
		FZWeaponInfo* WeaponInfo = static_cast<FZWeaponInfo*>(&NewItemInfo);
		WeaponCategory = WeaponInfo->WeaponCategory;
	}

	Super::ApplyItemInfo(NewItemInfo);

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

UAnimMontage * const AZWeapon::GetFireAnimMontage() const
{
	return FindMontage(TEXT("Fire"));
}

FZItemInfo AZWeapon::CreateItemInfo()
{
	FZWeaponInfo ItemInfo;

	InitItemInfo(ItemInfo);

	return ItemInfo;
}

void AZWeapon::RepItemOwner()
{
	Super::RepItemOwner();
	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	check(nullptr != MyGameInstance);

	auto SKMesh = MyGameInstance->GetSkeletalMesh(ItemName);
	if (nullptr == SKMesh)
	{
		ZLOG(Error, TEXT("SKMesh null.."));
		return;
	}

	WeaponMesh->SetSkeletalMesh(SKMesh);

	if (ItemOwner)
	{
		ZLOG_S(Error);
		//ItemOwner->MulticastAttachWeapon(this, AttachSocketName);
		ItemOwner->AttachWeapon(this, AttachSocketName);
		//DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
	
	//AttachToComponent(ItemOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, AttachSocketName);
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
	FVector EndLoc = StartLoc + Distance * CamRot.Vector();

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

void AZWeapon::InitItemInfo(FZItemInfo & ItemInfo)
{
	Super::InitItemInfo(ItemInfo);

	if (ItemInfo.IsOfType(FZWeaponInfo::TypeID))
	{
		ZLOG_S(Error);
		FZWeaponInfo* WeaponInfo = static_cast<FZWeaponInfo*>(&ItemInfo);
		WeaponInfo->WeaponCategory = WeaponCategory;
	}
	
}

bool AZWeapon::ServerFire_Validate(bool bFire)
{
	return true;
}

void AZWeapon::ServerFire_Implementation(bool bFire)
{
	if (bFire)
	{
		Fire();
	}
	else
	{
		FireEnd();
	}
}

bool AZWeapon::ClientSetSkeletalMesh_Validate(USkeletalMesh * NewMesh)
{
	return true;
}

void AZWeapon::ClientSetSkeletalMesh_Implementation(USkeletalMesh * NewMesh)
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Set SKMesh."));
	WeaponMesh->SetSkeletalMesh(NewMesh);
}

void AZWeapon::Fire()
{
	if (!HasAuthority())
	{
		return;
	}

	ZLOG(Warning, TEXT("Weapon Fire!!"));

	if (!::IsValid(ItemOwner))
	{
		ZLOG(Error, TEXT("Invalid item owner.."));
		return;
	}

	if (ItemOwner->IsAiming())
	{
		ItemOwner->MulticastPlayItemMontage(TEXT("FireAim"));
	}
	else
	{
		ItemOwner->MulticastPlayItemMontage(TEXT("Fire"));
	}
	//auto PlayerAnim = ItemOwner->GetCharacterAnimInstance();
	//if (!::IsValid(PlayerAnim))
	//{
	//	ZLOG(Error, TEXT("PlayerAnim not valid."));
	//	return;
	//}
	//MulticastPlayItemMontage(TEXT("Fire"));
	//if (GetFireAnimMontage())
	//{
	//	PlayerAnim->Montage_Play(GetFireAnimMontage());
	//}

	OnWeaponFired.Broadcast();
	OnItemInfoChanged.Broadcast();
}

void AZWeapon::FireEnd()
{

}

void AZWeapon::StartFire()
{
	ServerFire(true);
}

void AZWeapon::StopFire()
{
	ServerFire(false);
}
