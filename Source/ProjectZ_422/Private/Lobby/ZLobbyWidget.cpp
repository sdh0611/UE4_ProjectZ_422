// Fill out your copyright notice in the Description page of Project Settings.


#include "ZLobbyWidget.h"
#include "..\..\Public\Lobby\ZLobbyWidget.h"
#include "Lobby/ZLobbyPlayerController.h"
#include "Lobby/ZLobbyGameMode.h"
#include "ZPlayerState.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "ZGameInstance.h"
#include "EngineUtils.h"
#include "Engine/Engine.h"

void UZLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StartButton = Cast<UButton>(GetWidgetFromName(TEXT("BTN_Start")));
	check(StartButton);

	ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("BTN_Exit")));
	check(ExitButton);

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
	ExitButton->OnClicked.AddDynamic(this, &UZLobbyWidget::OnExitButtonClick);

	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	if (MyGameInstance)
	{
		UpdatePlayerName(MyGameInstance->GetWebConnector().GetUserNickname());
	}

	auto LobbyPC = GetOwningPlayer<AZLobbyPlayerController>();
	if (LobbyPC)
	{
		LobbyPC->ServerReceiveUpdateJoinPlayer();
	}

	

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
	ZLOG(Error, TEXT("PlayerName : %s"), *PlayerName);
	auto Text = NewObject<UTextBlock>(JoinPlayerBox);
	if (nullptr == Text)
	{
		ZLOG(Error, TEXT("Failed to create text block.."));
		return;
	}

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
	//auto MyGameInstnace = GetGameInstance<UZGameInstance>();
	//if (MyGameInstnace)
	//{
	//	MyGameInstnace->DestroySession();
	//}

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

void UZLobbyWidget::OnExitButtonClick()
{
	auto PC = GetOwningPlayer<AZBasePlayerController>();
	if (PC && PC->IsLocalPlayerController())
	{
		auto MyGameInstance = GetGameInstance<UZGameInstance>();
		if (MyGameInstance)
		{
			if (PC->GetNetMode() == ENetMode::NM_ListenServer)
			{
				ZLOG(Error, TEXT("Host : %s"), *MyGameInstance->GetWebConnector().GetIP());

				FString URL = *MyGameInstance->GetWebConnector().GetWebURL();
				URL.Append(TEXT("/delete_game"));

				FString PostParam = FString::Printf(TEXT("ip=%s"), *MyGameInstance->GetWebConnector().GetIP());
				MyGameInstance->GetWebConnector().HttpPost(URL, PostParam);


				auto LobbyGameMode = GetWorld()->GetAuthGameMode<AZLobbyGameMode>();
				if (LobbyGameMode)
				{
					LobbyGameMode->DestroyClientsSession();
				}
			}
			else
			{
				if (MyGameInstance->DestroySession())
				{
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Client DestroySession success")));
				}
			}
		}
		
		PC->ClientTravel(TEXT("StartMenu"), ETravelType::TRAVEL_Absolute);
		//UGameplayStatics::OpenLevel(GetWorld(), TEXT("StartMenu"));
		//GetWorld()->ServerTravel(TEXT("StartMenu"));
	}



}
