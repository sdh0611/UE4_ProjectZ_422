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
	virtual void NativeTick(const FGeometry & MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;
	

public:
	/* Currnet Money Info 업데이트 관련 */
	void UpdateCurrentMoneyInfo(int32 NewMoney);

	/* 해당 메소드들은 GameState에서 업데이트 시킬 것. */
	void UpdateRemainTime(float NewTime);
	void UpdateCurrentWave(int32 NewCurrentWave);
	void UpdateNumZombies(int32 NewValue);
	void UpdatePhaseText(EGamePhase NewPhase);

public:
	/*
		Toggle method.
	*/
	void ToggleInventoryWidget();
	void ToggleShopWidget();
	void ToggleInGameMenuWIdget();
	/*
		NOTE(9.11):
			폐기 예정
	*/
	void ToggleInputNumberWidget();

	/*
		화면에 Widget을 띄우기 위한 메소드들
	*/
	void DrawInventoryWidget();
	void DrawShopWidget();
	void DrawEndGameMenuWidget();
	void DrawInGameMenuWidget();
	/*
		NOTE(9.11):
			폐기 예정
	*/
	void DrawInputNumberWidget();
	
	/*
		화면에서 Widget을지우기 위한 메소드들
	 */
	void RemoveInventoryWidget();
	void RemoveShopWidget();
	void RemoveEndGameMenuWidget();
	void RemoveInGameMenuWidget();
	/*
		NOTE(9.11):
			폐기 예정
	*/
	void RemoveInputNumberWidget();

	/* UI 제거.(HUD제외) */
	void RemoveAllWidgetFromScreen();

	/* Handle Stack */
	void RemoveWidgetFromTop();

	void AddWidgetToList(class UZUserWidget* Widget);
	void RemoveWidgetFromList(class UZUserWidget* Widget);

public:
	/*
		각각의 Widget의 상태를 얻어오는 메소드들
	*/
	bool IsShopWidgetOnScreen() const;
	/*
		Widget을 얻어오기 위한 Get메소드들
	*/
	class UZInventoryWidget* const GetInventoryWidget() const;
	class UZShopWidget* const GetShopWidget() const;
	class UZCurrentWeaponInfoWidget* const GetCurrentWeaponInfoWidget() const;
	class UZInputNumberWidget* const GetInputNumberWidget() const;

protected:
	bool IsDrawWidgetListEmpty();
	
	UFUNCTION(BlueprintImplementableEvent)
	void CallWarningAnim();

	UFUNCTION(BlueprintImplementableEvent)
	void CallFadeInAnim();

	UFUNCTION(BlueprintImplementableEvent)
	void CallFadeAnim();

protected:
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

	UPROPERTY(EditAnywhere, Category = ZHUD, BlueprintReadOnly)
	class UZInputNumberWidget* InputNumberWidget;

	UPROPERTY(EditAnywhere, Category = ZHUD, BlueprintReadOnly)
	class UZUserWidget* EndGameMenuWidget;

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

private:
	UPROPERTY(VisibleAnywhere)
	TArray<TWeakObjectPtr<class UZUserWidget>> DrawWidgetList;
	
	//int32 DrawWidgetNum = 0;

};
