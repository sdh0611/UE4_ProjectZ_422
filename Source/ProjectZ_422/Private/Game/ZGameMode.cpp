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

	GameModeState = EGameModeState::ReadyToStart;
	HalfTime = 120.f;
	WaveTime = 300.f;
}


void AZGameMode::BeginPlay()
{
	Super::BeginPlay();


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
