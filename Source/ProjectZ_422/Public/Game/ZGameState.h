// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/GameStateBase.h"
#include "ZGameState.generated.h"

/*
	NOTE:
		������ ���¸� �޾ƿͼ� ��ϸ� �ϴ� ����.
		UI Update�ϴ� �κ��� ���� Server���� RPC�� ������Ʈ��ų ��.
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
		���� Zombie ��ü�� ����.
		�⺻ ������ ����
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
