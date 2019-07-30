// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/GameModeBase.h"
#include "ZGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AZGameMode();

public:
	virtual void BeginPlay() override;


public:
	virtual void AdjustKillScore(AController* Killer, AController* Victim, APawn* VictimPawn);


};
