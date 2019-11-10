// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZClientGameInstance.h"
#include "ZSessionListItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZSessionListItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	void UpdateWidget(const struct FZSessionInfo& NewSessionInfo);
	void UpdateSessionName(const FString& NewSessionName);
	void UpdateCurrentConnection(int32 NewCurrentConnection);
	void UpdateMaxConnection(int32 NewMaxConnection);

private:
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

protected:
	FZSessionInfo SessionInfo;

	UPROPERTY()
	class UTextBlock* SessionName;

	UPROPERTY()
	class UTextBlock* CurrentConnection;

	UPROPERTY()
	class UTextBlock* MaxConnection;

};
