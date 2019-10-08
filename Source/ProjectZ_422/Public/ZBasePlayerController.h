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

public:
	UFUNCTION(Client, Reliable, WithValidation)
	void ClientReceiveGetUserName();
	bool ClientReceiveGetUserName_Validate();
	void ClientReceiveGetUserName_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReceiveUserName(const FString& UserName);
	bool ServerReceiveUserName_Validate(const FString& UserName);
	void ServerReceiveUserName_Implementation(const FString& UserName);
	
protected:
	virtual void OnReceiveUserName(const FString& UserName);

protected:
	virtual void BeginPlay() override;



};
