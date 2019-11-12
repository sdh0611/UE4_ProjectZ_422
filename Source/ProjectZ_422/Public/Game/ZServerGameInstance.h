// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Game/ZGameInstance.h"
#include "ZServerGameInstance.generated.h"

/**
 *	NOTE(11.08):
		AWS Server¿ë GameInstance.
 */
UCLASS()
class PROJECTZ_422_API UZServerGameInstance : public UZGameInstance
{
	GENERATED_BODY()
	
public:
	UZServerGameInstance();

public:
	virtual void Init() override;
	virtual void Shutdown() override;

public:
	void TerminateSession();
	void ProcessEnd();
	void RemovePlayerSession(const FString& PlayerSessionID);
	void AcceptPlayerSession(const FString& PlayerSessionID);

public:
	bool bIsHealthyProcess = true;

protected:
	class FGameLiftServerSDKModule* GameLiftSDKModule;



};
