// Fill out your copyright notice in the Description page of Project Settings.


#include "ZPlayerStatusComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "UnrealNetwork.h"

UZPlayerStatusComponent::UZPlayerStatusComponent()
{
	CurrentDopingGage = 0.f;
	MaxDopingGage = 100.f;

	DecreaseDopingDelay = 5.f;
	DecreaseDopingGageValue = 10.f;
	SpeedIncrementWhileDoping = 120.f;
}

void UZPlayerStatusComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UZPlayerStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UZPlayerStatusComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UZPlayerStatusComponent, CurrentDopingGage, COND_OwnerOnly);

}

void UZPlayerStatusComponent::AdjustCurrentDopingGage(float Value)
{
	if (ROLE_Authority != GetOwnerRole())
	{
		return;
	}

	if (Value > 0 && CurrentDopingGage <= 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(DopingTimer, this, &UZPlayerStatusComponent::DecreaseDopingGage, DecreaseDopingDelay, true, DecreaseDopingDelay);
		GetWorld()->GetTimerManager().SetTimer(IncreaseHPTimer, this, &UZPlayerStatusComponent::IncreaseHPWhileDoping, 3.f, true, 3.f);
	}

	CurrentDopingGage = FMath::Clamp<float>(CurrentDopingGage + Value, 0.f, MaxDopingGage);
	ZLOG(Warning, TEXT("CurrentDoping : %.3f"), CurrentDopingGage);
	OnRep_CurrentDopingGage();

	if (CurrentDopingGage <= 0.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(DopingTimer);
		GetWorld()->GetTimerManager().ClearTimer(IncreaseHPTimer);
	}
}

void UZPlayerStatusComponent::SetCurrentDopingGage(float NewCurrentDopingGage)
{
	CurrentDopingGage = FMath::Clamp<float>(NewCurrentDopingGage, 0.f, MaxDopingGage);

	OnStatusChanged.Broadcast();
}

void UZPlayerStatusComponent::SetMaxDopingGage(float NewMaxDopingGage)
{
	if (NewMaxDopingGage < 0.f)
	{
		ZLOG(Error, TEXT("Invalid value."));
		return;
	}

	MaxDopingGage = NewMaxDopingGage;

	OnStatusChanged.Broadcast();
}

void UZPlayerStatusComponent::SetSpeedIncrementWhileDoping(float NewIncrement)
{
	float Increment = NewIncrement;

	if (Increment < 0.f)
	{
		Increment = 0.f;
	}

	SpeedIncrementWhileDoping = Increment;
}

float UZPlayerStatusComponent::GetCurrentDopingGage() const
{
	return CurrentDopingGage;
}

float UZPlayerStatusComponent::GetMaxDopingGage() const
{
	return MaxDopingGage;
}

float UZPlayerStatusComponent::GetSpeedIncrementWhileDoping() const
{
	return SpeedIncrementWhileDoping;
}

float UZPlayerStatusComponent::GetDopingGageRatio() const
{
	return CurrentDopingGage / MaxDopingGage;
}

void UZPlayerStatusComponent::DecreaseDopingGage()
{
	AdjustCurrentDopingGage(-DecreaseDopingGageValue);
}

void UZPlayerStatusComponent::IncreaseHPWhileDoping()
{
	AdjustCurrentHP(5.f);
}

void UZPlayerStatusComponent::OnRep_CurrentDopingGage()
{
	OnStatusChanged.Broadcast();
}
