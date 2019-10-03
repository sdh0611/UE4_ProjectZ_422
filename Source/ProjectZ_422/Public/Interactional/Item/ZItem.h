// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/Actor.h"
#include "ZItem.generated.h"

// Item에 대한 정보가 바뀔 때 호출할 델리게이트.
DECLARE_MULTICAST_DELEGATE(FOnItemInfoChanged);
// Item이 제거될 때 호출할 델리게이트.
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
		Item이 Player에 의해 Drop될 때 호출될 메소드.
	*/
	//virtual void OnDropped();
	virtual void OnDropped(int32 Quantity = 1);
	/*
		Item이 제거될 경우 호출될 메소드.
		주로 외부에서 호출하게 될 것. (ItemStatusComponent, Shop, etc...)
	*/
	virtual void OnRemoved();

	/*
		Item생성 시 ItemData를 적용하기 위한 메소드.
		각 아이템 분류별로 오버라이딩.
	*/
	virtual void InitItemData(const struct FZItemData* const NewItemData);
	
	/* Pickup으로 부터 생성된 경우 호출. */
	virtual void ApplyItemInfo(FZItemInfo& NewItemInfo);

public:
	/*
		Item 개수를 조정하기 위한 메소드. 기본 동작은 덧셈.
		@return : 은 해당 Item의 최대 보유 갯수를 초과한 만큼의 값.
	*/
	int32 AdjustQuantity(int32 Value);
	virtual FZItemInfo CreateItemInfo();
	virtual void RepItemOwner();

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
		Item을 전부 소진했는지 체크하는 함수.
		전부 소진했을 경우 ItemStatusComponent의 RemoveItem 호출.
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

	/* Replicated using */
	UFUNCTION()
	virtual void OnRep_ItemOwner();

	UFUNCTION()
	void OnRep_ItemInfoChanged();

public:
	/* 주로 UI와 상호작용 하기 위한 델리게이트들. */
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