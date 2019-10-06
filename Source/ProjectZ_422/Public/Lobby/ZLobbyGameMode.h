// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/GameModeBase.h"
#include "ZLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;


public:
	void StartGame();

public:
	int32 GetConnectNumber() const;

private:
	int32 ConnectNumber = 0;

};
