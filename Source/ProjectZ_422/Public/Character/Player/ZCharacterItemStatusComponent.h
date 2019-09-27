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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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

	/*
		Character�� DropWeapon���� ȣ��Ǵ� �޼ҵ�.
	*/
	void DropWeapon(int32 WeaponInventoryIndex);

	/*
		WeaponInventory�κ��� Weapon �����͸� ���� �� ����ϴ� �޼ҵ�.
	*/
	class AZWeapon* const GetWeaponFromWeaponInventory(int32 NewWeaponIndex);

	/*
		�� ����� ���� �޼ҵ�.
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
		ItemList�� ���� �̸��� ������, ���� ������ �ִ� ������ �������� ���� Item�� �˻�.
		���� �ִ� ������ �����ϰ� ������ �׳� ù��° ������ ��ȯ.
		@return : ItemList�� AZItem Pointer. ���ٸ� nullptr ��ȯ.
	*/
	class AZItem* GetItemByName(const FString& ItemName);

private:
	/*
		ItemList�� �� ������ Index�� ��ȯ�ϴ� �޼ҵ�.
		@return : InventoryIndex. �� ������ ���ٸ� -1 ��ȯ.
	*/
	int32 AllocateInventoryIndex();


private:
	/* Client to server RPC */



	/* Server to client RPC */



	/* Replicated using method */
	UFUNCTION()
	void OnRep_CurrentMoney();


public:
	/* UI�� ���� */
	FOnMoneyInfoChange OnMoneyInfoChange;

private:
	UPROPERTY(VisibleAnywhere, Category = ItemStatus, Transient, Replicated)
	TArray<class AZItem*> ItemList;

	/*
		WepaonInventory�� �� Item���� �����ϴ� �����̳�.
		ItemList�� ������ ����
		[Gun1] [Gun2] [Knife] [Grenade]
	*/
	UPROPERTY(VisibleAnywhere, Category = ItemStatus)
	TArray<class AZWeapon*> WeaponInventory;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemStatus)
	int32 MaxSizeOfItemList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemStatus, Transient)
	int32 CurrentSizeOfItemList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemStatus)
	int32 MaxWeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemStatus, Transient, Replicated)
	int32 CurrentWeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ItemStatus, Transient, ReplicatedUsing = OnRep_CurrentMoney)
	int32 CurrentMoney;

	UPROPERTY(BlueprintReadOnly, Category = ItemStatus)
	TWeakObjectPtr<class AZCharacter> OwnerCharacter;

};
