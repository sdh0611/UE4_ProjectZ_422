// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "ZBaseGameMode.h"
#include "ZLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZLobbyGameMode : public AZBaseGameMode
{
	GENERATED_BODY()
	
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;


public:
	void StartGame();

public:
	int32 GetConnectNumber() const;

private:
	int32 ConnectNumber = 0;

};
