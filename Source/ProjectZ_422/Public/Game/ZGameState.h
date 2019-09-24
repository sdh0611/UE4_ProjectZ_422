// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "ZGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "ZGameState.generated.h"

/*
	NOTE:
		게임의 상태를 받아와서 기록만 하는 역할.
		UI Update하는 부분은 추후 프로퍼티 속성에 ReplicatedUsing 설정해서 UI업데이트 시킬 것.
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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


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
	void SetCurrentNumZombies(int32 NewNumZombies);
	void SetCurrentGamePhase(EGamePhase NewPhase);


	/* Getter */
	int32 GetTotalWave() const;
	int32 GetCurrentWave() const;
	int32 GetCurrentNumZombies() const;
	float GetRemainTime() const;


protected:
	/* Replicated using method */
	
	UFUNCTION()
	void OnRep_TotalWave();
	
	UFUNCTION()
	void OnRep_CurrentWave();
	
	UFUNCTION()
	void OnRep_CurrentNumZombies();
	
	UFUNCTION()
	void OnRep_RemainTime();
	
	UFUNCTION()
	void OnRep_CurrentGamePhase();



private:
	UPROPERTY(EditAnywhere, Category = GameState, ReplicatedUsing = OnRep_TotalWave)
	int32 TotalWave;

	UPROPERTY(VisibleAnywhere, Category = GameState, ReplicatedUsing = OnRep_CurrentWave)
	int32 CurrentWave;

	UPROPERTY(VisibleAnywhere, Category = GameState, ReplicatedUsing = OnRep_CurrentNumZombies)
	int32 CurrentNumZombies;

	UPROPERTY(VisibleAnywhere, Category = GameState, ReplicatedUsing = OnRep_RemainTime)
	float RemainTime;

	UPROPERTY(VisibleAnywhere, Category = GameState, ReplicatedUsing = OnRep_CurrentGamePhase)
	EGamePhase CurrentGamePhase;

};
