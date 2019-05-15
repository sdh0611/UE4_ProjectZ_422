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
	virtual void OnInteraction(class AZCharacter* NewCharacter) override;

public:
	/*
		Item�� �����ϱ� ���� Ŭ���� ������ ��� ����.
		���߿� Pickup�� ó�� �����Ǵ� ���, BeginePlay() �޼ҵ忡�� 
		Game database�� ���� �����ϰ� Item ������ ������ ItemClass������ �ʱ�ȭ�ϵ��� ������ ��.
		����� Test�� ���� �����Ϳ��� ����.
	*/
	UPROPERTY(EditAnywhere, Category = Pickup)
	TSubclassOf<class AZItem> SpawnItemClass;

protected:
	UPROPERTY(VisibleAnywhere, Category = Pickup)
	class UStaticMeshComponent* Mesh;

	
};
