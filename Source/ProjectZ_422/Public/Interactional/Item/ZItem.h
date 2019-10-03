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

USTRUCT(BlueprintType)
struct PROJECTZ_422_API FZItemInfo
{
	GENERATED_BODY()

public:
	FZItemInfo() { ID = TypeID; };
	virtual ~FZItemInfo() { };

protected:
	int32 ID;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanDestroy = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString ItemName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentQuantityOfItem = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemType ItemType = EItemType::Default;

	bool bInit = false;

	static const int32 TypeID = 0;

	virtual bool IsOfType(int32 NewID) const { return (NewID == ID); }
	//virtual bool IsOfType(int32 NewID) const { ZLOG(Error, TEXT("ID : %d"), TypeID); return (NewID == FZItemInfo::TypeID); }

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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/*
		Item�� Player�� ���� Drop�� �� ȣ��� �޼ҵ�.
	*/
	//virtual void OnDropped();
	virtual void OnDropped(int32 Quantity = 1);
	/*
		Item�� ���ŵ� ��� ȣ��� �޼ҵ�.
		�ַ� �ܺο��� ȣ���ϰ� �� ��. (ItemStatusComponent, Shop, etc...)
	*/
	virtual void OnRemoved();
	virtual void OnPicked();

	virtual void WhenDropped();

	/*
		Item���� �� ItemData�� �����ϱ� ���� �޼ҵ�.
		�� ������ �з����� �������̵�.
	*/
	virtual void InitItemData(const struct FZItemData* const NewItemData);
	
	/* Pickup���� ���� ������ ��� ȣ��. */
	virtual void ApplyItemInfo(FZItemInfo& NewItemInfo);

public:
	/*
		Item ������ �����ϱ� ���� �޼ҵ�. �⺻ ������ ����.
		@return : �� �ش� Item�� �ִ� ���� ������ �ʰ��� ��ŭ�� ��.
	*/
	int32 AdjustQuantity(int32 Value);
	virtual FZItemInfo CreateItemInfo();
	virtual void RepItemOwner();
	virtual void ClearDelegates();

public:
	void SetCanDestroy(bool NewState);
	void SetItemName(const FString& NewItemName);
	void SetMaxQuantityOfItem(int32 NewValue);
	void SetCurrentQuantityOfItem(int32 NewValue);
	void SetItemWeight(int32 NewWeight);
	void SetInventoryIndex(int32 NewIndex);
	void SetItemOwner(class AZCharacter* NewItemOwner);
	void SetActive(bool NewState);

	bool IsCanDestroy() const;
	const FString& GetItemName() const;
	int32 GetMaxQuantityOfItem() const;
	int32 GetCurrentQuantityOfItem() const;
	int32 GetItemWeight() const;
	int32 GetInventoryIndex() const;
	const FText& GetItemExplanation() const;
	class AZCharacter* const GetItemOwner() const;
	bool IsItemQuantityMaximum() const;
	EItemType GetItemType() const;
	bool IsActive() const;
	class UAnimMontage* const FindMontage(const FString& MontageName) const;


protected:
	/*
		Item�� ���� �����ߴ��� üũ�ϴ� �Լ�.
		���� �������� ��� ItemStatusComponent�� RemoveItem ȣ��.
	*/
	void CheckItemExhausted();
	virtual void InitItemInfo(FZItemInfo& ItemInfo);

protected:
	/* From client to server RPC*/
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerOnDropItem(int32 Quantity);
	bool ServerOnDropItem_Validate(int32 Quantity);
	void ServerOnDropItem_Implementation(int32 Quantity);
	
	/* From server to client RPC*/
	UFUNCTION(Client, Reliable, WithValidation)
	void ClientOnItemRemoved();
	bool ClientOnItemRemoved_Validate();
	void ClientOnItemRemoved_Implementation();

	/* Net multicast */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnItemDropped();
	void MulticastOnItemDropped_Implementation();


	/* Replicated using */
	UFUNCTION()
	virtual void OnRep_ItemOwner();

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, Replicated)
	FString ItemName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, Replicated)
	int32 MaxQuantityOfItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, Transient, ReplicatedUsing = OnRep_ItemInfoChanged)
	int32 CurrentQuantityOfItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, Replicated)
	int32 ItemWeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, Transient, Replicated)
	int32 InventoryIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, Transient, Replicated)
	FText ItemExplanation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, Transient, ReplicatedUsing = OnRep_ItemOwner)
	class AZCharacter* ItemOwner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	EItemType ItemType;

	UPROPERTY(EditAnywhere)
	TMap<FString, class UAnimMontage*> MontageTable;

};