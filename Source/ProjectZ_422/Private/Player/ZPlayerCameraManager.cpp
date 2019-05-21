// Fill out your copyright notice in the Description page of Project Settings.


#include "ZPlayerCameraManager.h"
#include "ZCharacter.h"
#include "ZPlayerController.h"

AZPlayerCameraManager::AZPlayerCameraManager()
{
	NormalFOV = 90.f;
	AimFOV = 45.f;
	CurrentFOV = NormalFOV;
}

void AZPlayerCameraManager::InitializeFor(APlayerController * PC)
{
	Super::InitializeFor(PC);

}

void AZPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);

	auto Player = Cast<AZCharacter>(PCOwner->GetPawn());
	if (Player)
	{
		float TargetFOV = Player->IsAiming() ? AimFOV : NormalFOV;

		CurrentFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, 20.f);
		SetFOV(CurrentFOV);
	}

}
