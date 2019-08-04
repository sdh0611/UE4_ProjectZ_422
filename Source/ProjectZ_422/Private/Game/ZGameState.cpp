// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGameState.h"

AZGameState::AZGameState()
{
	TotalWave = 10;
	CurrentWave = 0;
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
}

int32 AZGameState::GetTotalWave() const
{
	return TotalWave;
}

int32 AZGameState::GetCurrentWave() const
{
	return CurrentWave;
}

float AZGameState::GetElaspedTime() const
{
	return ElapsedTime;
}
