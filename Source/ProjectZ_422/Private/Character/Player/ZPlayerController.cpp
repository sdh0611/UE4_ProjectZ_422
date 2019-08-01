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

AZHUD * const AZPlayerController::GetZHUD() const
{
	return Cast<AZHUD>(GetHUD());
}

UZCharacterItemStatusComponent * const AZPlayerController::GetCharacterItemStatusComponent() const
{
	auto Player = Cast<AZCharacter>(GetPawn());
	if (Player)
	{
		return Player->GetItemStatusComponent();
	}

	return nullptr;
}
