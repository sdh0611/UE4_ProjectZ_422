// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/GameModeBase.h"
#include "Http.h"
#include "IHttpRequest.h"
#include "IHttpResponse.h"
#include "ZTitleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZTitleGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
public:
	void HttpPost(const FString& URL, const FString& UserID, const FString& UserPW, FHttpRequestCompleteDelegate RequestDelegate);


protected:
	class FHttpModule* Http;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString HostName = TEXT("127.0.0.1:8000");

};
