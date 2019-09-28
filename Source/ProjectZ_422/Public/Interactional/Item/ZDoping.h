// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "ZItem.h"
#include "ZUsableItemInterface.h"
#include "ZDoping.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZDoping : public AZItem, public IZUsableItemInterface
{
	GENERATED_BODY()

public:
	AZDoping();

public:
	virtual void InitItemData(const struct FZItemData* NewItemData) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnUsed() override;


private:
	/* From client to server RPC */
	
	
	
	/* From client to server RPC */

	

private:
	UPROPERTY(EditAnywhere, Category = Recovery)
	float DopingAmount;

	UPROPERTY(EditAnywhere, Category = Recovery)
	float DopingTime;

};
