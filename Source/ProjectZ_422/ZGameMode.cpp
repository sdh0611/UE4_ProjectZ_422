// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGameMode.h"
#include "ZCharacter.h"

AZGameMode::AZGameMode()
{
	DefaultPawnClass = AZCharacter::StaticClass();

}

void AZGameMode::BeginPlay()
{
	Super::BeginPlay();
}
