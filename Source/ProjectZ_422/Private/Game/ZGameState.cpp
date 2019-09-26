// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGameState.h"
#include "ZPlayerController.h"
#include "ZHUD.h"
#include "ZUserHUD.h"
#include "Engine/World.h"
#include "UnrealNetwork.h"

AZGameState::AZGameState()
{
	TotalWave = 10;
	CurrentWave = 0;
	CurrentNumZombies = 0;
	RemainTime = 0.f;
	CurrentGamePhase = EGamePhase::Ready;
}

void AZGameState::BeginPlay()
{
	Super::BeginPlay();

	

}

void AZGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AZGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZGameState, TotalWave);
	DOREPLIFETIME(AZGameState, CurrentWave);
	DOREPLIFETIME(AZGameState, CurrentNumZombies);
	DOREPLIFETIME(AZGameState, RemainTime);
	DOREPLIFETIME(AZGameState, CurrentGamePhase);

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
		if (PC && PC->IsLocalPlayerController())
		{
			if (PC->GetUserHUD())
			{
				PC->GetUserHUD()->UpdateNumZombies(CurrentNumZombies);
			}
		}

	}

}

void AZGameState::SetTotalWave(int32 NewTotalWave)
{
	if (NewTotalWave < 1)
	{
		ZLOG(Error, TEXT("Invalid value."));
		return;
	}

	TotalWave = NewTotalWave;
}

void AZGameState::SetCurrentWave(int32 NewCurrentWave)
{
	if (NewCurrentWave < 1)
	{
		ZLOG(Error, TEXT("Invalid value."));
		return;
	}

	CurrentWave = NewCurrentWave;

	for (auto Controller = GetWorld()->GetPlayerControllerIterator(); Controller; ++Controller)
	{
		auto PC = Cast<AZPlayerController>(Controller->Get());
		if (PC && PC->IsLocalPlayerController())
		{
			if (PC->GetUserHUD())
			{
				PC->GetUserHUD()->UpdateCurrentWave(CurrentWave);
			}
		}
	}
}

void AZGameState::SetCurrentNumZombies(int32 NewNumZombies)
{
	if (NewNumZombies < 0)
	{
		ZLOG(Error, TEXT("Invalid value.."));
		return;
	}

	CurrentNumZombies = NewNumZombies;

	for (auto Controller = GetWorld()->GetPlayerControllerIterator(); Controller; ++Controller)
	{
		auto PC = Cast<AZPlayerController>(Controller->Get());
		if (PC && PC->IsLocalPlayerController())
		{
			if (PC->GetUserHUD())
			{
				PC->GetUserHUD()->UpdateNumZombies(CurrentNumZombies);
			}
		}
	}

}

void AZGameState::SetCurrentGamePhase(EGamePhase NewPhase)
{
	CurrentGamePhase = NewPhase;

	for (auto Controller = GetWorld()->GetPlayerControllerIterator(); Controller; ++Controller)
	{
		auto PC = Cast<AZPlayerController>(Controller->Get());
		if (PC && PC->IsLocalPlayerController())
		{
			if (PC->GetUserHUD())
			{
				PC->GetUserHUD()->UpdatePhaseText(CurrentGamePhase);
			}
		}

	}

}

void AZGameState::UpdateRemainTime(float NewRemainTime)
{
	if (NewRemainTime < 0.f)
	{
		ZLOG(Error, TEXT("Invalid value."));
		return;
	}

	RemainTime = NewRemainTime;

	
	for (auto Controller = GetWorld()->GetPlayerControllerIterator(); Controller; ++Controller)
	{
		auto PC = Cast<AZPlayerController>(Controller->Get());
		if (PC && PC->IsLocalPlayerController())
		{
			if (PC->GetUserHUD())
			{
				PC->GetUserHUD()->UpdateRemainTime(RemainTime);
			}
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

void AZGameState::OnRep_TotalWave()
{


}

void AZGameState::OnRep_CurrentWave()
{
	for (auto Controller = GetWorld()->GetPlayerControllerIterator(); Controller; ++Controller)
	{
		auto PC = Cast<AZPlayerController>(Controller->Get());
		if (PC && PC->IsLocalPlayerController())
		{
			if (PC->GetUserHUD())
			{
				PC->GetUserHUD()->UpdateCurrentWave(CurrentWave);
			}
		}

	}
}

void AZGameState::OnRep_CurrentNumZombies()
{
	for (auto Controller = GetWorld()->GetPlayerControllerIterator(); Controller; ++Controller)
	{
		auto PC = Cast<AZPlayerController>(Controller->Get());
		if (PC && PC->IsLocalPlayerController())
		{
			if (PC->GetUserHUD())
			{
				PC->GetUserHUD()->UpdateNumZombies(CurrentNumZombies);
			}
		}

	}
}

void AZGameState::OnRep_RemainTime()
{
	for (auto Controller = GetWorld()->GetPlayerControllerIterator(); Controller; ++Controller)
	{
		auto PC = Cast<AZPlayerController>(Controller->Get());
		if (PC && PC->IsLocalPlayerController())
		{
			if (PC->GetUserHUD())
			{
				PC->GetUserHUD()->UpdateRemainTime(RemainTime);
			}
		}

	}
}

void AZGameState::OnRep_CurrentGamePhase()
{
	for (auto Controller = GetWorld()->GetPlayerControllerIterator(); Controller; ++Controller)
	{
		auto PC = Cast<AZPlayerController>(Controller->Get());
		if (PC && PC->IsLocalPlayerController())
		{
			if (PC->GetUserHUD())
			{
				PC->GetUserHUD()->UpdatePhaseText(CurrentGamePhase);
			}
		}

	}
}
