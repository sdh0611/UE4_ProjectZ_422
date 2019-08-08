// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGameState.h"
#include "ZPlayerController.h"
#include "ZHUD.h"
#include "ZUserHUD.h"
#include "Engine/World.h"

AZGameState::AZGameState()
{
	TotalWave = 10;
	CurrentWave = 0;
	CurrentNumZombies = 0;
	RemainTime = 0.f;
	ElapsedTime = 0.f;
}

void AZGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AZGameState::IncreaseCurrentWave()
{
	++CurrentWave;

	for (auto Controller = GetWorld()->GetPlayerControllerIterator(); Controller; ++Controller)
	{
		auto PC = Cast<AZPlayerController>(Controller->Get());
		if (PC)
		{
			PC->GetZHUD()->GetUserHUD()->UpdateCurrentWave(CurrentWave);
		}
	}

}

void AZGameState::AdjustCurrentNumZombies(int32 NewValue)
{
	CurrentNumZombies += NewValue;
	if (CurrentNumZombies < 0)
	{
		CurrentNumZombies = 0;
	}

	for (auto Controller = GetWorld()->GetPlayerControllerIterator(); Controller; ++Controller)
	{
		auto PC = Cast<AZPlayerController>(Controller->Get());
		if (PC)
		{
			PC->GetZHUD()->GetUserHUD()->UpdateNumZombies(CurrentNumZombies);
		}

	}

}

void AZGameState::SetTotalWave(int32 NewTotalWave)
{
	TotalWave = NewTotalWave;
}

void AZGameState::UpdateRemainTime(float NewRemainTime)
{
	RemainTime = NewRemainTime;

	for (auto Controller = GetWorld()->GetPlayerControllerIterator(); Controller; ++Controller)
	{
		auto PC = Cast<AZPlayerController>(Controller->Get());
		if (PC)
		{
			PC->GetZHUD()->GetUserHUD()->UpdateRemainTime(RemainTime);
		}
	}

}

int32 AZGameState::GetTotalWave() const
{
	return TotalWave;
}

int32 AZGameState::GetCurrentWave() const
{
	return CurrentWave;
}

int32 AZGameState::GetCurrentNumZombies() const
{
	return CurrentNumZombies;
}

float AZGameState::GetRemainTime() const
{
	return RemainTime;
}

float AZGameState::GetElaspedTime() const
{
	return ElapsedTime;
}
