// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZLobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZLobbyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	void UpdateConnectNumber(int32 NewNumber);
	void UpdatePlayerName(int32 PlayerIndex, const FString& PlayerName);
	void UpdateChatBox(const FString& PlayerName, const FString& NewChat);

public:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayFadeInStartButtonAnim();

protected:
	bool IsValidPlayerIndex(int32 PlayerIndex) const;

private:
	UFUNCTION()
	void OnInputChatCommit(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnStartButtonClick();

protected:
	UPROPERTY()
	class UButton* StartButton;

	UPROPERTY()
	class UTextBlock* ConnectNumber;

	UPROPERTY()
	TArray<class UTextBlock*> PlayerNameList;

	UPROPERTY()
	class UEditableTextBox* InputChat;

	UPROPERTY()
	class UScrollBox* ChatBox;

	

};
