// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/GameStateBase.h"
#include "ZGameState.generated.h"

/*
	NOTE:
		게임의 상태를 받아와서 기록만 하는 역할.
		UI Update하는 부분은 추후 Server에서 RPC로 업데이트시킬 것.
*/

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
	/*
		현재 Zombie 개체수 조정.
		기본 동작은 덧셈
	*/
	void AdjustCurrentNumZombies(int32 NewValue);
	void UpdateRemainTime(float NewRemainTime);


public:
	/* Setter */
	void SetTotalWave(int32 NewTotalWave);
	void SetCurrentWave(int32 NewCurrentWave);

	/* Getter */
	int32 GetTotalWave() const;
	int32 GetCurrentWave() const;
	int32 GetCurrentNumZombies() const;
	float GetRemainTime() const;
	float GetElaspedTime() const;

private:
	UPROPERTY(EditAnywhere, Category = GameState)
	int32 TotalWave;

	UPROPERTY(VisibleAnywhere, Category = GameState)
	int32 CurrentWave;

	UPROPERTY(VisibleAnywhere, Category = GameState)
	int32 CurrentNumZombies;

	UPROPERTY(VisibleAnywhere, Category = GameState)
	float RemainTime;

	UPROPERTY(VisibleAnywhere, Category = GameState)
	float ElapsedTime;

};
