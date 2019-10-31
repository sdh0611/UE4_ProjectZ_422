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

private:
	UFUNCTION()
	void OnMakeGameButtonClick();

	UFUNCTION()
	void OnRefreshButtonClick();

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
	class UHorizontalBox* LoadingWidget;

	UPROPERTY()
	class UZMakeGameWidget* MakeGameWidget;
};
