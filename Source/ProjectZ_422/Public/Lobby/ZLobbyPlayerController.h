// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "ZBasePlayerController.h"
#include "ZLobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZLobbyPlayerController : public AZBasePlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

public:
	void UpdateConnectNumber(int32 NewNumber);
	void UpdatePlayerName(int32 PlayerIndex, const FString & PlayerName);
	

public:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReceiveChat(const FString& PlayerName, const FString& RecvChat);
	bool ServerReceiveChat_Validate(const FString& PlayerName, const FString& RecvChat);
	void ServerReceiveChat_Implementation(const FString& PlayerName, const FString& RecvChat);


	UFUNCTION(Client, Reliable, WithValidation)
	void ClientReceiveChat(const FString& PlayerName, const FString& RecvChat);
	bool ClientReceiveChat_Validate(const FString& PlayerName, const FString& RecvChat);
	void ClientReceiveChat_Implementation(const FString& PlayerName, const FString& RecvChat);
	

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UZLobbyWidget> UserHUDClass;

	
protected:
	UPROPERTY(BlueprintReadOnly)
	class UZLobbyWidget* UserHUD;

	
};
