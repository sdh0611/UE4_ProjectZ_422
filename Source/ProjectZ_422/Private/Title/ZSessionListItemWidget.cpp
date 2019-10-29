// Fill out your copyright notice in the Description page of Project Settings.


#include "ZSessionListItemWidget.h"
#include "Components/TextBlock.h"
#include "ZTitlePlayerController.h"
#include "ZPlayerState.h"
#include "Kismet/GameplayStatics.h"

void UZSessionListItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SessionName = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_SessionName")));
	check(SessionName);

	CurrentConnection = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_CurrentConnection")));
	check(CurrentConnection);

	MaxConnection = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_MaxConnection")));
	check(MaxConnection);

}

void UZSessionListItemWidget::UpdateWidget(const FZSessionInfo & NewSessionInfo)
{
	SessionInfo = NewSessionInfo;
	UpdateSessionName(NewSessionInfo.SessionName);
	UpdateCurrentConnection(NewSessionInfo.CurrentConnection);
	UpdateMaxConnection(NewSessionInfo.MaxConnection);
}

void UZSessionListItemWidget::UpdateSessionName(const FString & NewSessionName)
{
	SessionName->SetText(FText::FromString(NewSessionName));
}

void UZSessionListItemWidget::UpdateCurrentConnection(int32 NewCurrentConnection)
{
	CurrentConnection->SetText(FText::FromString(FString::FromInt(NewCurrentConnection)));
}

void UZSessionListItemWidget::UpdateMaxConnection(int32 NewMaxConnection)
{
	MaxConnection->SetText(FText::FromString(FString::FromInt(NewMaxConnection)));
}

FReply UZSessionListItemWidget::NativeOnMouseButtonDoubleClick(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	auto Result = Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);

	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	if (MyGameInstance)
	{
		auto PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			auto PS = PC->GetPlayerState<APlayerState>();
			if (PS)
			{
				MyGameInstance->SessionJoinByIndex(PS->UniqueId.GetUniqueNetId(), *SessionInfo.SessionName, SessionInfo.SessionIndex);
			}
		}
	}

	return Result;
}
