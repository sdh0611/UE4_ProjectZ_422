// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZSessionListWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZSessionListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
	void ShowSessionList();

protected:
	UFUNCTION(BlueprintCallable)
	void OnRefreshEnd();

private:
	UFUNCTION()
	void OnMakeGameButtonClick();

	UFUNCTION()
	void OnRefreshButtonClick();

	UFUNCTION()
	void OnExitButtonClick();

	void UpdateSessionList(const TArray<struct FZSessionInfo>& SessionsInfo);


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UZSessionListItemWidget> SessionListItemWidgetClass;

protected:
	UPROPERTY()
	class UScrollBox* SessionList;

	UPROPERTY()
	class UButton* MakeGame;

	UPROPERTY()
	class UButton* RefreshList;

	UPROPERTY()
	class UButton* Cancel;

	UPROPERTY()
	class UHorizontalBox* LoadingWidget;

	UPROPERTY()
	class UZMakeGameWidget* MakeGameWidget;
};
