// Fill out your copyright notice in the Description page of Project Settings.


#include "ZSessionListWidget.h"
#include "ZGameInstance.h"
#include "ZMakeGameWidget.h"
#include "ZTitlePlayerController.h"
#include "ZPlayerState.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UZSessionListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SessionList = Cast<UScrollBox>(GetWidgetFromName(TEXT("SessionList")));
	check(SessionList);

	MakeGame = Cast<UButton>(GetWidgetFromName(TEXT("BTN_MakeGame")));
	check(MakeGame);

	RefreshList = Cast<UButton>(GetWidgetFromName(TEXT("BTN_RefreshList")));
	check(RefreshList);

	MakeGameWidget = Cast<UZMakeGameWidget>(GetWidgetFromName(TEXT("UI_ZMakeGame")));
	check(MakeGameWidget);

	MakeGame->OnClicked.AddDynamic(this, &UZSessionListWidget::OnMakeGameButtonClick);
	RefreshList->OnClicked.AddDynamic(this, &UZSessionListWidget::OnRefreshButtonClick);

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
				MyGameInstance->FindSession(PS->UniqueId.GetUniqueNetId(), true, false);
			}
		}
	}

}

void UZSessionListWidget::UpdateSessionList(const TArray<struct FZSessionInfo>& SessionsInfo)
{
}
