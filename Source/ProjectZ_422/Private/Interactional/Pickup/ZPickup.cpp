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

	DOREPLIFETIME(AZPickup, bIsActive);
	DOREPLIFETIME(AZPickup, ItemInfo);
	DOREPLIFETIME(AZPickup, Name);
	DOREPLIFETIME(AZPickup, SpawnItemClass);
}

void AZPickup::OnInteraction(AZCharacter * NewCharacter)
{
	ZLOG(Warning, TEXT("Interaction pickup!"));

	/*
		Item list에 아이템 추가.
		실질적인 아이템의 생성은 ItemStatusComponent에서 수행.
		Pickup의 파괴 여부는 ItemStatusComponent에서 판단. -> Item 추가에 실패할 수도 있기 때문.
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
			Pickup 활성화
		*/
		SetActorEnableCollision(true);
		SetActorHiddenInGame(false);
		//Mesh->SetSimulatePhysics(true);
	}
	else
	{
		/*
			Pickup 비활성화
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

	auto MyGameInstance = Cast<UZGameInstance>(GetGameInstance());
	check(MyGameInstance);

	auto ItemMesh = MyGameInstance->GetStaticMesh(ItemInfo.ItemName);
	if (nullptr == ItemMesh)
	{
		ZLOG(Error, TEXT("Item mesh not exsit.."));
		return;
	}

	Mesh->SetStaticMesh(ItemMesh);
	ClientSetStaticMesh(ItemMesh);
}

bool AZPickup::IsActive() const
{
	return bIsActive;
}

const FZItemInfo & AZPickup::GetItemInfo() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return ItemInfo;
}

bool AZPickup::ClientSetStaticMesh_Validate(UStaticMesh * NewMesh)
{
	return true;
}

void AZPickup::ClientSetStaticMesh_Implementation(UStaticMesh * NewMesh)
{
	Mesh->SetStaticMesh(NewMesh);

}
