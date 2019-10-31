// Fill out your copyright notice in the Description page of Project Settings.


#include "ZSessionListItemWidget.h"
#include "Components/TextBlock.h"
#include "ZTitlePlayerController.h"
#include "ZPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

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
	UpdateSessionName(NewSessionInfo.ServerName);
	UpdateCurrentConnection(NewSessionInfo.CurrentConnection);
	UpdateMaxConnection(NewSessionInfo.MaxConnection);
}

void UZSessionListItemWidget::UpdateSessionName(const FString & NewSessionName)
{
	if (SessionName)
	{
		SessionName->SetText(FText::FromString(NewSessionName));
	}
}

void UZSessionListItemWidget::UpdateCurrentConnection(int32 NewCurrentConnection)
{
	if (CurrentConnection)
	{
		CurrentConnection->SetText(FText::FromString(FString::FromInt(NewCurrentConnection)));
	}
}

void UZSessionListItemWidget::UpdateMaxConnection(int32 NewMaxConnection)
{
	if (MaxConnection)
	{
		MaxConnection->SetText(FText::FromString(FString::FromInt(NewMaxConnection)));
	}
}

FReply UZSessionListItemWidget::NativeOnMouseButtonDoubleClick(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Double click!"));
	auto Result = Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("In double click!"));

	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	if (MyGameInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("In double click1!"));
		auto PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("In double click2!"));
			auto PS = PC->GetPlayerState<APlayerState>();
			if (PS)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("In double click3!"));
				MyGameInstance->SessionJoinByIndex(PS->UniqueId.GetUniqueNetId(), NAME_GameSession, SessionInfo.SessionIndex);
			}
		}
	}

	return Result;
}
