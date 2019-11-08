// Fill out your copyright notice in the Description page of Project Settings.


#include "ZClientGameInstance.h"
#include "Engine/LocalPlayer.h"

#include "GameLiftClientSDK.h"
#include "GameLiftClientObject.h"
#include "GameLiftClientApi.h"

UZClientGameInstance::UZClientGameInstance()
{
	/* Client 관련 */
	GameLiftAccessKey = TEXT("AKIAUMWLOZXE6L2RTDFJ");
	GameLiftSecretAccessKey = TEXT("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");

}

void UZClientGameInstance::Init()
{
	if (!IsDedicatedServerInstance())
	{
		/* Client 관련 */
		GameLiftClientObject = UGameLiftClientObject::CreateGameLiftObject(GameLiftAccessKey, GameLiftSecretAccessKey);
		if (nullptr == GameLiftClientObject)
		{
			ZLOG(Error, TEXT("Failed to create GameLiftClientObject.."));
		}
	}

}

void UZClientGameInstance::CreateGameSession(const FString & AliasID, int32 MaxPlayer)
{
	if (nullptr == GameLiftClientObject)
	{
		return;
	}

	FGameLiftGameSessionConfig MySessionConfig;
	MySessionConfig.SetAliasID(AliasID);
	MySessionConfig.SetMaxPlayers(MaxPlayer);


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
	MyCreatePlayerSessionObject->OnCreatePlayerSessionFailed.AddDynamic(this, &UZClientGameInstance::OnPlayerSessionCreateFail);
	MyCreatePlayerSessionObject->Activate();
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
	if (SessionStatus == EGameLiftGameSessionStatus::STATUS_Active
		|| SessionStatus == EGameLiftGameSessionStatus::STATUS_Activating)
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

void UZClientGameInstance::OnPlayerSessionCreateFail(const FString & ErrorMessage)
{


}

