// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Components/ActorComponent.h"
#include "ZCharacterItemStatusComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMoneyInfoChange, int32);

enum EWeaponSlot : uint8
{
	Main1 = 0,
	Main2,
	Grenade,
	Knife
};

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
		Item을 습득할 때 호출하게 될 메소드.
	*/
	void AddItem(class AZItem* NewItem, class AZPickup* OwnerPickup = nullptr);

	/*
		Weapon을 장착할 때 호출하는 메소드.
		AddItem() 안에서 호출될 것.
		@ Return : Character가 들게될 CurrentWeapon값
	*/
	class AZWeapon* const EquipWeapon(class AZWeapon* NewWeapon);

	/*
		Item을 List 내에서 제거할 때 호출하게 될 메소드.
	*/
	void RemoveItem(int32 InventoryIndex, bool bIsDropped = false);

	/*
		Character의 DropWeapon에서 호출되는 메소드.
	*/
	void DropWeapon(int32 WeaponInventoryIndex);

	/*
		WeaponInventory로부터 Weapon 포인터를 얻어올 때 사용하는 메소드.
	*/
	class AZWeapon* const GetWeaponFromWeaponInventory(int32 NewWeaponIndex);

	/*
		돈 계산을 위한 메소드.
	*/
	void AdjustMoney(int32 Value);

public:
	void SetMaxSizeOfItemList(int32 NewMaxSize);
	void SetCurrentSizeOfItemList(int32 NewCurrentSize);
	void SetMaxWeight(int32 NewMaxWeight);
	void SetCurrentWeight(int32 NewCurrentWeight);
	void SetCurrentMoney(int32 NewCurrentMoney);

public:
	int32 GetMaxSizeOfItemList() const;
	int32 GetCurrentSizeOfItemList() const;
	int32 GetMaxWeight() const;
	int32 GetCurrentWeight() const;
	int32 GetCurrentMoney() const;
	bool IsItemListFull() const;
	const TArray<class AZItem*>& GetItemList() const;
	class AZItem* GetItemByIndex(int32 ItemIndex) const;
	/*
		ItemList내 같은 이름을 가졌고, 현재 개수가 최대 개수에 도달하지 않은 Item을 검색.
		전부 최대 개수를 보유하고 있으면 그냥 첫번째 아이템 반환.
		@return : ItemList내 AZItem Pointer. 없다면 nullptr 반환.
	*/
	class AZItem* GetItemByName(const FString& ItemName);

private:
	/*
		ItemList내 빈 공간의 Index를 반환하는 메소드.
		@return : InventoryIndex. 빈 공간이 없다면 -1 반환.
	*/
	int32 AllocateInventoryIndex();

public:
	/* UI와 연동 */
	FOnMoneyInfoChange OnMoneyInfoChange;

private:
	UPROPERTY(VisibleAnywhere, Category = ItemStatus)
	TArray<class AZItem*> ItemList;

	/*
		WepaonInventory에 들어갈 Item들을 관리하는 컨테이너.
		ItemList와 포인터 공유
		[Gun1] [Gun2] [Knife] [Grenade]
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemStatus, Meta = (AllowPrivateAccess = true), Transient)
	int32 CurrentMoney;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemStatus, Meta = (AllowPrivateAccess = true))
	class AZCharacter* OwnerCharacter;

};
