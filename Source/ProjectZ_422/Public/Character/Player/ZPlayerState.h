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

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetPlayerName(const FString& S) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_PlayerName() override;


public:
	void AddKill();
	void AdjustScore(int32 NewScore);
	void SetPlayerIndex(int32 NewIndex);


private:
	UPROPERTY(Replicated)
	int32 PlayerIndex = 0;

	//UPROPERTY(Replicated)
	int32 Kills;

	//UPROPERTY(Replicated)
	int32 CurScore;
};
