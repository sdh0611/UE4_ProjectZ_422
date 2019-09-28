// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "ZItem.h"
#include "ZUsableItemInterface.h"
#include "ZRecovery.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZRecovery : public AZItem, public IZUsableItemInterface
{
	GENERATED_BODY()
	
public:
	AZRecovery();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void InitItemData(const struct FZItemData* NewItemData) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnUsed() override;

private:
	/* From client to server RPC */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUseItem();
	bool ServerUseItem_Validate();
	void ServerUseItem_Implementation();
	

	/* From client to server RPC */




private:
	UPROPERTY(EditAnywhere, Category = Recovery, Replicated)
	float RecoveryAmount;

	UPROPERTY(EditAnywhere, Category = Recovery, Replicated)
	float RecoveryDelay;

};
