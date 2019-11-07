// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/GameModeBase.h"
#include "ZBaseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AZBaseGameMode();

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:
	void UpdatePlayersName();
	

public:
	int32 GetConnectNumber() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ConnectNumber = 0;

	class FGameLiftServerSDKModule* GameLiftSDKModule;

};
