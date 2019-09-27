// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/PlayerController.h"
#include "ZPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AZPlayerController();

public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	void OpenShop();
	void ConstructShopWidget();

public:
	/*
		UserHUD Class�� ��ȯ�ϴ� �޼ҵ�.
		Inventory���� ������ UserHUD �����͸� �޾Ƽ� ���� ������.
		@ Return : ZUserHUD class pointer
	*/
	class UZUserHUD* const GetUserHUD() const;

	class AZHUD* const GetZHUD() const;

	class UZCharacterItemStatusComponent* const GetCharacterItemStatusComponent() const;

public:
	/* From client to server  */
	UFUNCTION(Server, Reliable, WithValidation)
	void Buy(int32 BuyItemShopID, int32 Quantity);
	bool Buy_Validate(int32 BuyItemShopID, int32 Quantity);
	void Buy_Implementation(int32 BuyItemShopID, int32 Quantity);

	/* From server to client */
	UFUNCTION(Server, Reliable, WithValidation)
	void Sell(int32 SellItemInventoryIndex, int32 Quantity);
	bool Sell_Validate(int32 SellItemInventoryIndex, int32 Quantity);
	void Sell_Implementation(int32 SellItemInventoryIndex, int32 Quantity);


	/* Replicated using */



protected:
	void ToggleInventory();
	void ToggleInGameMenu();
	void RemoveWidgetFromTop();

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UZUserHUD> UserHUDClass;

private:
	UPROPERTY()
	class UZUserHUD* UserHUD;



};
