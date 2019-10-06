// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/PlayerController.h"

#include "Http.h"
#include "IHttpRequest.h"
#include "IHttpResponse.h"

#include "ZBasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZBasePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void OnPossess(APawn* InPawn) override;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(Client, Reliable, WithValidation)
	void ClientRequestLogout();
	bool ClientRequestLogout_Validate();
	void ClientRequestLogout_Implementation();

	UFUNCTION(Client, Reliable, WithValidation)
	void ClientReceiveSetUserName();
	bool ClientReceiveSetUserName_Validate();
	void ClientReceiveSetUserName_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReceiveSetUserName(const FString& NewName);
	bool ServerReceiveSetUserName_Validate(const FString& NewName);
	void ServerReceiveSetUserName_Implementation(const FString& NewName);


private:
	void OnLogoutResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


};
