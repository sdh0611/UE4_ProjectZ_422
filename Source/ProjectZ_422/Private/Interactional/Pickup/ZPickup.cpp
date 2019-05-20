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
			Pickup�� Item�� ������ ���� ���� ���.
			���ο� Item�� ������.
		*/
		// Code to test 
		AZItem* NewItem = GetWorld()->SpawnActor<AZItem>(SpawnItemClass);
		if (NewItem)
		{
			ZLOG(Warning, TEXT("Spawn item success!"));
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

void AZPickup::WhenSpawnedByItem()
{
	Mesh->AddTorqueInRadians(GetActorForwardVector() * 400000.f);
}

void AZPickup::SetActive(bool NewState)
{
	if (NewState)
	{
		/*
			Pickup Ȱ��ȭ
		*/
		SetActorEnableCollision(ECollisionEnabled::QueryOnly);
		SetActorHiddenInGame(false);
		Mesh->SetSimulatePhysics(true);
	}
	else
	{
		/*
			Pickup ��Ȱ��ȭ
		*/
		SetActorEnableCollision(ECollisionEnabled::NoCollision);
		SetActorHiddenInGame(true);
		Mesh->SetSimulatePhysics(false);
	}

	bIsActive = NewState;
}

void AZPickup::SetItem(AZItem * NewItem)
{
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
