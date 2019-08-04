// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/GameModeBase.h"
#include "ZGameMode.generated.h"

UENUM(BlueprintType)
enum class EGameModeState : uint8
{
	ReadyToStart,
	InProgress,
	End
};

UENUM(BlueprintType)
enum class ECurrentGameState: uint8
{
	Ready,
	HalfTime,
	WaveTime,
	Boss,
	End
};

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
	//virtual void StartPlay() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	void AdjustKillScore(AController* Killer, AController* Victim, APawn* VictimPawn);

public:
	ECurrentGameState GetCurrentGameState() const;

protected:
	void HandleCurrentGameState(ECurrentGameState NewCurrentGameState);
	//void CheckTime();
	bool IsGameClear();
	bool IsWaveEnd();

protected:
	UPROPERTY(EditAnywhere, Category = GameMode)
	ECurrentGameState CurrentGameState;

	UPROPERTY(EditAnywhere, Category = GameMode)
	EGameModeState GameModeState;

	/* 전체 Wave */
	UPROPERTY(EditAnywhere, Category = GameState)
	int32 TotalWave;

	/* 현재 Wave */
	UPROPERTY(VisibleAnywhere, Category = GameMode)
	int32 CurrentWave;

	/* Wave사이에 준비시간. -> 초(Second) 단위 */
	UPROPERTY(EditAnywhere, Category = GameMode)
	float HalfTime;

	/* Wave 진행시간. -> 초(Second) 단위 */
	UPROPERTY(EditAnywhere, Category = GameMode)
	float WaveTime;

	/* Game 진행시간. -> 초(Second) 단위 */
	UPROPERTY(VisibleAnywhere, Category = GameMode)
	float CurrentRemainTime;

};
