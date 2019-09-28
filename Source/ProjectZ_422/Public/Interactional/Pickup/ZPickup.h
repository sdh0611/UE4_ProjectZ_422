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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnInteraction(class AZCharacter* NewCharacter) override;
	virtual void OnFocus() override;
	virtual void OnFocusEnd() override;


public:
	void WhenSpawnedByItem();

public:
	void SetActive(bool NewState);
	void SetItem(class AZItem* NewItem);

public:
	class AZItem* const GetItem() const;
	bool IsActive() const;

protected:
	/* From client to server RPC */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerOnInteraction(class AZCharacter* NewCharacter);
	bool ServerOnInteraction_Validate(class AZCharacter* NewCharacter);
	void ServerOnInteraction_Implementation(class AZCharacter* NewCharacter);

	/* From server to client RPC */




protected:
	/*
		Item�� �����ϱ� ���� Ŭ���� ������ ��� ����.
		���߿� Pickup�� ó�� �����Ǵ� ���, BeginePlay() �޼ҵ忡�� 
		Game database�� ���� �����ϰ� Item ������ ������ ItemClass������ �ʱ�ȭ�ϵ��� ������ ��.
		����� Test�� ���� �����Ϳ��� ����.
	*/
	UPROPERTY(EditAnywhere, Category = Pickup)
	TSubclassOf<class AZItem> SpawnItemClass;

protected:
	UPROPERTY(VisibleAnywhere, Category = Pickup, Replicated)
	bool bIsActive;

	UPROPERTY(VisibleAnywhere, Category = Pickup)
	class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, Category = Pickup)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = Pickup, Replicated)
	class AZItem* Item;

	// Item Name
	UPROPERTY(EditAnywhere, Category = Pickup, Replicated)
	FString Name;

};
