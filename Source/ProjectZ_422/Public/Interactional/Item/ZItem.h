// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/Actor.h"
#include "ZItem.generated.h"

// Item�� ���� ������ �ٲ� �� ȣ���� ��������Ʈ.
DECLARE_MULTICAST_DELEGATE(FOnItemInfoChanged);
// Item�� ���ŵ� �� ȣ���� ��������Ʈ.
DECLARE_MULTICAST_DELEGATE(FOnItemRemoved);


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

	// Item�� ���ۿ� ���� �޼ҵ�.
	UFUNCTION(BlueprintCallable, Category = Item)
	virtual void OnUsed();

	/*
		Item�� ���ŵ� ��� ȣ��� �޼ҵ�.
		�ַ� �ܺο��� ȣ���ϰ� �� ��. (ItemStatusComponent, Shop, etc...)
	*/
	virtual void OnRemoved();

	/*
		Item�� Player�� ���� Drop�� �� ȣ��� �޼ҵ�.
	*/
	UFUNCTION(BlueprintCallable, Category = Item)
	virtual void OnDropped();

public:
	/*
		Item ������ �����ϱ� ���� �޼ҵ�.
		@return : �� �ش� Item�� �ִ� ���� ������ �ʰ��� ��ŭ�� ��.
	*/
	int32 AdjustQuantity(int32 Value);

public:
	void SetCanDestroy(bool NewState);
	void SetItemName(const FString& NewItemName);
	void SetMaxQuantityOfItem(int32 NewValue);
	void SetCurrentQuantityOfItem(int32 NewValue);
	void SetItemWeight(int32 NewWeight);
	void SetInventoryIndex(int32 NewIndex);
	void SetItemOwner(class AZCharacter* NewItemOwner);
	void SetPickup(class AZPickup* NewPickup);
	void SetActive(bool NewState);

public:
	bool IsCanDestroy() const;
	const FString& GetItemName() const;
	int32 GetMaxQuantityOfItem() const;
	int32 GetCurrentQuantityOfItem() const;
	int32 GetItemWeight() const;
	int32 GetInventoryIndex() const;
	class AZCharacter* const GetItemOwner() const;
	bool IsItemQuantityMaximum() const;
	EItemType GetItemType() const;
	bool IsActive() const;

private:
	/*
		Item�� ���� �����ߴ��� üũ�ϴ� �Լ�.
		���� �������� ��� ItemStatusComponent�� RemoveItem ȣ��.
	*/
	void CheckItemExhausted();

public:
	FOnItemInfoChanged OnItemInfoChanged;
	FOnItemRemoved OnItemRemoved;

	UPROPERTY(EditAnywhere, Category = Item)
	TSubclassOf<class AZPickup> PickupClass;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	bool bCanDestroy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	bool bIsActive;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	class AZPickup* Pickup;

};
