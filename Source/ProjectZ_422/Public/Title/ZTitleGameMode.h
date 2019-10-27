// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "ZBaseGameMode.h"

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
	
public:
	void ToLobby(const FString& URL);

protected:
	virtual void BeginPlay() override;
	


};
