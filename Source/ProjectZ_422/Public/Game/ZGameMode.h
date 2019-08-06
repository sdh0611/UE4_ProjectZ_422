// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/GameModeBase.h"
#include "ZGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimeUpdate, float);

UENUM(BlueprintType)
enum class EGameModeState : uint8
{
	ReadyToStart,
	InProgress,
	End
};

UENUM(BlueprintType)
enum class EGamePhase: uint8
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
	void HandleGamePhase(EGamePhase NewCurrentGameState);

protected:
	//void CheckTime();
	bool IsGameClear();
	bool IsWaveEnd();
	float GetCurrentRemainTime() const;
	EGamePhase GetCurrentGamePhase() const;

public:
	FOnTimeUpdate OnTimeUpdate;

protected:
	UPROPERTY(EditAnywhere, Category = GameMode)
	EGamePhase CurrentGamePhase;

	UPROPERTY(EditAnywhere, Category = GameMode)
	EGameModeState GameModeState;

	/* ��ü Wave */
	UPROPERTY(EditAnywhere, Category = GameState)
	int32 TotalWave;

	/* ���� Wave */
	UPROPERTY(VisibleAnywhere, Category = GameMode)
	int32 CurrentWave;

	/* Wave���̿� �غ�ð�. -> ��(Second) ���� */
	UPROPERTY(EditAnywhere, Category = GameMode)
	float HalfTime;

	/* Wave ����ð�. -> ��(Second) ���� */
	UPROPERTY(EditAnywhere, Category = GameMode)
	float WaveTime;

	/* Game ����ð�. -> ��(Second) ���� */
	UPROPERTY(VisibleAnywhere, Category = GameMode)
	float CurrentRemainTime;

};
