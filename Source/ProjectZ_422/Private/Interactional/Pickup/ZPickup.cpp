// Fill out your copyright notice in the Description page of Project Settings.


#include "ZPickup.h"
#include "ZCharacter.h"
#include "ZItem.h"
#include "ZWeapon.h"
#include "ZCharacterItemStatusComponent.h"
#include "ZGameInstance.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"


AZPickup::AZPickup()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	// Code to test 
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_BODY(TEXT("StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SM_AR4.SM_AR4'"));
	if (SM_BODY.Succeeded())
	{
		Mesh->SetStaticMesh(SM_BODY.Object);
	}
	else
	{
		ZLOG(Error, TEXT("Fail to find pickup static mesh..."));
	}

	SetActorEnableCollision(ECollisionEnabled::QueryOnly);

	Item = nullptr;
	bIsActive = true;

	Name = TEXT("Default");
}

void AZPickup::BeginPlay()
{
	Super::BeginPlay();

	auto GameInstance = Cast<UZGameInstance>(GetGameInstance());
	check(nullptr != GameInstance);
	

}

void AZPickup::OnInteraction(AZCharacter * NewCharacter)
{
	ZLOG(Warning, TEXT("Interaction pickup!"));

	if (nullptr == Item)
	{
		/*
			Pickup이 Item을 생성한 적이 없는 경우.
			새로운 Item을 생성함.
		*/
		// Code to test 
		AZItem* NewItem = GetWorld()->SpawnActor<AZItem>(SpawnItemClass);
		if (NewItem)
		{
			ZLOG(Warning, TEXT("Spawn item success!"));
			// Code for test
			if (NewItem->GetItemType() == EItemType::Weapon)
			{
				ZLOG(Warning, TEXT("%s"), *Name);
				Cast<AZWeapon>(NewItem)->InitItemData(Cast<UZGameInstance>(GetGameInstance())->GetWeaponDataByName(Name));
			}
			// Add item in character's item status component.
			NewCharacter->GetItemStatusComponent()->AddItem(NewItem, this);
		}
	}
	else
	{
		/*
			Pickup이 Item을 생성한 적이 있는 경우.(즉, 한번 이상 버려진 Item)
		*/
		Item->SetActive(true);
		NewCharacter->GetItemStatusComponent()->AddItem(Item);
	}

	SetActive(false);

	Super::OnInteraction(NewCharacter);
}

void AZPickup::WhenSpawnedByItem()
{
	SetActive(true);
	Mesh->AddTorqueInRadians(GetActorForwardVector() * 400000.f);
}

void AZPickup::SetActive(bool NewState)
{
	if (NewState)
	{
		/*
			Pickup 활성화
		*/
		SetActorEnableCollision(ECollisionEnabled::QueryOnly);
		SetActorHiddenInGame(false);
		Mesh->SetSimulatePhysics(true);
	}
	else
	{
		/*
			Pickup 비활성화
		*/
		SetActorEnableCollision(ECollisionEnabled::NoCollision);
		SetActorHiddenInGame(true);
		Mesh->SetSimulatePhysics(false);
	}

	bIsActive = NewState;
}

void AZPickup::SetItem(AZItem * NewItem)
{
	if (nullptr == Item)
	{
		auto ZGameInstance = Cast<UZGameInstance>(GetGameInstance());
		check(nullptr != ZGameInstance);

		auto ItemMesh = ZGameInstance->GetStaticMesh(NewItem->GetItemName());
		check(nullptr != ItemMesh);

		Mesh->SetStaticMesh(ItemMesh);
	}
	else
	{
		if (Item->GetItemName() != NewItem->GetItemName())
		{
			auto ZGameInstance = Cast<UZGameInstance>(GetGameInstance());
			check(nullptr != ZGameInstance);

			auto ItemMesh = ZGameInstance->GetStaticMesh(NewItem->GetItemName());
			check(nullptr != ItemMesh);

			Mesh->SetStaticMesh(ItemMesh);
		}
	}

	Item = NewItem;
}

AZItem * const AZPickup::GetItem() const
{
	return Item;
}

bool AZPickup::IsActive() const
{
	return bIsActive;
}
