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

	/* 첫 시작시 준비시간 부여 */
	CurrentGameState = ECurrentGameState::HalfTime;
	CurrentRemainTime = HalfTime;
}

void AZGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* Wave 클리어여부 체크 */
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
		/* 킬 수 증가 */
		KillerState->AddKill();
	}

	/* Killer 소지금 증가 */
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
				남은 시간을 HalfTime만큼으로 초기화하고, 상점 오픈 
			*/
			CurrentRemainTime = HalfTime;
			break;
		}
		case ECurrentGameState::WaveTime:
		{
			/* 
				남은 시간을 WaveTime만큼으로 초기화하고, 상점 닫음
				Spawner 활성화 및 Wave 증가
			*/
			CurrentRemainTime = WaveTime;
			auto ZGameState = Cast<AZGameState>(GameState);
			if (nullptr != ZGameState)
			{
				/*
					Wave정보 업데이트
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
