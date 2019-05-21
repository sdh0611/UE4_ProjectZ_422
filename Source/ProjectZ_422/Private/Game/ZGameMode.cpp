// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGameMode.h"
#include "ZCharacter.h"
#include "ZPlayerController.h"
#include "ZHUD.h"

AZGameMode::AZGameMode()
{
	DefaultPawnClass = AZCharacter::StaticClass();
	PlayerControllerClass = AZPlayerController::StaticClass();
	HUDClass = AZHUD::StaticClass();
	
}

void AZGameMode::BeginPlay()
{
	Super::BeginPlay();
}
