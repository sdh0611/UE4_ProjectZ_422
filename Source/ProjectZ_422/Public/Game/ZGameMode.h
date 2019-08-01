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
}

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
	virtual void StartPlay() override;


public:
	virtual void AdjustKillScore(AController* Killer, AController* Victim, APawn* VictimPawn);


protected:
	UPROPERTY(VisibleAnywhere, Category = GameMode)
	EGameModeState GameModeState;

	/* Wave���̿� �غ�ð�. -> ��(Second) ���� */
	UPROPERTY(EditAnywhere, Category = GameMode)
	float HalfTime;

	/* Wave ����ð�. -> ��(Second) ���� */
	UPROPERTY(EditAnywhere, Category = GameMode)
	float WaveTime;



};
