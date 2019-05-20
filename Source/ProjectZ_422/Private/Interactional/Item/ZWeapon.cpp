// Fill out your copyright notice in the Description page of Project Settings.


#include "ZWeapon.h"
#include "ZCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "ConstructorHelpers.h"



AZWeapon::AZWeapon()
{
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

	// -1 : Player���� ������� ���� ����
	WeaponInventoryIndex = -1;
	bIsEquipped = false;
}

void AZWeapon::BeginPlay()
{
	Super::BeginPlay();
	
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
	bIsEquipped = NewState;
}

int32 AZWeapon::GetWeaponInventoryIndex() const
{
	return WeaponInventoryIndex;
}

bool AZWeapon::IsEquipped() const
{
	return bIsEquipped;
}
