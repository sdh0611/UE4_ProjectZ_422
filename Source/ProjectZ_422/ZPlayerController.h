// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/PlayerController.h"
#include "ZPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AZPlayerController();

public:
	class AZHUD* const GetZHUD() const;
	
};
