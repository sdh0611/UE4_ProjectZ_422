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
	UZUserHUD(const FObjectInitializer & ObjectInitializer);


protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry & MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;
	

public:
	/* Currnet Money Info ������Ʈ ���� */
	void UpdateCurrentMoneyInfo(int32 NewMoney);

	/* �ش� �޼ҵ���� GameState���� ��������Ʈ�� ������Ʈ ��ų ��. */
	void UpdateRemainTime(float NewTime);
	void UpdateCurrentWave(int32 NewCurrentWave);
	void UpdateNumZombies(int32 NewValue);

public:
	/*
		ȭ�鿡 Widget�� ���� ���� �޼ҵ��
	*/
	void DrawInventoryWidget();
	void DrawShopWidget();
	/*
		ȭ�鿡�� Widget������� ���� �޼ҵ��
	 */
	void RemoveInventoryWidget();
	void RemoveShopWidget();

public:
	/*
		������ Widget�� ���¸� ������ �޼ҵ��
	*/
	bool IsInventoryOnScreen() const;
	bool IsShopWidgetOnScreen() const;
	/*
		Widget�� ������ ���� Get�޼ҵ��
	*/
	class UZInventoryWidget* const GetInventoryWidget() const;
	class UZShopWidget* const GetShopWidget() const;
	class UZCurrentWeaponInfoWidget* const GetCurrentWeaponInfoWidget() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = UserHUD, Meta = (AllowPrivateAccess = true))
	bool bIsInventoryOnScreen;

	UPROPERTY(BlueprintReadOnly, Category = UserHUD, Meta = (AllowPrivateAccess = true))
	bool bIsShopOnScreen;

	UPROPERTY(EditAnywhere, Category = ZHUD, BlueprintReadOnly)
	class UZInventoryWidget* InventoryWidget;

	UPROPERTY(EditAnywhere, Category = ZHUD, BlueprintReadOnly)
	class UZShopWidget* ShopWidget;

	UPROPERTY(EditAnywhere, Category = ZHUD, BlueprintReadOnly)
	class UZHPBarWidget* HPBarWidget;

	UPROPERTY(EditAnywhere, Category = ZHUD, BlueprintReadOnly)
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
