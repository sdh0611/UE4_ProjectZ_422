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
	void ConstructShopWidget();
	void AddItemToInventoryWidget(class AZItem* const NewItem) ;
	void AddItemToSellWidget(class AZItem* const NewItem);
	void FadeBloodSplatter();

public:
	/*
		UserHUD Class를 반환하는 메소드.
		Inventory등의 조작은 UserHUD 포인터를 받아서 직접 조작함.
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
	void Buy(int32 BuyItemShopID, int32 Quantity);
	bool Buy_Validate(int32 BuyItemShopID, int32 Quantity);
	void Buy_Implementation(int32 BuyItemShopID, int32 Quantity);

	UFUNCTION(Server, Reliable, WithValidation)
	void Sell(int32 SellItemInventoryIndex, int32 Quantity);
	bool Sell_Validate(int32 SellItemInventoryIndex, int32 Quantity);
	void Sell_Implementation(int32 SellItemInventoryIndex, int32 Quantity);

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
