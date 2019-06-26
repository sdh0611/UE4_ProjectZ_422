// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Camera/PlayerCameraManager.h"
#include "ZPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	AZPlayerCameraManager();

public:
	virtual void InitializeFor(APlayerController* PC) override;
	virtual void UpdateCamera(float DeltaTime) override;

private:
	float NormalFOV;
	float AimFOV;
	float CurrentFOV;


};
