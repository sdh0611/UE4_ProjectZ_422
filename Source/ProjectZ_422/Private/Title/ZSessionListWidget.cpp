// Fill out your copyright notice in the Description page of Project Settings.


#include "ZSessionListWidget.h"
#include "ZGameInstance.h"
#include "ZMakeGameWidget.h"
#include "ZSessionListItemWidget.h"
#include "ZTitlePlayerController.h"
#include "ZPlayerState.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

void UZSessionListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SessionList = Cast<UScrollBox>(GetWidgetFromName(TEXT("SessionList")));
	check(SessionList);

	MakeGame = Cast<UButton>(GetWidgetFromName(TEXT("BTN_MakeGame")));
	check(MakeGame);

	RefreshList = Cast<UButton>(GetWidgetFromName(TEXT("BTN_RefreshList")));
	check(RefreshList);

	LoadingWidget = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("LoadingWidget")));
	check(LoadingWidget);

	MakeGameWidget = Cast<UZMakeGameWidget>(GetWidgetFromName(TEXT("UI_ZMakeGame")));
	check(MakeGameWidget);

	MakeGame->OnClicked.AddDynamic(this, &UZSessionListWidget::OnMakeGameButtonClick);
	RefreshList->OnClicked.AddDynamic(this, &UZSessionListWidget::OnRefreshButtonClick);

	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	if (MyGameInstance)
	{
		MyGameInstance->OnFindSessionsSuccess.BindUObject(this, &UZSessionListWidget::UpdateSessionList);
		MyGameInstance->OnFindSessionsEnd.BindLambda([this]() {
			LoadingWidget->SetVisibility(ESlateVisibility::Collapsed);
		});
	}
}

void UZSessionListWidget::ShowSessionList()
{
	SetVisibility(ESlateVisibility::Visible);
	OnRefreshButtonClick();
}

void UZSessionListWidget::OnMakeGameButtonClick()
{
	if (MakeGameWidget)
	{
		MakeGameWidget->SetVisibility(ESlateVisibility::Visible);
	}

}

void UZSessionListWidget::OnRefreshButtonClick()
{
	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	if (MyGameInstance)
	{
		auto PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			auto PS = PC->GetPlayerState<APlayerState>();
			if (PS)
			{
				/* 일단은 풀링방식으로 안하기로 */
				SessionList->ClearChildren();
				LoadingWidget->SetVisibility(ESlateVisibility::Visible);
				MyGameInstance->FindSession(PS->UniqueId.GetUniqueNetId(), true, true);
			}
		}
	}

}

void UZSessionListWidget::UpdateSessionList(const TArray<struct FZSessionInfo>& SessionsInfo)
{
	LoadingWidget->SetVisibility(ESlateVisibility::Collapsed);

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("UpdagteSessionList!"));
	auto PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (nullptr == PC)
	{
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("In UpdagteSessionList!"));


	for (const auto& SessionInfo : SessionsInfo)
	{
		auto SessionListItem = CreateWidget<UZSessionListItemWidget>(PC, SessionListItemWidgetClass);
		if (SessionListItem)
		{
			SessionList->AddChild(SessionListItem);
			SessionListItem->UpdateWidget(SessionInfo);
		}
	}

}
