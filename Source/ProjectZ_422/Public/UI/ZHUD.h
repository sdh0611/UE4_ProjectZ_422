// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/HUD.h"
#include "ZHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AZHUD();

public:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(VisibleAnywhere, Category = ZHUD)
	class AZPlayerController* PlayerController;

};
