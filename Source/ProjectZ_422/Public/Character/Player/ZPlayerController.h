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
	virtual void BeginPlay() override;

public:
	/*
		UserHUD Class를 반환하는 메소드.
		Inventory등의 조작은 UserHUD 포인터를 받아서 직접 조작함.
		@ Return : ZUserHUD class pointer
	*/
	class UZUserHUD* const GetUserHUD() const;

	class AZHUD* const GetZHUD() const;

	class UZCharacterItemStatusComponent* const GetCharacterItemStatusComponent() const;

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UZUserHUD> UserHUDClass;

private:
	UPROPERTY()
	class UZUserHUD* UserHUD;


};
