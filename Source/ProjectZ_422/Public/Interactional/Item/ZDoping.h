// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "ZItem.h"
#include "ZDoping.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZDoping : public AZItem
{
	GENERATED_BODY()

public:
	AZDoping();

public:
	virtual void InitItemData(const struct FZItemData* NewItemData) override;


private:
	UPROPERTY(EditAnywhere, Category = Recovery)
	float DopingAmount;

	UPROPERTY(EditAnywhere, Category = Recovery)
	float DopingTime;

};
