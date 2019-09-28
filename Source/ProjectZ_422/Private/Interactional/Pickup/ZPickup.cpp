// Fill out your copyright notice in the Description page of Project Settings.


#include "ZPickup.h"
#include "ZCharacter.h"
#include "ZItem.h"
#include "ZWeapon.h"
#include "ZCharacterItemStatusComponent.h"
#include "ZGameInstance.h"
#include "Components/BoxComponent.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "UnrealNetwork.h"



AZPickup::AZPickup()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//RootComponent = Mesh;
	Mesh->SetupAttachment(RootComponent);

	Item = nullptr;
	bIsActive = true;

	Name = TEXT("Default");
}

void AZPickup::BeginPlay()
{
	Super::BeginPlay();

}

void AZPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZPickup, bIsActive);
	DOREPLIFETIME(AZPickup, Item);
	DOREPLIFETIME(AZPickup, Name);
}

void AZPickup::OnInteraction(AZCharacter * NewCharacter)
{
	ZLOG(Warning, TEXT("Interaction pickup!"));
	if (!HasAuthority())
	{
		ServerOnInteraction(NewCharacter);
		return;
	}


	if (nullptr == Item)
	{
		/*
			Pickup�� Item�� ������ ���� ���� ���.
			���ο� Item�� ������.
		*/
		// Code to test 
		AZItem* NewItem = GetWorld()->SpawnActor<AZItem>(SpawnItemClass);
		if (NewItem)
		{
			ZLOG(Warning, TEXT("Spawn item success!"));

			auto MyGameInstance = GetGameInstance<UZGameInstance>();
			check(nullptr != MyGameInstance);

			NewItem->InitItemData(MyGameInstance->GetItemDataByName(Name));

			// Add item in character's item status component.
			NewCharacter->GetItemStatusComponent()->AddItem(NewItem, this);
		}
	}
	else
	{
		/*
			Pickup�� Item�� ������ ���� �ִ� ���.(��, �ѹ� �̻� ������ Item)
		*/
		Item->SetActive(true);
		NewCharacter->GetItemStatusComponent()->AddItem(Item);
	}

	SetActive(false);

	Super::OnInteraction(NewCharacter);
}

void AZPickup::OnFocus()
{
	Mesh->SetRenderCustomDepth(true);
}

void AZPickup::OnFocusEnd()
{
	Mesh->SetRenderCustomDepth(false);
}

void AZPickup::WhenSpawnedByItem()
{
	SetActive(true);
	CollisionBox->AddTorqueInRadians(GetActorForwardVector() * 400000.f);
}

void AZPickup::SetActive(bool NewState)
{
	if (NewState)
	{
		/*
			Pickup Ȱ��ȭ
		*/
		SetActorEnableCollision(true);
		SetActorHiddenInGame(false);
		//Mesh->SetSimulatePhysics(true);
	}
	else
	{
		/*
			Pickup ��Ȱ��ȭ
		*/
		SetActorEnableCollision(false);
		SetActorHiddenInGame(true);
		//Mesh->SetSimulatePhysics(false);
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

bool AZPickup::ServerOnInteraction_Validate(class AZCharacter* NewCharacter)
{
	return true;
}

void AZPickup::ServerOnInteraction_Implementation(class AZCharacter* NewCharacter)
{
	OnInteraction(NewCharacter);
}
