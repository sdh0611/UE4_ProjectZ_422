// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	void AddItem(class AZItem* NewItem);
	void RemoveItem(int32 InventoryIndex, bool bDestroy);

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
		ItemList내 빈 공간의 Index를 반환하는 메소드.
		@return : InventoryIndex. 빈 공간이 없다면 -1 반환.
	*/
	int32 AllocateInventoryIndex();
	/*
		ItemList내 같은 이름을 가졌고, 현재 개수가 최대 개수에 도달하지 않은 Item을 검색.
		@return : ItemList내 AZItem Pointer. 없다면 nullptr 반환.
	*/
	class AZItem* GetItemByName(const FString& ItemName);

private:
	UPROPERTY(VisibleAnywhere, Category = ItemStatus)
	TArray<class AZItem*> ItemList;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemStatus, Meta = (AllowPrivateAccess = true))
	int32 MaxSizeOfItemList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemStatus, Meta = (AllowPrivateAccess = true), Transient)
	int32 CurrentSizeOfItemList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemStatus, Meta = (AllowPrivateAccess = true))
	int32 MaxWeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemStatus, Meta = (AllowPrivateAccess = true), Transient)
	int32 CurrentWeight;



};
