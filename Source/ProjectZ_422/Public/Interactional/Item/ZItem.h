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
	Ammo,
	Recovery,
	Doping,
	Invalid
};

static FString GetItemTypeEnumAsString(EItemType ItemType)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EItemType"), true);
	if (nullptr == EnumPtr)
	{
		return FString(TEXT("Invalid"));
	}

	FString Concated = EnumPtr->GetNameStringByValue((int64)ItemType);
	Concated.RemoveFromStart(TEXT("EItemType::"));

	return Concated;
}

static EItemType GetItemTypeFromString(const FString& ItemTypeName)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EItemType"), true);
	if (nullptr == EnumPtr)
	{
		return EItemType::Invalid;
	}

	if (ItemTypeName == TEXT("Weapon"))
	{
		return EItemType::Weapon;
	}
	else if (ItemTypeName == TEXT("Recovery"))
	{
		return EItemType::Recovery;
	}
	else if (ItemTypeName == TEXT("Doping"))
	{
		return EItemType::Doping;
	}
	else if (ItemTypeName == TEXT("Ammo"))
	{
		return EItemType::Ammo;
	}

	return EItemType::Invalid;
}



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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/*
		Item�� ���ۿ� ���� �޼ҵ�.
	*/
	//UFUNCTION(BlueprintCallable, Category = Item)
	//virtual void OnUsed();

	/*
		Item�� Player�� ���� Drop�� �� ȣ��� �޼ҵ�.
	*/
	UFUNCTION(BlueprintCallable, Category = Item)
	virtual void OnDropped();

	virtual void OnDropped(int32 Quantity);
	/*
		Item�� ���ŵ� ��� ȣ��� �޼ҵ�.
		�ַ� �ܺο��� ȣ���ϰ� �� ��. (ItemStatusComponent, Shop, etc...)
	*/
	virtual void OnRemoved();

	/*
		Item���� �� ItemData�� �����ϱ� ���� �޼ҵ�.
		�� ������ �з����� �������̵�.
	*/
	virtual void InitItemData(const struct FZItemData* const NewItemData);

public:
	/*
		Item ������ �����ϱ� ���� �޼ҵ�. �⺻ ������ ����.
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
	class UTexture2D* const GetItemImage() const;
	const FText& GetItemExplanation() const;
	class AZCharacter* const GetItemOwner() const;
	bool IsItemQuantityMaximum() const;
	EItemType GetItemType() const;
	bool IsActive() const;
	class UAnimMontage* const FindMontage(const FString& MontageName) const;


private:
	/*
		Item�� ���� �����ߴ��� üũ�ϴ� �Լ�.
		���� �������� ��� ItemStatusComponent�� RemoveItem ȣ��.
	*/
	void CheckItemExhausted();


protected:
	/* From client to server RPC*/
	
	
	/* From server to client RPC*/


	/* Replicated using */
	UFUNCTION()
	void OnRep_ItemOwner();

	UFUNCTION()
	void OnRep_ItemInfoChanged();

public:
	/* �ַ� UI�� ��ȣ�ۿ� �ϱ� ���� ��������Ʈ��. */
	FOnItemInfoChanged OnItemInfoChanged;
	FOnItemRemoved OnItemRemoved;

	UPROPERTY(EditAnywhere, Category = Item)
	TSubclassOf<class AZPickup> PickupClass;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item, Replicated)
	bool bCanDestroy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item, Replicated)
	bool bIsActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	FString ItemName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	int32 MaxQuantityOfItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, Transient, ReplicatedUsing = OnRep_ItemInfoChanged)
	int32 CurrentQuantityOfItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	int32 ItemWeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, Transient, Replicated)
	int32 InventoryIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, Transient)
	FText ItemExplanation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, Transient)
	class UTexture2D* ItemImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, Transient, ReplicatedUsing = OnRep_ItemOwner)
	class AZCharacter* ItemOwner;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	EItemType ItemType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	class AZPickup* Pickup;

	UPROPERTY(EditAnywhere)
	TMap<FString, class UAnimMontage*> MontageTable;

};
