// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "ZInteractional.h"
#include "ZPickup.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZPickup : public AZInteractional
{
	GENERATED_BODY()
	
public:
	AZPickup();

public:
	virtual void BeginPlay() override;
	virtual void OnInteraction(class AZCharacter* NewCharacter) override;

public:
	void WhenSpawnedByItem();

public:
	void SetActive(bool NewState);
	void SetItem(class AZItem* NewItem);

public:
	class AZItem* const GetItem() const;
	bool IsActive() const;

public:
	/*
		Item을 생성하기 위한 클래스 정보를 담는 변수.
		나중에 Pickup이 처음 생성되는 경우, BeginePlay() 메소드에서 
		Game database로 부터 랜덤하게 Item 정보를 가져와 ItemClass변수를 초기화하도록 수정할 것.
		현재는 Test를 위해 에디터에서 설정.
	*/
	UPROPERTY(EditAnywhere, Category = Pickup)
	TSubclassOf<class AZItem> SpawnItemClass;

protected:
	UPROPERTY(VisibleAnywhere, Category = Pickup)
	bool bIsActive;

	UPROPERTY(VisibleAnywhere, Category = Pickup)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = Pickup)
	class AZItem* Item;

};
