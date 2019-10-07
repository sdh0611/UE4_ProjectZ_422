// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "UObject/NoExportTypes.h"

#include "Http.h"
#include "IHttpRequest.h"
#include "IHttpResponse.h"

#include "ZWebConnector.generated.h"

DECLARE_DELEGATE_TwoParams(FOnLoginResponse, bool, const FString&);


/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZWebConnector : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void PostInitProperties() override;


public:
	void HttpPost(FString NewURL, const FString& PostParameter, FHttpRequestCompleteDelegate RequestDelegate);

public:
	void Login(const FString& NewUserID, const FString& NewUserPW, FOnLoginResponse LoginResponse);
	void Logout();
	
public:
	void SetURL(const FString& NewURL);
	
	bool IsVerified() const;
	const FString& GetWebURL() const;
	const FString& GetUserNickname() const;

private:
	void OnLoginResponseReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnLogoutResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

public:
	FOnLoginResponse OnLoginResponse;

private:
	class FHttpModule* Http;

	bool bIsVerified = false;

	FString UserID;

	FString UserNickname;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta=(AllowPrivateAccess=true))
	FString URL = TEXT("127.0.0.1:8000");

};
