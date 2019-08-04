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
	DefaultPawnClass = AZCharacter::StaticClass();
	PlayerControllerClass = AZPlayerController::StaticClass();
	HUDClass = AZHUD::StaticClass();
	GameStateClass = AZGameState::StaticClass();
	PlayerStateClass = AZPlayerState::StaticClass();

	CurrentGameState = ECurrentGameState::Ready;
	GameModeState = EGameModeState::ReadyToStart;
	HalfTime = 120.f;
	WaveTime = 300.f;
	CurrentRemainTime = 0.f;
}


void AZGameMode::BeginPlay()
{
	Super::BeginPlay();

	/* ù ���۽� �غ�ð� �ο� */
	CurrentGameState = ECurrentGameState::HalfTime;
	CurrentRemainTime = HalfTime;
}

void AZGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* Wave Ŭ����� üũ */
	CurrentRemainTime -= DeltaTime;
	if (DeltaTime <= 0.f)
	{
		HandleCurrentGameState(ECurrentGameState::HalfTime);
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

ECurrentGameState AZGameMode::GetCurrentGameState() const
{
	return CurrentGameState;
}

void AZGameMode::HandleCurrentGameState(ECurrentGameState NewCurrentGameState)
{
	switch (NewCurrentGameState)
	{
		case ECurrentGameState::HalfTime:
		{
			/* 
				���� �ð��� HalfTime��ŭ���� �ʱ�ȭ�ϰ�, ���� ���� 
			*/
			CurrentRemainTime = HalfTime;
			break;
		}
		case ECurrentGameState::WaveTime:
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

	CurrentGameState = NewCurrentGameState;


}

bool AZGameMode::IsGameClear()
{
	return CurrentWave > TotalWave;
}

bool AZGameMode::IsWaveEnd()
{
	if (ECurrentGameState::WaveTime != GetCurrentGameState())
	{
		return false;
	}

	return (CurrentRemainTime <= 0.f);
}
