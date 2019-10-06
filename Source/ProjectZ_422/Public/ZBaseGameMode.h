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
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	

};
