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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PreClientTravel(const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel) override;

public:
	//void RemoveAllWidget();
	//void ShowLoadingWidget(bool bShow);

public:
	UFUNCTION(Client, Reliable, WithValidation)
	void ClientReceiveGetUserName();
	bool ClientReceiveGetUserName_Validate();
	void ClientReceiveGetUserName_Implementation();

	UFUNCTION(Client, Reliable, WithValidation)
	void ClientRemoveAllWidget();
	bool ClientRemoveAllWidget_Validate();
	void ClientRemoveAllWidget_Implementation();

	UFUNCTION(Client, Reliable, WithValidation)
	void ClientReceiveRemovePlayerSession();
	bool ClientReceiveRemovePlayerSession_Validate();
	void ClientReceiveRemovePlayerSession_Implementation();

	UFUNCTION(Client, Reliable, WithValidation)
	void ClientReceiveAcceptPlayerSession();
	bool ClientReceiveAcceptPlayerSession_Validate();
	void ClientReceiveAcceptPlayerSession_Implementation();


	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReceiveUserName(const FString& UserName);
	bool ServerReceiveUserName_Validate(const FString& UserName);
	void ServerReceiveUserName_Implementation(const FString& UserName);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRemovePlayerSession(const FString& PlayerSessionID);
	bool ServerRemovePlayerSession_Validate(const FString& PlayerSessionID);
	void ServerRemovePlayerSession_Implementation(const FString& PlayerSessionID);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAcceptPlayerSession(const FString& PlayerSessionID);
	bool ServerAcceptPlayerSession_Validate(const FString& PlayerSessionID);
	void ServerAcceptPlayerSession_Implementation(const FString& PlayerSessionID);


	

protected:
	virtual void OnReceiveUserName(const FString& UserName);

protected:
	virtual void BeginPlay() override;

//private:
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta=(AllowPrivateAccess=true))
//	TSubclassOf<class UUserWidget> LoadingWidgetClass;
//	
//	UPROPERTY()
//	class UUserWidget* LoadingWidget;
//
};
