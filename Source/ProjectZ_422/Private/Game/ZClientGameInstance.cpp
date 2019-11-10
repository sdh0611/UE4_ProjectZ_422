// Fill out your copyright notice in the Description page of Project Settings.


#include "ZClientGameInstance.h"
#include "Engine/LocalPlayer.h"
#include "ZBasePlayerController.h"
#include "Json.h"


void UZClientGameInstance::CreateGameSession(const FString & SessionName, int32 MaxPlayer)
{
	FString ConnectURL = WebConnector->GetGameLiftClientServiceURL();
	ConnectURL.Append(TEXT("/create_game_session"));

	FString PostParameters = FString::Printf(TEXT("maxPlayer=%d&creatorID=%s&sessionName=%s"), 
		MaxPlayer, *WebConnector->GetUserNickname(), *SessionName);

	WebConnector->HttpPost(ConnectURL, PostParameters,
		FHttpRequestCompleteDelegate::CreateUObject(this, &UZClientGameInstance::OnCreateGameSessionResponse));


}

void UZClientGameInstance::DescribeGameSession(const FString & SessionID)
{
	FString ConnectURL = WebConnector->GetGameLiftClientServiceURL();
	ConnectURL.Append(TEXT("/describe_game_session"));

	FString PostParameters = FString::Printf(TEXT("sessionID=%s"), *SessionID);

	WebConnector->HttpPost(ConnectURL, PostParameters,
		FHttpRequestCompleteDelegate::CreateUObject(this, &UZClientGameInstance::OnDescribeGameSessionResponse));


}

void UZClientGameInstance::CreatePlayerSession(const FString & SessionID)
{
	FString ConnectURL = WebConnector->GetGameLiftClientServiceURL();
	ConnectURL.Append(TEXT("/create_player_session"));

	FString PostParameters = FString::Printf(TEXT("sessionID=%s"), *SessionID);

	WebConnector->HttpPost(ConnectURL, PostParameters,
		FHttpRequestCompleteDelegate::CreateUObject(this, &UZClientGameInstance::OnCreatePlayerSessionResponse));


}

void UZClientGameInstance::SearchGameSessions(const FString & FilterExpression, const FString & SortExpression)
{
	FString ConnectURL = WebConnector->GetGameLiftClientServiceURL();
	ConnectURL.Append(TEXT("/search_game_sessions"));

	FString PostParameters = FString::Printf(TEXT("filterExpression=%s&sortExpression=%s"), *FilterExpression, *SortExpression);

	WebConnector->HttpPost(ConnectURL, PostParameters,
		FHttpRequestCompleteDelegate::CreateUObject(this, &UZClientGameInstance::OnSearchGameSessionsResponse));

}

void UZClientGameInstance::OnCreateGameSessionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		ZLOG(Error, TEXT("Create game http request fail.."));
		return;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	ZLOG(Error, TEXT("%s"), *Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		if (!JsonObject->GetBoolField(TEXT("result")))
		{
			return;
		}

		FString SessionID;
		if (!JsonObject->TryGetStringField(TEXT("sessionID"), SessionID))
		{
			ZLOG(Error, TEXT("Failed to load sessionID.."));
			return;
		}

		DescribeGameSession(SessionID);
	}
	else
	{
		ZLOG(Error, TEXT("Deserialize fail.."));
	}

	

}

void UZClientGameInstance::OnDescribeGameSessionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		ZLOG(Error, TEXT("Describe game http request fail.."));
		return;
	}
	
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	ZLOG(Error, TEXT("%s"), *Response->GetContentAsString());
	
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		if (!JsonObject->GetBoolField(TEXT("result")))
		{
			return;
		}

		FString Status;
		if (!JsonObject->TryGetStringField(TEXT("status"), Status))
		{
			return;
		}

		if (0 != Status.Compare(TEXT("ACTIVE")))
		{
			return;
		}

		FString SessionID;
		if (!JsonObject->TryGetStringField(TEXT("sessionID"), SessionID))
		{
			ZLOG(Error, TEXT("Failed to load sessionID.."));
			return;
		}

		CreatePlayerSession(SessionID);

	}
	else
	{
		ZLOG(Error, TEXT("Deserialize fail.."));
	}

}

void UZClientGameInstance::OnCreatePlayerSessionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		ZLOG(Error, TEXT("Create player session http request fail.."));
		return;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	ZLOG(Error, TEXT("%s"), *Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		if (!JsonObject->GetBoolField(TEXT("result")))
		{
			return;
		}

		FString Status;
		if (!JsonObject->TryGetStringField(TEXT("status"), Status))
		{
			return;
		}

		if (0 == Status.Compare(TEXT("COMPLETED"))
			|| 0 == Status.Compare(TEXT("TIMEDOUT")))
		{
			return;
		}


		FString IPAddress;
		if (!JsonObject->TryGetStringField(TEXT("ipAddress"), IPAddress))
		{
			return;
		}
		
		FString Port;
		if (!JsonObject->TryGetStringField(TEXT("port"), Port))
		{
			return;
		}
				
		FString TravelURL = IPAddress + TEXT(":") + Port;
		auto PC = GetFirstGamePlayer()->GetPlayerController(GetWorld());
		if (PC)
		{
			PC->ClientTravel(TravelURL, TRAVEL_Absolute);
		}

	}
	else
	{
		ZLOG(Error, TEXT("Deserialize fail.."));
	}

}

void UZClientGameInstance::OnSearchGameSessionsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	OnSearchGameSessionsEnd.Execute();

	if (!bWasSuccessful)
	{
		ZLOG(Error, TEXT("Create game http request fail.."));
		return;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	ZLOG(Error, TEXT("%s"), *Response->GetContentAsString());


	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		if (!JsonObject->GetBoolField(TEXT("result")))
		{
			return;
		}

		int32 Size = 0;
		if (!JsonObject->TryGetNumberField(TEXT("size"), Size))
		{
			return;
		}

		if (Size < 1)
		{
			return;
		}

		const TSharedPtr<FJsonObject>* OutObjects;
		if (!JsonObject->TryGetObjectField(TEXT("gameSessions"), OutObjects))
		{
			return;
		}

		TArray<FZSessionInfo> SessionInfos;
		FZSessionInfo SessionInfo;
		for (int i = 0; i < Size; ++i)
		{
			if (!OutObjects[i]->TryGetStringField(TEXT("GameSessionId"), SessionInfo.SessionID))
			{
				return;
			}

			if (!OutObjects[i]->TryGetStringField(TEXT("Name"), SessionInfo.SessionName))
			{
				return;
			}

			if (!OutObjects[i]->TryGetStringField(TEXT("CreatorId"), SessionInfo.CreatorName))
			{
				return;
			}

			if (!OutObjects[i]->TryGetNumberField(TEXT("MaximumPlayerSessionCount"), SessionInfo.MaxConnection))
			{
				return;
			}

			if (!OutObjects[i]->TryGetNumberField(TEXT("CurrentPlayerSessionCount"), SessionInfo.CurrentConnection))
			{
				return;
			}
			
			SessionInfos.Add(SessionInfo);
		}

		OnSearchGameSessionsSuccesss.Execute(SessionInfos);
	}
	else
	{
		ZLOG(Error, TEXT("Deserialize fail.."));
	}



}
