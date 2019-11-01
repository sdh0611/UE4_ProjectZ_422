// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Engine/GameViewportClient.h"
#include "ZGameViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
	
public:
	UZGameViewportClient();

public:
	virtual void Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice = true) override;

public:
	void ShowLoadingScreen();
	void HideLoadingScreen();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> LoadingWidgetClass;

	UPROPERTY()
	class UUserWidget* LoadingWidget;


};
