// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGameMode.h"
#include "ZGameState.h"
#include "ZPlayerState.h"
#include "ZCharacter.h"
#include "ZBaseZombie.h"
#include "ZShop.h"
#include "ZEnemySpawner.h"
#include "ZCharacterItemStatusComponent.h"
#include "ZPlayerController.h"
#include "ZHUD.h"
#include "ZUserHUD.h"
#include "ZPlayerState.h"
#include "EngineUtils.h"
#include "TimerManager.h"

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

	CurrentWave = 0;

	HalfTime = 10.f;
	WaveTime = 300.f;
	CurrentRemainTime = 0.f;
}


void AZGameMode::InitGame(const FString & MapName, const FString & Options, FString & ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	CurrentLevelName = *MapName;
}

void AZGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

}

void AZGameMode::BeginPlay()
{
	Super::BeginPlay();

	/* 첫 시작시 준비시간 부여 */
	CurrentGamePhase = EGamePhase::HalfTime;
	CurrentRemainTime = HalfTime;

	auto MyGameState = GetGameState<AZGameState>();
	check(nullptr != MyGameState);
	MyGameState->SetTotalWave(TotalWave);

	/* Level내의 Spawner 저장 */
	for (TActorIterator<AZEnemySpawner> Iterator(GetWorld()); Iterator; ++Iterator)
	{
		EnemySpawners.Add(*Iterator);
	}

	/* 처음은 HalfTime으로 시작 */
	HandleGamePhase(EGamePhase::HalfTime);
}

void AZGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//if (!IsGameEnd())
	//{
	//	CurrentRemainTime -= DeltaTime;
	//	if (CurrentRemainTime <= 0.f)
	//	{
	//		UpdateGamePhase();
	//	}
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

	auto Zombie = Cast<AZBaseZombie>(VictimPawn);
	if (nullptr == Zombie)
	{
		return;
	}

	/* 좀비 개체수 조정 */
	AdjustZombieNum(-1);

	/* Killer 소지금 증가 */
	auto KillerPawn = Cast<AZCharacter>(Killer->GetPawn());
	if (nullptr == KillerPawn)
	{
		return;
	}

	KillerPawn->GetItemStatusComponent()->AdjustMoney(Zombie->GetZombieValue());
	KillerState->AdjustScore(Zombie->GetZombieScore());

}

EGamePhase AZGameMode::GetCurrentGamePhase() const
{
	return CurrentGamePhase;
}

void AZGameMode::StopAllSpawner()
{
	for (const auto& Spawner : EnemySpawners)
	{
		Spawner->SetActive(false);
	}

}

void AZGameMode::UpdateGamePhase()
{
	switch (CurrentGamePhase)
	{
		case EGamePhase::HalfTime:
		{
			/* WaveTime Phase로 전환 */
			ZLOG(Error, TEXT("Turn wave time."));
			HandleGamePhase(EGamePhase::WaveTime);
			break;
		}
		case EGamePhase::WaveTime:
		{
			/* Wave 체크 후 Wave 10 미만이면 HalfTime으로 전환, Wave 10이면 Boss로 전환 */
			if (CurrentWave < TotalWave)
			{
				if (IsWaveEnd())
				{
					ZLOG(Error, TEXT("Turn half time."));
					HandleGamePhase(EGamePhase::HalfTime);
				}
			}
			else
			{
				if (IsGameClear())
				{
					ZLOG(Error, TEXT("Turn win."));
					HandleGamePhase(EGamePhase::Win);
				}
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

void AZGameMode::HandleGamePhase(EGamePhase NewCurrentGamePhase)
{
	switch (NewCurrentGamePhase)
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
			for (TActorIterator<AZShop> Shop(GetWorld()); Shop; ++Shop)
			{
				Shop->OpenShop();
			}

			/* Phase timer 등록 */
			GetWorld()->GetTimerManager().SetTimer(PhaseTimer, this, &AZGameMode::UpdateGameTime, 0.1f, true);

			break;
		}
		case EGamePhase::WaveTime:
		{
			/*
				남은 시간을 WaveTime만큼으로 초기화하고, 상점 닫음
				Spawner 활성화 및 Wave 증가
			*/
			CurrentRemainTime = WaveTime;
			++CurrentWave;

			/*	GameState 업데이트.	*/
			auto ZGameState = Cast<AZGameState>(GameState);
			if (nullptr != ZGameState)
			{
				/*	Wave정보 업데이트 */
				ZGameState->SetCurrentWave(CurrentWave);
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

			/* 상점 닫음 */
			for (TActorIterator<AZShop> Shop(GetWorld()); Shop; ++Shop)
			{
				Shop->CloseShop();
			}

			/* Stop spawner timer 등록 */
			GetWorld()->GetTimerManager().SetTimer(StopSpawnTimer, this, &AZGameMode::StopAllSpawner, CurrentRemainTime);

			/* Phase timer 등록 */
			GetWorld()->GetTimerManager().SetTimer(PhaseTimer, this, &AZGameMode::UpdateGameTime, 0.1f, true);

			break;
		}
		case EGamePhase::Win:
		{
			ZLOG(Error, TEXT("Win."));
			for (const auto& Spawner : EnemySpawners)
			{
				Spawner->SetActive(false);
			}

			break;
		}
		case EGamePhase::Lose:
		{
			ZLOG(Error, TEXT("Lose."));
			for (const auto& Spawner : EnemySpawners)
			{
				Spawner->SetActive(false);
			}

			break;
		}

	}

	/* Phase text 업데이트 */
	auto MyGameState = GetGameState<AZGameState>();
	if (MyGameState)
	{
		MyGameState->SetCurrentGamePhase(NewCurrentGamePhase);
	}


	CurrentGamePhase = NewCurrentGamePhase;


}

void AZGameMode::AdjustZombieNum(int32 Value)
{
	CurrentNumZombies += Value;
	if (CurrentNumZombies < 0)
	{
		CurrentNumZombies = 0;
	}

	auto MyGameState = GetGameState<AZGameState>();
	if (MyGameState)
	{
		MyGameState->SetCurrentNumZombies(CurrentNumZombies);
	}

	if (IsWaveEnd())
	{
		UpdateGamePhase();
	}
}

bool AZGameMode::IsGameEnd()
{
	return (CurrentGamePhase == EGamePhase::Win || CurrentGamePhase == EGamePhase::Lose);
}

bool AZGameMode::IsGameClear()
{
	if ((CurrentWave >= TotalWave) && (CurrentNumZombies < 1))
	{
		return true;
	}

	return false;
}

bool AZGameMode::IsWaveEnd()
{
	if (CurrentRemainTime > 0.f)
	{
		return false;
	}

	if (CurrentNumZombies > 0)
	{
		return false;
	}

	return true;
}

float AZGameMode::GetCurrentRemainTime() const
{
	return CurrentRemainTime;
}

void AZGameMode::UpdateGameTime()
{
	CurrentRemainTime -= 0.1f;

	/* GameState에 시간 업데이트 */
	auto ZGameState = Cast<AZGameState>(GameState);
	if (ZGameState)
	{
		ZGameState->UpdateRemainTime(CurrentRemainTime);
	}

	if (CurrentRemainTime <= 0.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(PhaseTimer);

		UpdateGamePhase();
		
	}
	
}
