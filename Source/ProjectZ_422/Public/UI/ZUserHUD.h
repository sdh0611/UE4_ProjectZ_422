// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "ZGameMode.h"
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


public:
	/* Currnet Money Info ������Ʈ ���� */
	void UpdateCurrentMoneyInfo(int32 NewMoney);

	/* �ش� �޼ҵ���� GameState���� ������Ʈ ��ų ��. */
	void UpdateRemainTime(float NewTime);
	void UpdateCurrentWave(int32 NewCurrentWave);
	void UpdateNumZombies(int32 NewValue);
	void UpdatePhaseText(EGamePhase NewPhase);
	void UpdateInteractionName(const FString& Name);
	void ToggleInteractionInfo(bool bShow);

public:
	/*
		Toggle method.
	*/
	void ToggleInventoryWidget();
	void ToggleShopWidget();
	void ToggleInGameMenuWIdget();
	/*
		NOTE(9.11):
			��� ����
	*/
	void ToggleInputNumberWidget();

	/*
		ȭ�鿡 Widget�� ���� ���� �޼ҵ��
	*/
	void DrawInventoryWidget();
	void DrawShopWidget();
	void DrawDeadMenuWidget();
	void DrawWinMenuWidget();
	void DrawInGameMenuWidget();
	/*
		NOTE(9.11):
			��� ����
	*/
	void DrawInputNumberWidget();
	
	/*
		ȭ�鿡�� Widget������� ���� �޼ҵ��
	 */
	void RemoveInventoryWidget();
	void RemoveShopWidget();
	//void RemoveEndGameMenuWidget();
	void RemoveInGameMenuWidget();
	/*
		NOTE(9.11):
			��� ����
	*/
	void RemoveInputNumberWidget();

	/* UI ����.(HUD����) */
	void RemoveAllWidgetFromScreen();

	/* Handle Stack */
	void RemoveWidgetFromTop();

	void AddWidgetToList(class UZUserWidget* Widget);
	void RemoveWidgetFromList(class UZUserWidget* Widget);

public:
	bool IsShopWidgetOnScreen() const;
	/*
		Widget�� ������ ���� Get�޼ҵ��
	*/
	class UZInventoryWidget* const GetInventoryWidget() const;
	class UZShopWidget* const GetShopWidget() const;
	class UZCurrentWeaponInfoWidget* const GetCurrentWeaponInfoWidget() const;
	class UZInputNumberWidget* const GetInputNumberWidget() const;


	UFUNCTION(BlueprintImplementableEvent)
	void CallFadeOutBloodSplatterAnim();

protected:
	bool IsDrawWidgetListEmpty();
	
	UFUNCTION(BlueprintImplementableEvent)
	void CallWarningAnim();

	UFUNCTION(BlueprintImplementableEvent)
	void CallFadeInAnim();

	UFUNCTION(BlueprintImplementableEvent)
	void CallFadeAnim();


protected:
	UPROPERTY(EditAnywhere, Category = ZHUD, BlueprintReadOnly)
	class UZInventoryWidget* InventoryWidget;

	UPROPERTY(EditAnywhere, Category = ZHUD, BlueprintReadOnly)
	class UZShopWidget* ShopWidget;

	UPROPERTY(EditAnywhere, Category = ZHUD, BlueprintReadOnly)
	class UZHPBarWidget* HPBarWidget;

	UPROPERTY(EditAnywhere, Category = ZHUD, BlueprintReadOnly)
	class UZCurrentWeaponInfoWidget* CurrentWeaponInfoWidget;

	UPROPERTY(EditAnywhere, Category = ZHUD, BlueprintReadOnly)
	class UZInputNumberWidget* InputNumberWidget;

	UPROPERTY(EditAnywhere, Category = ZHUD, BlueprintReadOnly)
	class UZEndGameWidget* EndGameMenuWidget;

	UPROPERTY(EditAnywhere, Category = ZHUD, BlueprintReadOnly)
	class UZUserWidget* InGameMenuWidget;

	UPROPERTY()
	class UTextBlock* CurrentMoneyInfoText;

	UPROPERTY()
	class UTextBlock* RemainTimeText;

	UPROPERTY()
	class UTextBlock* PhaseText;

	UPROPERTY()
	class UTextBlock* TotalWaveText;

	UPROPERTY()
	class UTextBlock* CurrentWaveText;

	UPROPERTY()
	class UTextBlock* CurrentNumZombiesText;

	UPROPERTY()
	class UBorder* InteractionInfo;

	UPROPERTY()
	class UTextBlock* InteractionNameText;

private:
	UPROPERTY(VisibleAnywhere)
	TArray<TWeakObjectPtr<class UZUserWidget>> DrawWidgetList;
	
	//int32 DrawWidgetNum = 0;

};
