// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/PlayerState.h"
#include "ZPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AZPlayerState();

public:
	void AddKill();
	void AdjustScore(int32 NewScore);
	
private:
	int32 Kills;

	int32 CurScore;
};
