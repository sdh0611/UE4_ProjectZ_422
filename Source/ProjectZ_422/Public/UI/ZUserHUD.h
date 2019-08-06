// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZUserHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZUserHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
public:
	/* Currnet Money Info 업데이트 관련 */
	void UpdateCurrentMoneyInfo(int32 NewMoney);

	/* 해당 메소드는 GameMode에서 델리게이트로 업데이트 시킬 것. */
	void UpdateRemainTime(float NewTime);
	
	void UpdateCurrentWave(int32 NewCurrentWave);
	void UpdateNumZombies(int32 NewValue);

public:
	/*
		화면에 Widget을 띄우기 위한 메소드들
	*/
	void DrawInventoryWidget();
	void DrawShopWidget();
	/*
		화면에서 Widget을지우기 위한 메소드들
	 */
	void RemoveInventoryWidget();
	void RemoveShopWidget();

public:
	/*
		각각의 Widget의 상태를 얻어오는 메소드들
	*/
	bool IsInventoryOnScreen() const;
	bool IsShopWidgetOnScreen() const;
	/*
		Widget을 얻어오기 위한 Get메소드들
	*/
	class UZInventoryWidget* const GetInventoryWidget() const;
	class UZShopWidget* const GetShopWidget() const;
	class UZCurrentWeaponInfoWidget* const GetCurrentWeaponInfoWidget() const;

private:
	UPROPERTY(BlueprintReadOnly, Category = UserHUD, Meta = (AllowPrivateAccess = true))
	bool bIsInventoryOnScreen;

	UPROPERTY(BlueprintReadOnly, Category = UserHUD, Meta = (AllowPrivateAccess = true))
	bool bIsShopOnScreen;

	UPROPERTY()
	class UZInventoryWidget* InventoryWidget;

	UPROPERTY()
	class UZShopWidget* ShopWidget;

	UPROPERTY()
	class UZHPBarWidget* HPBarWidget;

	UPROPERTY()
	class UZCurrentWeaponInfoWidget* CurrentWeaponInfoWidget;

	UPROPERTY()
	class UTextBlock* CurrentMoneyInfoText;

	UPROPERTY()
	class UTextBlock* RemainTimeText;

	UPROPERTY()
	class UTextBlock* TotalWaveText;

	UPROPERTY()
	class UTextBlock* CurrentWaveText;

	UPROPERTY()
	class UTextBlock* CurrentNumZombiesText;

};
