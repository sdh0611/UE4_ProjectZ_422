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
enum class EGamePhase: uint8
{
	Ready,
	HalfTime,
	WaveTime,
	Boss,
	End,
	Win,
	Lose
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
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	void AdjustKillScore(AController* Killer, AController* Victim, APawn* VictimPawn);

public:
	void UpdateGameTime(float DeltaTime);
	void HandleGamePhase(EGamePhase NewCurrentGameState);
	void AdjustZombieNum(int32 Value);

public:
	EGamePhase GetCurrentGamePhase() const;

protected:
	void StopAllSpawner();

	/* Getter */
	bool IsGameEnd();
	bool IsGameClear();
	bool IsWaveEnd();
	float GetCurrentRemainTime() const;

protected:
	FTimerHandle StopSpawnTimer;

	UPROPERTY(EditAnywhere, Category = GameMode)
	EGameModeState GameModeState;

	/*
		���� ������.
		GameState�� ����ϴ� ������Ƽ.
	*/
	UPROPERTY(EditAnywhere, Category = GameMode)
	EGamePhase CurrentGamePhase;

	/* 
		��ü Wave. 
		GameState�� ����ϴ� ������Ƽ.
	*/
	UPROPERTY(EditAnywhere, Category = GameMode)
	int32 TotalWave;

	/* 
		���� Wave. 
		GameState�� ����ϴ� ������Ƽ.
	*/
	UPROPERTY(VisibleAnywhere, Category = GameMode)
	int32 CurrentWave;

	/* 
		���� ������ ��.
		GameState�� ����ϴ� ������Ƽ.
	*/
	UPROPERTY(VisibleAnywhere, Category = GameState)
	int32 CurrentNumZombies;

	/* Wave���̿� �غ�ð�. -> ��(Second) ���� */
	UPROPERTY(EditAnywhere, Category = GameMode)
	float HalfTime;

	/* Wave ����ð�. -> ��(Second) ���� */
	UPROPERTY(EditAnywhere, Category = GameMode)
	float WaveTime;

	/*
		Game ����ð�. -> ��(Second) ���� 
		GameState�� ����ϴ� ������Ƽ.
	*/
	UPROPERTY(VisibleAnywhere, Category = GameMode)
	float CurrentRemainTime;

	/* ���� ���� �̸� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameMode)
	FName CurrentLevelName;

	UPROPERTY()
	TArray<class AZEnemySpawner*> EnemySpawners;

};
