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
	
	Cancel = Cast<UButton>(GetWidgetFromName(TEXT("BTN_Cancel")));
	check(Cancel);

	LoadingWidget = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("LoadingWidget")));
	check(LoadingWidget);

	MakeGameWidget = Cast<UZMakeGameWidget>(GetWidgetFromName(TEXT("UI_ZMakeGame")));
	check(MakeGameWidget);

	MakeGame->OnClicked.AddDynamic(this, &UZSessionListWidget::OnMakeGameButtonClick);
	RefreshList->OnClicked.AddDynamic(this, &UZSessionListWidget::OnRefreshButtonClick);
	Cancel->OnClicked.AddDynamic(this, &UZSessionListWidget::OnExitButtonClick);

	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	if (MyGameInstance)
	{
		MyGameInstance->OnFindSessionsSuccess.BindUObject(this, &UZSessionListWidget::UpdateSessionList);
		MyGameInstance->OnFindSessionsEnd.BindUObject(this, &UZSessionListWidget::OnRefreshEnd);
		//MyGameInstance->OnFindSessionsEnd.BindLambda([this]() {
		//	LoadingWidget->SetVisibility(ESlateVisibility::Collapsed);
		//});
	}
}

void UZSessionListWidget::ShowSessionList()
{
	SetVisibility(ESlateVisibility::Visible);
	OnRefreshButtonClick();
}

void UZSessionListWidget::OnRefreshEnd()
{
	LoadingWidget->SetVisibility(ESlateVisibility::Collapsed);

	MakeGame->SetIsEnabled(true);
	RefreshList->SetIsEnabled(true);
	Cancel->SetIsEnabled(true);

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
	if (MakeGameWidget)
	{
		MakeGameWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	if (MyGameInstance)
	{
		auto PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			auto PS = PC->GetPlayerState<APlayerState>();
			if (PS)
			{
				/* �ϴ��� Ǯ��������� ���ϱ�� */
				MakeGame->SetIsEnabled(false);
				RefreshList->SetIsEnabled(false);
				Cancel->SetIsEnabled(false);
				SessionList->ClearChildren();

				LoadingWidget->SetVisibility(ESlateVisibility::Visible);
				MyGameInstance->FindSession(PS->UniqueId.GetUniqueNetId(), true, true);
			}
		}
	}

}

void UZSessionListWidget::OnExitButtonClick()
{
	SetVisibility(ESlateVisibility::Collapsed);
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
