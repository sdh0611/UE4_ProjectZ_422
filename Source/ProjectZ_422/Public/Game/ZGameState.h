// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/GameStateBase.h"
#include "ZGameState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AZGameState();

public:
	virtual void Tick(float DeltaTime) override;

public:
	void IncreaseCurrentWave();
	

public:
	int32 GetTotalWave() const;
	int32 GetCurrentWave() const;
	float GetElaspedTime() const;

private:
	UPROPERTY(EditAnywhere, Category = GameState)
	int32 TotalWave;

	UPROPERTY(VisibleAnywhere, Category = GameState)
	int32 CurrentWave;

	UPROPERTY(VisibleAnywhere, Category = GameState)
	float ElapsedTime;

};
