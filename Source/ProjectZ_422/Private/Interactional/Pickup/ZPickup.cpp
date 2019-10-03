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
	Mesh->SetIsReplicated(true);

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

	DOREPLIFETIME(AZPickup, Item);
	DOREPLIFETIME(AZPickup, bIsActive);
	DOREPLIFETIME(AZPickup, ItemInfo);
	DOREPLIFETIME(AZPickup, Name);
	DOREPLIFETIME(AZPickup, SpawnItemClass);
}

void AZPickup::OnInteraction(AZCharacter * NewCharacter)
{
	ZLOG(Warning, TEXT("Interaction pickup!"));

	/*
		Item list�� ������ �߰�.
		�������� �������� ������ ItemStatusComponent���� ����.
		Pickup�� �ı� ���δ� ItemStatusComponent���� �Ǵ�. -> Item �߰��� ������ ���� �ֱ� ����.
	*/
	NewCharacter->GetItemStatusComponent()->AddItem(this);
	
	//SetActive(false);
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

void AZPickup::SetItemInfo(const FZItemInfo & NewItemInfo)
{
	ItemInfo = NewItemInfo;

	//auto MyGameInstance = Cast<UZGameInstance>(GetGameInstance());
	//check(MyGameInstance);

	//auto ItemMesh = MyGameInstance->GetStaticMesh(ItemInfo.ItemName);
	//if (nullptr == ItemMesh)
	//{
	//	ZLOG(Error, TEXT("Item mesh not exsit.."));
	//	return;
	//}

	//Mesh->SetStaticMesh(ItemMesh);
	//ClientSetStaticMesh(ItemMesh);
}

void AZPickup::SetItem(AZItem * NewItem)
{
	Item = NewItem;
	if (!::IsValid(Item))
	{
		ZLOG(Error, TEXT("Invalid item.."));
		return;
	}

	auto MyGameInstance = Cast<UZGameInstance>(GetGameInstance());
	check(MyGameInstance);

	auto ItemMesh = MyGameInstance->GetStaticMesh(Item->GetItemName());
	if (nullptr == ItemMesh)
	{
		ZLOG(Error, TEXT("Item mesh not exsit.."));
		return;
	}

	Mesh->SetStaticMesh(ItemMesh);
	MulticastSetStaticMesh(ItemMesh);

}

bool AZPickup::IsActive() const
{
	return bIsActive;
}

FZItemInfo AZPickup::GetItemInfo() 
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return ItemInfo;
}

AZItem * const AZPickup::GetItem() const
{
	return Item;
}

void AZPickup::MulticastSetStaticMesh_Implementation(UStaticMesh * NewMesh)
{
	Mesh->SetStaticMesh(NewMesh);

}
