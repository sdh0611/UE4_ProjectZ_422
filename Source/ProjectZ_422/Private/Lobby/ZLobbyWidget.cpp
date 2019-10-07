// Fill out your copyright notice in the Description page of Project Settings.


#include "ZLobbyWidget.h"
#include "..\..\Public\Lobby\ZLobbyWidget.h"
#include "Lobby/ZLobbyPlayerController.h"
#include "Lobby/ZLobbyGameMode.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "ZGameInstance.h"

void UZLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StartButton = Cast<UButton>(GetWidgetFromName(TEXT("BTN_Start")));
	check(StartButton);

	ConnectNumber = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_ConnectNumber")));
	check(ConnectNumber);


	InputChat = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ETB_InputChat")));
	check(InputChat);
	InputChat->OnTextCommitted.AddDynamic(this, &UZLobbyWidget::OnInputChatCommit);

	ChatBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ChatBox")));
	check(ChatBox);

	JoinPlayerBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("JoinPlayerBox")));
	check(JoinPlayerBox);

	StartButton->OnClicked.AddDynamic(this, &UZLobbyWidget::OnStartButtonClick);	

}

void UZLobbyWidget::UpdateConnectNumber(int32 NewNumber)
{
	if (nullptr == ConnectNumber)
	{
		return;
	}

	if (NewNumber < 0)
	{
		return;
	}

	ConnectNumber->SetText(FText::FromString(FString::FromInt(NewNumber)));

}

void UZLobbyWidget::UpdatePlayerName(const FString & PlayerName)
{
	auto Text = NewObject<UTextBlock>(JoinPlayerBox);
	Text->SetText(FText::FromString(PlayerName));
	JoinPlayerBox->AddChild(Text);

}

void UZLobbyWidget::UpdateChatBox(const FString& PlayerName, const FString& NewChat)
{
	FString Chat = FString::Printf(TEXT("%s : %s"), *PlayerName, *NewChat);
	
	UTextBlock* NewText = NewObject<UTextBlock>(ChatBox);
	NewText->SetText(FText::FromString(Chat));
	NewText->SetAutoWrapText(true);

	ChatBox->AddChild(NewText);
	ChatBox->ScrollToEnd();
	
}

void UZLobbyWidget::DeletePlayerName(const FString& PlayerName)
{
	auto Children = JoinPlayerBox->GetAllChildren();
	for (const auto& Child : Children)
	{
		auto Text = Cast<UTextBlock>(Child);
		if (Text)
		{
			if (Text->GetText().EqualTo(FText::FromString(PlayerName)))
			{
				Text->RemoveFromParent();
			}
		}
	}

}

bool UZLobbyWidget::IsValidPlayerIndex(int32 PlayerIndex) const
{
	if (!PlayerNameList.IsValidIndex(PlayerIndex))
	{
		ZLOG(Error, TEXT("Invalid player index.."));
		return false;
	}

	if (nullptr == PlayerNameList[PlayerIndex])
	{
		ZLOG(Error, TEXT("Invalid player text.."));
		return false;
	}

	return true;
}

void UZLobbyWidget::OnInputChatCommit(const FText & Text, ETextCommit::Type CommitMethod)
{
	switch (CommitMethod)
	{
		case ETextCommit::OnEnter:
		{
			auto MyGameInstance = GetGameInstance<UZGameInstance>();
			if (nullptr == MyGameInstance)
			{
				ZLOG(Error, TEXT("Invalid game instance.."));
				return;
			}

			auto MyPC = Cast<AZLobbyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			if (nullptr == MyPC)
			{
				return;
			}
			MyPC->ServerReceiveChat(MyGameInstance->GetWebConnector().GetUserNickname(), Text.ToString());

			if (InputChat)
			{
				InputChat->SetText(FText::GetEmpty());
			}
			break;
		}
		case ETextCommit::OnUserMovedFocus:
		{
			break;
		}


	}


}

void UZLobbyWidget::OnStartButtonClick()
{
	ZLOG_S(Error);

	auto MyGameMode = Cast<AZLobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (MyGameMode)
	{
		MyGameMode->StartGame();
	}
	else
	{
		ZLOG(Error, TEXT("No game mode.."));
	}


}
