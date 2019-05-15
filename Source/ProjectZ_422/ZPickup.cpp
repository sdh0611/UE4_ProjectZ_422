// Fill out your copyright notice in the Description page of Project Settings.


#include "ZPickup.h"
#include "ZCharacter.h"
#include "ZItem.h"
#include "ZCharacterItemStatusComponent.h"
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


}

void AZPickup::OnInteraction(AZCharacter * NewCharacter)
{
	ZLOG(Warning, TEXT("Interaction pickup!"));

	// Code to test 
	AZItem* NewItem = GetWorld()->SpawnActor<AZItem>(AZItem::StaticClass());
	if (NewItem)
	{
		ZLOG(Warning, TEXT("Spawn item success!"));
		// Add item in character's item status component.
		NewCharacter->GetItemStatusComponent()->AddItem(NewItem);
	}

	Super::OnInteraction(NewCharacter);
}
