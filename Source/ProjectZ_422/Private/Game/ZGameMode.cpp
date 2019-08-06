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

}

void AZGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateGameTime(DeltaTime);


	//CurrentRemainTime -= DeltaTime;

	//OnTimeUpdate.Broadcast(CurrentRemainTime);

	/* Wave Ŭ����� üũ */
	//if (CurrentRemainTime <= 0.f)
	//{
	//	HandleGamePhase(EGamePhase::HalfTime);
	//}

}

void AZGameMode::AdjustKillScore(AController * Killer, AController * Victim, APawn * VictimPawn)
{
	auto KillerState = Killer->GetPlayerState<AZPlayerState>();
	if (KillerState)
	{
		/* ų �� ���� */
		KillerState->AddKill();
	}

	/* ���� ��ü�� ���� */
	auto ZGameState = Cast<AZGameState>(GameState);
	if(ZGameState)
	{
		ZGameState->AdjustCurrentNumZombies(-1);
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

void AZGameMode::UpdateGameTime(float DeltaTime)
{
	CurrentRemainTime -= DeltaTime;

	//OnTimeUpdate.Broadcast(CurrentRemainTime);

	if (CurrentRemainTime <= 0.f)
	{
		switch (CurrentGamePhase)
		{
			case EGamePhase::HalfTime:
			{
				/* WaveTime Phase�� ��ȯ */
				HandleGamePhase(EGamePhase::WaveTime);
				break;
			}
			case EGamePhase::WaveTime:
			{
				/* Wave üũ �� Wave 10 �̸��̸� HalfTime���� ��ȯ, Wave 10�̸� Boss�� ��ȯ */
				if (CurrentWave <= TotalWave)
				{
					HandleGamePhase(EGamePhase::HalfTime);
				}
				else
				{
					HandleGamePhase(EGamePhase::Boss);
				}
				break;
			}
			case EGamePhase::Boss:
			{
				/* Clear���� üũ */
				

				break;
			}

		}
	}

	/* GameState�� �ð� ������Ʈ */
	auto ZGameState = Cast<AZGameState>(GameState);
	if (ZGameState)
	{
		ZGameState->UpdateRemainTime(CurrentRemainTime);
	}

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
