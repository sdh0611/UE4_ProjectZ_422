// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/Actor.h"
#include "ZItem.generated.h"

UENUM(Blueprintable)
enum class EItemType : uint8
{
	Default,
	Weapon,
	Recovery,
	Doping,
	Invalid
};


UCLASS()
class PROJECTZ_422_API AZItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnUsed();

public:
	/*
		Item ������ �����ϱ� ���� �޼ҵ�.
		@return : �� �ش� Item�� �ִ� ���� ������ �ʰ��� ��ŭ�� ��.
	*/
	int32 AdjustQuantity(int32 Value);

public:
	void SetItemName(const FString& NewItemName);
	void SetMaxQuantityOfItem(int32 NewValue);
	void SetCurrentQuantityOfItem(int32 NewValue);
	void SetItemWeight(int32 NewWeight);
	void SetInventoryIndex(int32 NewIndex);
	void SetItemOwner(class AZCharacter* NewItemOwner);

public:
	const FString& GetItemName() const;
	int32 GetMaxQuantityOfItem() const;
	int32 GetCurrentQuantityOfItem() const;
	int32 GetItemWeight() const;
	int32 GetInventoryIndex() const;
	class AZCharacter* const GetItemOwner() const;
	bool IsItemQuantityMaximum() const;
	EItemType GetItemType() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	FString ItemName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	int32 MaxQuantityOfItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, Transient)
	int32 CurrentQuantityOfItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	int32 ItemWeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, Transient)
	int32 InventoryIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, Transient)
	class AZCharacter* ItemOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	EItemType ItemType;

};
