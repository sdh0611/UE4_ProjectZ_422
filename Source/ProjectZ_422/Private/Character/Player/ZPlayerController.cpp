// Fill out your copyright notice in the Description page of Project Settings.


#include "ZPlayerController.h"
#include "ZPlayerState.h"
#include "ZHUD.h"
#include "ZPlayerCameraManager.h"
#include "ZCharacter.h"
#include "ZCharacterItemStatusComponent.h"


AZPlayerController::AZPlayerController()
{
	PlayerCameraManagerClass = AZPlayerCameraManager::StaticClass();

}

void AZPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

}

AZHUD * const AZPlayerController::GetZHUD() const
{
	return Cast<AZHUD>(GetHUD());
}

UZCharacterItemStatusComponent * const AZPlayerController::GetCharacterItemStatusComponent() const
{
	auto Chracter = Cast<AZCharacter>(GetPawn());
	if (Chracter)
	{
		return Chracter->GetItemStatusComponent();
	}

	return nullptr;
}
