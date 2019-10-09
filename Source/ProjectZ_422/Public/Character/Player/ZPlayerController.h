// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "ZBasePlayerController.h"
#include "ZPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZPlayerController : public AZBasePlayerController
{
	GENERATED_BODY()
	
public:
	AZPlayerController();

public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	void ConstructShopWidget();
	void AddItemToInventoryWidget(class AZItem* const NewItem) ;
	void AddItemToWeaponInventoryWidget(class AZWeapon* const NewWeapon) ;
	void AddItemToSellWidget(class AZItem* const NewItem);
	void FadeBloodSplatter();

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
	void ServerCheckIsShopOpen();
	bool ServerCheckIsShopOpen_Validate();
	void ServerCheckIsShopOpen_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBuy(int32 BuyItemShopID, int32 Quantity);
	bool ServerBuy_Validate(int32 BuyItemShopID, int32 Quantity);
	void ServerBuy_Implementation(int32 BuyItemShopID, int32 Quantity);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSell(int32 SellItemInventoryIndex, int32 Quantity);
	bool ServerSell_Validate(int32 SellItemInventoryIndex, int32 Quantity);
	void ServerSell_Implementation(int32 SellItemInventoryIndex, int32 Quantity);

	/* From server to client */
	UFUNCTION(Client, Reliable, WithValidation)
	void ClientOpenShop();
	bool ClientOpenShop_Validate();
	void ClientOpenShop_Implementation();

	UFUNCTION(Client, Reliable, WithValidation)
	void CloseShop();
	bool CloseShop_Validate();
	void CloseShop_Implementation();

	UFUNCTION(Client, Reliable, WithValidation)
	void ClientBloodSplatter();
	bool ClientBloodSplatter_Validate();
	void ClientBloodSplatter_Implementation();

	UFUNCTION(Client, Reliable, WithValidation)
	void ClientAddPitchAndYaw(float AddPitch, float AddYaw);
	bool ClientAddPitchAndYaw_Validate(float AddPitch, float AddYaw);
	void ClientAddPitchAndYaw_Implementation(float AddPitch, float AddYaw);

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
