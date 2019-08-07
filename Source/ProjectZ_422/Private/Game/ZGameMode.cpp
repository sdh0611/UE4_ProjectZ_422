// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGameMode.h"
#include "ZGameState.h"
#include "ZPlayerState.h"
#include "ZCharacter.h"
#include "ZEnemySpawner.h"
#include "ZCharacterItemStatusComponent.h"
#include "ZPlayerController.h"
#include "ZHUD.h"
#include "ZPlayerState.h"
#include "EngineUtils.h"


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
	HalfTime = 10.f;
	WaveTime = 300.f;
	CurrentRemainTime = 0.f;
}


void AZGameMode::BeginPlay()
{
	Super::BeginPlay();

	/* 첫 시작시 준비시간 부여 */
	CurrentGamePhase = EGamePhase::HalfTime;
	CurrentRemainTime = HalfTime;
	auto MyGameState = GetGameState<AZGameState>();

	/* Level내의 Spawner 저장 */
	for (TActorIterator<AZEnemySpawner> Iterator(GetWorld()); Iterator; ++Iterator)
	{
		EnemySpawners.Add(*Iterator);
	}

}

void AZGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateGameTime(DeltaTime);


	//CurrentRemainTime -= DeltaTime;

	//OnTimeUpdate.Broadcast(CurrentRemainTime);

	/* Wave 클리어여부 체크 */
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
		/* 킬 수 증가 */
		KillerState->AddKill();
	}

	/* 좀비 개체수 조정 */
	auto ZGameState = Cast<AZGameState>(GameState);
	if(ZGameState)
	{
		ZGameState->AdjustCurrentNumZombies(-1);
	}

	/* Killer 소지금 증가 */
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
				/* WaveTime Phase로 전환 */
				HandleGamePhase(EGamePhase::WaveTime);
				break;
			}
			case EGamePhase::WaveTime:
			{
				/* Wave 체크 후 Wave 10 미만이면 HalfTime으로 전환, Wave 10이면 Boss로 전환 */
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
				/* Clear여부 체크 */
				

				break;
			}

		}
	}

	/* GameState에 시간 업데이트 */
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
			/* Spawner 비활성화 */
			for (const auto& Spawner : EnemySpawners)
			{
				Spawner->SetActive(false);
			}

			/* 남은 시간을 HalfTime만큼으로 초기화하고, 상점 오픈 */
			CurrentRemainTime = HalfTime;
			break;
		}
		case EGamePhase::WaveTime:
		{
			/* 
				남은 시간을 WaveTime만큼으로 초기화하고, 상점 닫음
				Spawner 활성화 및 Wave 증가
			*/
			CurrentRemainTime = WaveTime;
			auto ZGameState = Cast<AZGameState>(GameState);
			if (nullptr != ZGameState)
			{
				/*	Wave정보 업데이트 */
				ZGameState->IncreaseCurrentWave();
				for (const auto& Spawner : EnemySpawners)
				{
					Spawner->SetActive(false);
				}
			}

			/* Spawner 활성화 */
			for (const auto& Spawner : EnemySpawners)
			{
				Spawner->SetActive(true);
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
