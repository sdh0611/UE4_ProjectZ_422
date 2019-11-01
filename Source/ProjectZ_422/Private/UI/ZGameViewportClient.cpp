// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGameViewportClient.h"
#include "Blueprint/UserWidget.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

UZGameViewportClient::UZGameViewportClient()
{
	static ConstructorHelpers::FClassFinder<UUserWidget>
		UI_LOADING(TEXT("WidgetBlueprint'/Game/Blueprint/Loading/UI_ZLoadingWidget.UI_ZLoadingWidget_C'"));
	if (UI_LOADING.Succeeded())
	{
		LoadingWidgetClass = UI_LOADING.Class;
	}

}

void UZGameViewportClient::Init(FWorldContext & WorldContext, UGameInstance * OwningGameInstance, bool bCreateNewAudioDevice)
{
	Super::Init(WorldContext, OwningGameInstance, bCreateNewAudioDevice);

	if (LoadingWidgetClass)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("LoadingWidget Success")));
		LoadingWidget = CreateWidget<UUserWidget>(OwningGameInstance, LoadingWidgetClass);
		if (LoadingWidget)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Add LoadingWidget Success")));
			//LoadingWidget->AddToViewport();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("LoadingWidget fail")));

	}

}

void UZGameViewportClient::ShowLoadingScreen()
{
	//if (LoadingWidgetClass)
	//{
	//	LoadingWidget = CreateWidget<UUserWidget, UGameViewportClient>(this, LoadingWidgetClass);
	//	if (LoadingWidget)
	//	{
	//		LoadingWidget->AddToViewport();
	//	}
	//}
	if (LoadingWidget)
	{
		AddViewportWidgetContent(LoadingWidget->GetCachedWidget().ToSharedRef());
	}
}

void UZGameViewportClient::HideLoadingScreen()
{
	if (LoadingWidget)
	{
		RemoveViewportWidgetContent(LoadingWidget->GetCachedWidget().ToSharedRef());
	}
}
