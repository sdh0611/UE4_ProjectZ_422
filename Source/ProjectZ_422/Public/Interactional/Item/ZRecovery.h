// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "ZItem.h"
#include "ZRecovery.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZRecovery : public AZItem
{
	GENERATED_BODY()
	
public:
	AZRecovery();

public:
	virtual void InitItemData(const struct FZItemData* NewItemData) override;
	virtual void OnUsed() override;

private:
	UPROPERTY(EditAnywhere, Category = Recovery)
	float RecoveryAmount;

	UPROPERTY(EditAnywhere, Category = Recovery)
	float RecoveryDelay;
};
