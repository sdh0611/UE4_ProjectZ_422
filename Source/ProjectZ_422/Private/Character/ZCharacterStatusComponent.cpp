// Fill out your copyright notice in the Description page of Project Settings.


#include "ZCharacterStatusComponent.h"
#include "ZCharacter.h"
#include "ZBuff.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UZCharacterStatusComponent::UZCharacterStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	CurrentHP = 0.f;
	MaxHP = 100.f;

	CurrentDopingGage = 0.f;
	MaxDopingGage = 100.f;
	TargetDopingGage = CurrentDopingGage;

	DecreaseDopingDelay = 5.f;
	DecreaseDopingGageValue = 10.f;
	SpeedIncrementWhileDoping = 120.f;

	bCanInterpGageValues = true;
}


// Called when the game starts
void UZCharacterStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	auto Character = Cast<AZCharacter>(GetOwner());
	if (Character)
	{
		OwnerCharacter = Character;
	}

	CurrentHP = MaxHP;
	TargetHP = CurrentHP;

	OnStatusChanged.Broadcast();
}


// Called every frame
void UZCharacterStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// UpdateBuff
	UpdateBuff(DeltaTime);

	if (bCanInterpGageValues)
	{
		InterpGage(DeltaTime);
	}

}

void UZCharacterStatusComponent::AdjustCurrentHP(float Value)
{
	CurrentHP = FMath::Clamp<float>(CurrentHP + Value, 0.f, MaxHP);
	ZLOG(Warning, TEXT("CurrentHP : %.3f"), CurrentHP);
	

	//OnStatusChanged.Broadcast();
}

void UZCharacterStatusComponent::AdjustCurrentDopingGage(float Value)
{
	if (CurrentDopingGage <= 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(DopingTimer, this, &UZCharacterStatusComponent::DecreaseDopingGage, DecreaseDopingDelay, true, DecreaseDopingDelay);
		GetWorld()->GetTimerManager().SetTimer(IncreaseHPTimer, this, &UZCharacterStatusComponent::IncreaseHPWhileDoping, 3.f, true, 3.f);
	}

	CurrentDopingGage = FMath::Clamp<float>(CurrentDopingGage + Value, 0.f, MaxDopingGage);
	ZLOG(Warning, TEXT("CurrentDoping : %.3f"), CurrentDopingGage);

	if (CurrentDopingGage <= 0.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(DopingTimer);
		GetWorld()->GetTimerManager().ClearTimer(IncreaseHPTimer);
	}
	
	
	//OnStatusChanged.Broadcast();
}

void UZCharacterStatusComponent::AddBuff(UZBuff * NewBuff)
{
	NewBuff->ApplyBuff(this);
	BuffList.Add(NewBuff);
}

void UZCharacterStatusComponent::UpdateBuff(float DeltaTime)
{
	for (const auto& Buff : BuffList)
	{
		// Check Is buff end
		if (Buff->CheckBuffEnd())
		{
			Buff->EndBuff(this);
		}
		else
		{
			// Update Buff
			Buff->Update(DeltaTime);
		}
	}
}

void UZCharacterStatusComponent::SetCurrentHP(float NewCurrentHP)
{
	CurrentHP = FMath::Clamp<float>(NewCurrentHP, 0.f, MaxHP);
}

void UZCharacterStatusComponent::SetMaxHP(float NewMaxHP)
{
	if (NewMaxHP < 0.f)
	{
		ZLOG(Error, TEXT("Invalid value."));
		return;
	}

	MaxHP = NewMaxHP;

	OnStatusChanged.Broadcast();
}

void UZCharacterStatusComponent::SetCurrentDopingGage(float NewCurrentDopingGage)
{
	CurrentDopingGage= FMath::Clamp<float>(NewCurrentDopingGage, 0.f, MaxDopingGage);

	OnStatusChanged.Broadcast();
}

void UZCharacterStatusComponent::SetMaxDopingGage(float NewMaxDopingGage)
{
	if (NewMaxDopingGage < 0.f)
	{
		ZLOG(Error, TEXT("Invalid value."));
		return;
	}

	MaxDopingGage = NewMaxDopingGage;

	OnStatusChanged.Broadcast();
}

void UZCharacterStatusComponent::SetSpeedIncrementWhiledoping(float NewIncrement)
{
	float Increment = NewIncrement;

	if (Increment < 0.f)
	{
		Increment = 0.f;
	}

	SpeedIncrementWhileDoping = Increment;
}

bool UZCharacterStatusComponent::IsDead() const
{
	return CurrentHP <= 0.f;
}

float UZCharacterStatusComponent::GetCurrentHP() const
{
	return CurrentHP;
}

float UZCharacterStatusComponent::GetMaxHP() const
{
	return MaxHP;
}

float UZCharacterStatusComponent::GetCurrentDopingGage() const
{
	return CurrentDopingGage;
}

float UZCharacterStatusComponent::GetMaxDopingGage() const
{
	return MaxDopingGage;
}

float UZCharacterStatusComponent::GetSpeedIncrementWhileDoping() const
{
	return SpeedIncrementWhileDoping;
}

float UZCharacterStatusComponent::GetHPRatio() const
{
	return CurrentHP / MaxHP;
}

float UZCharacterStatusComponent::GetTargetHPRatio() const
{
	return TargetHP / MaxHP;
}

float UZCharacterStatusComponent::GetDopingGageRatio() const
{
	return CurrentDopingGage / MaxDopingGage;
}

float UZCharacterStatusComponent::GetTargetDopingGageRatio() const
{
	return TargetDopingGage / MaxDopingGage;
}

void UZCharacterStatusComponent::DecreaseDopingGage()
{
	AdjustCurrentDopingGage(-DecreaseDopingGageValue);
}

void UZCharacterStatusComponent::InterpGage(float DeltaTime)
{
	bool bCallDelegate = false;

	if (CurrentHP != TargetHP)
	{
		TargetHP = FMath::FInterpTo(TargetHP, CurrentHP, DeltaTime, 10.f);
		bCallDelegate = true;
	}

	if (CurrentDopingGage != TargetDopingGage)
	{
		TargetDopingGage = FMath::FInterpTo(TargetDopingGage, CurrentDopingGage, DeltaTime, 10.f);
		bCallDelegate = true;
	}

	if (bCallDelegate)
	{
		OnStatusChanged.Broadcast();
	}

}

void UZCharacterStatusComponent::IncreaseHPWhileDoping()
{
	AdjustCurrentHP(5.f);
}

