// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGameMode.h"
#include "ZGameState.h"
#include "ZPlayerState.h"
#include "ZCharacter.h"
#include "ZCharacterItemStatusComponent.h"
#include "ZPlayerController.h"
#include "ZHUD.h"
#include "ZPlayerState.h"

AZGameMode::AZGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = AZCharacter::StaticClass();
	PlayerControllerClass = AZPlayerController::StaticClass();
	HUDClass = AZHUD::StaticClass();
	GameStateClass = AZGameState::StaticClass();
	PlayerStateClass = AZPlayerState::StaticClass();

	CurrentGamePhase = EGamePhase::Ready;
	GameModeState = EGameModeState::ReadyToStart;
	HalfTime = 120.f;
	WaveTime = 300.f;
	CurrentRemainTime = 0.f;
}


void AZGameMode::BeginPlay()
{
	Super::BeginPlay();

	/* ù ���۽� �غ�ð� �ο� */
	CurrentGamePhase = EGamePhase::HalfTime;
	CurrentRemainTime = HalfTime;
	auto ZGameState = Cast<AZGameState>(GameState);
	if (ZGameState)
	{
		OnTimeUpdate.AddUObject(ZGameState, &AZGameState::SetRemainTime);
	}
}

void AZGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentRemainTime -= DeltaTime;

	OnTimeUpdate.Broadcast(CurrentRemainTime);

	/* Wave Ŭ����� üũ */
	if (DeltaTime <= 0.f)
	{
		HandleGamePhase(EGamePhase::HalfTime);
	}

}

void AZGameMode::AdjustKillScore(AController * Killer, AController * Victim, APawn * VictimPawn)
{
	auto KillerState = Killer->GetPlayerState<AZPlayerState>();
	if (KillerState)
	{
		/* ų �� ���� */
		KillerState->AddKill();
	}

	/* Killer ������ ���� */
	auto KillerPawn = Cast<AZCharacter>(Killer->GetPawn());
	if (nullptr == KillerPawn)
	{
		return;
	}
	KillerPawn->GetItemStatusComponent()->AdjustMoney(100);


}

EGamePhase AZGameMode::GetCurrentGamePhase() const
{
	return CurrentGamePhase;
}

void AZGameMode::HandleGamePhase(EGamePhase NewCurrentGameState)
{
	switch (NewCurrentGameState)
	{
		case EGamePhase::HalfTime:
		{
			/* 
				���� �ð��� HalfTime��ŭ���� �ʱ�ȭ�ϰ�, ���� ���� 
			*/
			CurrentRemainTime = HalfTime;
			break;
		}
		case EGamePhase::WaveTime:
		{
			/* 
				���� �ð��� WaveTime��ŭ���� �ʱ�ȭ�ϰ�, ���� ����
				Spawner Ȱ��ȭ �� Wave ����
			*/
			CurrentRemainTime = WaveTime;
			auto ZGameState = Cast<AZGameState>(GameState);
			if (nullptr != ZGameState)
			{
				/*
					Wave���� ������Ʈ
				*/
				ZGameState->IncreaseCurrentWave();

			}

			break;
		}



	}

	CurrentGamePhase= NewCurrentGameState;


}

bool AZGameMode::IsGameClear()
{
	return CurrentWave > TotalWave;
}

bool AZGameMode::IsWaveEnd()
{
	if (EGamePhase::WaveTime != GetCurrentGamePhase())
	{
		return false;
	}

	return (CurrentRemainTime <= 0.f);
}

float AZGameMode::GetCurrentRemainTime() const
{
	return CurrentRemainTime;
}
