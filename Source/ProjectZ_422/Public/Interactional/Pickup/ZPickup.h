// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "ZInteractional.h"
#include "ZItem.h"
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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnInteraction(class AZCharacter* NewCharacter) override;
	virtual void OnFocus() override;
	virtual void OnFocusEnd() override;


public:
	void WhenSpawnedByItem();

public:
	void SetActive(bool NewState);
	void SetItem(class AZItem* NewItem);

	bool IsActive() const;
	class AZItem* const GetItem() const;

private:
	/* From server to client RPC*/
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetStaticMesh(class UStaticMesh* NewMesh);
	void MulticastSetStaticMesh_Implementation(class UStaticMesh* NewMesh);

public:
	/*
		Item�� �����ϱ� ���� Ŭ���� ������ ��� ����.
		���߿� Pickup�� ó�� �����Ǵ� ���, BeginePlay() �޼ҵ忡�� 
		Game database�� ���� �����ϰ� Item ������ ������ ItemClass������ �ʱ�ȭ�ϵ��� ������ ��.
		����� Test�� ���� �����Ϳ��� ����.
	*/
	UPROPERTY(EditAnywhere, Category = Pickup, Replicated)
	TSubclassOf<class AZItem> SpawnItemClass;

protected:
	UPROPERTY(VisibleAnywhere, Category = Pickup, Replicated)
	class AZItem* Item;

	UPROPERTY(VisibleAnywhere, Category = Pickup, Replicated)
	bool bIsActive;

	UPROPERTY(VisibleAnywhere, Category = Pickup)
	class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, Category = Pickup)
	class UStaticMeshComponent* Mesh;

	// Item Name
	UPROPERTY(EditAnywhere, Category = Pickup, Replicated)
	FString Name;

};
