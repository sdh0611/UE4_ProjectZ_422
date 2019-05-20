// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Components/ActorComponent.h"
#include "ZCharacterItemStatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTZ_422_API UZCharacterItemStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UZCharacterItemStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/*
		Item�� ������ �� ȣ���ϰ� �� �޼ҵ�.
	*/
	void AddItem(class AZItem* NewItem, class AZPickup* OwnerPickup = nullptr);
	/*
		Weapon�� ������ �� ȣ���ϴ� �޼ҵ�.
		AddItem() �ȿ��� ȣ��� ��.
		@ Return : Character�� ��Ե� CurrentWeapon��
	*/
	class AZWeapon* const EquipWeapon(class AZWeapon* NewWeapon);
	/*
		Item�� List ������ ������ �� ȣ���ϰ� �� �޼ҵ�.
	*/
	void RemoveItem(int32 InventoryIndex, bool bIsDropped = false);

public:
	void SetMaxSizeOfItemList(int32 NewMaxSize);
	void SetCurrentSizeOfItemList(int32 NewCurrentSize);
	void SetMaxWeight(int32 NewMaxWeight);
	void SetCurrentWeight(int32 NewCurrentWeight);

public:
	int32 GetMaxSizeOfItemList() const;
	int32 GetCurrentSizeOfItemList() const;
	int32 GetMaxWeight() const;
	int32 GetCurrentWeight() const;
	class AZItem* GetItemByIndex(int32 ItemIndex) const;
	bool IsItemListFull() const;

private:
	/*
		ItemList�� �� ������ Index�� ��ȯ�ϴ� �޼ҵ�.
		@return : InventoryIndex. �� ������ ���ٸ� -1 ��ȯ.
	*/
	int32 AllocateInventoryIndex();
	/*
		ItemList�� ���� �̸��� ������, ���� ������ �ִ� ������ �������� ���� Item�� �˻�.
		@return : ItemList�� AZItem Pointer. ���ٸ� nullptr ��ȯ.
	*/
	class AZItem* GetItemByName(const FString& ItemName);

private:
	UPROPERTY(VisibleAnywhere, Category = ItemStatus)
	TArray<class AZItem*> ItemList;

	/*
		WepaonInventory�� �� Item���� �����ϴ� �����̳�.
		ItemList�� ������ ����
	*/
	UPROPERTY(VisibleAnywhere, Category = ItemStatus)
	TArray<class AZWeapon*> WeaponInventory;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemStatus, Meta = (AllowPrivateAccess = true))
	int32 MaxSizeOfItemList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemStatus, Meta = (AllowPrivateAccess = true), Transient)
	int32 CurrentSizeOfItemList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemStatus, Meta = (AllowPrivateAccess = true))
	int32 MaxWeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemStatus, Meta = (AllowPrivateAccess = true), Transient)
	int32 CurrentWeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemStatus, Meta = (AllowPrivateAccess = true))
	class AZCharacter* OwnerCharacter;

};
