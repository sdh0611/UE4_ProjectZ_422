// Fill out your copyright notice in the Description page of Project Settings.


#include "ZCharacterStatusComponent.h"
#include "ZCharacter.h"
#include "ZBuff.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "UnrealNetwork.h"

// Sets default values for this component's properties
UZCharacterStatusComponent::UZCharacterStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bReplicates = true;

	// ...
	CurrentHP = 0.f;
	MaxHP = 100.f;

	bCanInterpGageValues = false;
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

	OnStatusChanged.Broadcast();
}


// Called every frame
void UZCharacterStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UZCharacterStatusComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UZCharacterStatusComponent, CurrentHP, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UZCharacterStatusComponent, MaxHP, COND_OwnerOnly);

}

void UZCharacterStatusComponent::AdjustCurrentHP(float Value)
{
	if (ROLE_Authority != GetOwnerRole())
	{
		return;
	}

	CurrentHP = FMath::Clamp<float>(CurrentHP + Value, 0.f, MaxHP);
	ZLOG(Warning, TEXT("CurrentHP : %.3f"), CurrentHP);
	//if (CurrentHP <= 0.f)
	//{
	//	OwnerCharacter->bIsDead = true;
	//}

	OnStatusChanged.Broadcast();
}

void UZCharacterStatusComponent::SetCurrentHP(float NewCurrentHP)
{
	if (ROLE_Authority != GetOwnerRole())
	{
		return;
	}

	CurrentHP = FMath::Clamp<float>(NewCurrentHP, 0.f, MaxHP);

	OnRep_CurrentHP();

}

void UZCharacterStatusComponent::SetMaxHP(float NewMaxHP)
{
	if (ROLE_Authority != GetOwnerRole())
	{
		return;
	}

	if (NewMaxHP < 0.f)
	{
		ZLOG(Error, TEXT("Invalid value."));
		return;
	}

	MaxHP = NewMaxHP;

	OnStatusChanged.Broadcast();
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


float UZCharacterStatusComponent::GetHPRatio() const
{
	return CurrentHP / MaxHP;
}

void UZCharacterStatusComponent::OnRep_CurrentHP()
{
	ZLOG_S(Error);
	OnStatusChanged.Broadcast();
}

