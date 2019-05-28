// Fill out your copyright notice in the Description page of Project Settings.


#include "ZCharacterStatusComponent.h"

// Sets default values for this component's properties
UZCharacterStatusComponent::UZCharacterStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	CurrentHP = 100.f;
	MaxHP = 100.f;
}


// Called when the game starts
void UZCharacterStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UZCharacterStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UZCharacterStatusComponent::AdjustCurrentHP(float Value)
{
	CurrentHP = FMath::Clamp<float>(CurrentHP + Value, 0.f, MaxHP);
	ZLOG(Warning, TEXT("CurrentHP : %.3f"), CurrentHP);

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

