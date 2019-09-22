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
	void UpdateGamePhase();
	void HandleGamePhase(EGamePhase NewCurrentGameState);
	void AdjustZombieNum(int32 Value);

public:
	EGamePhase GetCurrentGamePhase() const;

protected:
	void StopAllSpawner();

	/* PhaseTimer 바인딩용 */
	void UpdateGameTime();

	/* Getter */
	bool IsGameEnd();
	bool IsGameClear();
	bool IsWaveEnd();
	float GetCurrentRemainTime() const;


protected:
	FTimerHandle PhaseTimer;

	FTimerHandle StopSpawnTimer;

	UPROPERTY(EditAnywhere, Category = GameMode)
	EGameModeState GameModeState;

	/*
		현재 페이즈.
		GameState에 기록하는 프로퍼티.
	*/
	UPROPERTY(EditAnywhere, Category = GameMode)
	EGamePhase CurrentGamePhase;

	/* 
		전체 Wave. 
		GameState에 기록하는 프로퍼티.
	*/
	UPROPERTY(EditAnywhere, Category = GameMode)
	int32 TotalWave;

	/* 
		현재 Wave. 
		GameState에 기록하는 프로퍼티.
	*/
	UPROPERTY(VisibleAnywhere, Category = GameMode)
	int32 CurrentWave;

	/* 
		현재 좀비의 수.
		GameState에 기록하는 프로퍼티.
	*/
	UPROPERTY(VisibleAnywhere, Category = GameState)
	int32 CurrentNumZombies;

	/* Wave사이에 준비시간. -> 초(Second) 단위 */
	UPROPERTY(EditAnywhere, Category = GameMode)
	float HalfTime;

	/* Wave 진행시간. -> 초(Second) 단위 */
	UPROPERTY(EditAnywhere, Category = GameMode)
	float WaveTime;

	/*
		Game 진행시간. -> 초(Second) 단위 
		GameState에 기록하는 프로퍼티.
	*/
	UPROPERTY(VisibleAnywhere, Category = GameMode)
	float CurrentRemainTime;

	/* 현재 레벨 이름 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameMode)
	FName CurrentLevelName;

	UPROPERTY()
	TArray<class AZEnemySpawner*> EnemySpawners;

};
