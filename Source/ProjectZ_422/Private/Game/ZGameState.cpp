// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGameState.h"

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

	ElapsedTime += DeltaTime;
}

void AZGameState::IncreaseCurrentWave()
{
	++CurrentWave;
	OnCurrentWaveUpdate.Broadcast(CurrentWave);
}

void AZGameState::AdjustCurrentNumZombies(int32 NewValue)
{
	CurrentNumZombies += NewValue;
	if (CurrentNumZombies < 0)
	{
		CurrentNumZombies = 0;
	}
	OnNumZombiesUpdate.Broadcast(CurrentNumZombies);
}

void AZGameState::SetTotalWave(int32 NewTotalWave)
{
	TotalWave = NewTotalWave;
}

void AZGameState::UpdateRemainTime(float NewRemainTime)
{
	RemainTime = NewRemainTime;
	if (OnTimeUpdate.IsBound())
	{
		OnTimeUpdate.Broadcast(RemainTime);
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
