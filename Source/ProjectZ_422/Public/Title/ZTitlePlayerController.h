// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/PlayerController.h"
#include "Http.h"
#include "IHttpRequest.h"
#include "IHttpResponse.h"
#include "ZTitlePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZTitlePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

public:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRequestLogin(const FString& URL, const FString& UserID, const FString& UserPW);
	bool ServerRequestLogin_Validate(const FString& URL, const FString& UserID, const FString& UserPW);
	void ServerRequestLogin_Implementation(const FString& URL, const FString& UserID, const FString& UserPW);


protected:
	void OnLoginServerResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

protected:
	UFUNCTION(Client, Reliable, WithValidation)
	void ClientReceiveLoginSuccess(const FString& NewNickname);
	bool ClientReceiveLoginSuccess_Validate(const FString& NewNickname);
	void ClientReceiveLoginSuccess_Implementation(const FString& NewNickname);

	UFUNCTION(Client, Reliable, WithValidation)
	void ClientRecevieLoginFail();
	bool ClientRecevieLoginFail_Validate();
	void ClientRecevieLoginFail_Implementation();


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UZTitleWidget> TitleWidgetClass;

	UPROPERTY()
	class UZTitleWidget* TitleWidget;


};
