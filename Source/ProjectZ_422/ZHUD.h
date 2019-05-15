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
	
public:
	/*
		UserHUD Class를 반환하는 메소드.
		Inventory등의 조작은 UserHUD 포인터를 받아서 직접 조작함.
		@ Return : ZUserHUD class pointer
	*/
	class UZUserHUD* const GetUserHUD() const;

private:
	UPROPERTY(VisibleAnywhere, Category = WidgetClass)
	TSubclassOf<class UZUserHUD> UserHUDClass;

private:
	UPROPERTY(VisibleAnywhere, Category = ZHUD)
	class AZPlayerController* PlayerController;

private:
	UPROPERTY(VisibleAnywhere, Category = Widget)
	class UZUserHUD* UserHUD;

};
