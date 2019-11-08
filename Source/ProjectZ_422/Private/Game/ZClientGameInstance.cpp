// Fill out your copyright notice in the Description page of Project Settings.


#include "ZClientGameInstance.h"
#include "Engine/LocalPlayer.h"
#include "ZBasePlayerController.h"

#include "GameLiftClientSDK.h"
#include "GameLiftClientObject.h"
#include "GameLiftClientApi.h"

UZClientGameInstance::UZClientGameInstance()
{
}

void UZClientGameInstance::Init()
{
	if (!IsDedicatedServerInstance())
	{
		/* Client ฐüทร */
		GameLiftClientObject = UGameLiftClientObject::CreateGameLiftObject(GameLiftAccessKey, GameLiftSecretAccessKey);
		if (nullptr == GameLiftClientObject)
		{
			ZLOG(Error, TEXT("Failed to create GameLiftClientObject.."));
		}
	}

}

void UZClientGameInstance::CreateGameSession(const FString& SessionName, int32 MaxPlayer)
{
	if (nullptr == GameLiftClientObject)
	{
		return;
	}

	FGameLiftGameSessionConfig MySessionConfig;
	MySessionConfig.SetAliasID(GameLiftFleetAliasID);
	MySessionConfig.SetMaxPlayers(MaxPlayer);
	
	FGameLiftGameSessionServerProperties Property;
	Property.Key = TEXT("SessionName");
	Property.Value = SessionName;
	MySessionConfig.SetGameSessionProperties(TArray<FGameLiftGameSessionServerProperties>({ Property }));

	UGameLiftCreateGameSession* MyGameSessionObject = GameLiftClientObject->CreateGameSession(MySessionConfig);
	if (nullptr == MyGameSessionObject)
	{
		ZLOG_S(Error);
		return;
	}

	MyGameSessionObject->OnCreateGameSessionSuccess.AddDynamic(this, &UZClientGameInstance::OnGameCreationSuccess);
	MyGameSessionObject->OnCreateGameSessionFailed.AddDynamic(this, &UZClientGameInstance::OnGameCreationFailed);
	MyGameSessionObject->Activate();
}

void UZClientGameInstance::DescribeGameSession(const FString & GameSessionID)
{
	if (nullptr == GameLiftClientObject)
	{
		return;
	}

	UGameLiftDescribeGameSession* MyDescribeGameSessionObject = GameLiftClientObject->DescribeGameSession(GameSessionID);
	if (nullptr == MyDescribeGameSessionObject)
	{
		ZLOG_S(Error);
		return;
	}
	MyDescribeGameSessionObject->OnDescribeGameSessionStateSuccess.AddDynamic(this, &UZClientGameInstance::OnDescribeGameSessionSuccess);
	MyDescribeGameSessionObject->OnDescribeGameSessionStateFailed.AddDynamic(this, &UZClientGameInstance::OnDescribeGameSessionFailed);
	MyDescribeGameSessionObject->Activate();
}

void UZClientGameInstance::CreatePlayerSession(const FString & GameSessionID, const FString & UniquePlayerID)
{
	if (nullptr == GameLiftClientObject)
	{
		return;
	}

	UGameLiftCreatePlayerSession* MyCreatePlayerSessionObject = GameLiftClientObject->CreatePlayerSession(GameSessionID, UniquePlayerID);
	if (nullptr == MyCreatePlayerSessionObject)
	{
		ZLOG_S(Error);
		return;
	}
	MyCreatePlayerSessionObject->OnCreatePlayerSessionSuccess.AddDynamic(this, &UZClientGameInstance::OnPlayerSessionCreateSuccess);
	MyCreatePlayerSessionObject->OnCreatePlayerSessionFailed.AddDynamic(this, &UZClientGameInstance::OnPlayerSessionCreateFailed);
	MyCreatePlayerSessionObject->Activate();
}

void UZClientGameInstance::SearchSessions()
{
	UGameLiftSearchGameSessions* MySearchSessionsObject = 
		GameLiftClientObject->SearchGameSessions(GameLiftFleetID, GameLiftFleetAliasID, TEXT("hasAvailablePlayerSessions=true"), TEXT(""));
	if (nullptr == MySearchSessionsObject)
	{
		ZLOG_S(Error);
		return;
	}
	
	MySearchSessionsObject->OnSearchGameSessionsSuccess.AddDynamic(this, &UZClientGameInstance::OnSearchSessionsSuccess);
	MySearchSessionsObject->OnSearchGameSessionsFailed.AddDynamic(this, &UZClientGameInstance::OnSearchSessionsFailed);
	MySearchSessionsObject->Activate();

}

void UZClientGameInstance::OnGameCreationSuccess(const FString & GameSessionID)
{
	DescribeGameSession(GameSessionID);

}

void UZClientGameInstance::OnGameCreationFailed(const FString & ErrorMessage)
{

}

void UZClientGameInstance::OnDescribeGameSessionSuccess(const FString & SessionID, EGameLiftGameSessionStatus SessionStatus)
{
	if (SessionStatus == EGameLiftGameSessionStatus::STATUS_Active)
	{
		CreatePlayerSession(SessionID, WebConnector->GetUserNickname());
	}
}

void UZClientGameInstance::OnDescribeGameSessionFailed(const FString & ErrorMessage)
{


}

void UZClientGameInstance::OnPlayerSessionCreateSuccess(const FString & IPAddress, const FString & Port, const FString & PlayerSessionID, const int & PlayerSessionStatus)
{
	const FString TravelURL = IPAddress + ":" + Port;
	auto PC = GetFirstGamePlayer()->GetPlayerController(GetWorld());
	if (PC)
	{
		PC->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
	}
}

void UZClientGameInstance::OnPlayerSessionCreateFailed(const FString & ErrorMessage)
{


}

void UZClientGameInstance::OnSearchSessionsSuccess(const TArray<FString>& GameSessionIds)
{
	for (const auto& GameSessionID : GameSessionIds)
	{
		DescribeGameSession(GameSessionID);
	}

}

void UZClientGameInstance::OnSearchSessionsFailed(const FString & ErrorMessage)
{
}

