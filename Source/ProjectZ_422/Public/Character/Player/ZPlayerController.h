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
	virtual void SetupInputComponent() override;


public:
	/*
		UserHUD Class�� ��ȯ�ϴ� �޼ҵ�.
		Inventory���� ������ UserHUD �����͸� �޾Ƽ� ���� ������.
		@ Return : ZUserHUD class pointer
	*/
	class UZUserHUD* const GetUserHUD() const;

	class AZHUD* const GetZHUD() const;

	class UZCharacterItemStatusComponent* const GetCharacterItemStatusComponent() const;


protected:
	void ToggleInventory();
	void ToggleInGameMenu();
	void RemoveWidgetFromTop();

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UZUserHUD> UserHUDClass;

private:
	UPROPERTY()
	class UZUserHUD* UserHUD;


};
