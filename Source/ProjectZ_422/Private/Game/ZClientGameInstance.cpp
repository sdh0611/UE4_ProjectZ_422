// Fill out your copyright notice in the Description page of Project Settings.


#include "ZClientGameInstance.h"
#include "Engine/LocalPlayer.h"
#include "ZBasePlayerController.h"
#include "Json.h"
#include "JsonObjectConverter.h"


void UZClientGameInstance::Shutdown()
{
	


	Super::Shutdown();
}

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

	FString PostParameters = FString::Printf(TEXT("sessionID=%s&playerID=%s"), *SessionID, *WebConnector->GetUserNickname());

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

const FString & UZClientGameInstance::GetPlayerSessionID() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return PlayerSessionID;
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

		//DescribeGameSession(SessionID);

		/* GameLiftLocal 테스트용 코드 */
		CreatePlayerSession(SessionID);
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

		if (!JsonObject->TryGetStringField(TEXT("playerSessionID"), PlayerSessionID))
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
		ZLOG(Error, TEXT("Search game http request fail.."));
		return;
	}

	TArray<TSharedPtr<FJsonValue>> JsonArray;
	TArray<FZSessionInfo> SessionInfos;
	FJsonObjectConverter::JsonArrayToUStruct<FZSessionInfo>(JsonArray, &SessionInfos, 0, 0);


	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	ZLOG(Error, TEXT("%s"), *Response->GetContentAsString());
	
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		ZLOG_S(Error);
		if (!JsonObject->GetBoolField(TEXT("result")))
		{
			ZLOG_S(Error);
			return;
		}

		FZSearchSessionData SearchSessionData;
		if (!FJsonObjectConverter::JsonObjectStringToUStruct<FZSearchSessionData>(
			Response->GetContentAsString(), &SearchSessionData, 0, 0))
		{
			ZLOG(Error, TEXT("Failed to convert json."));
			return;
		}

		if (!SearchSessionData.Result)
		{
			ZLOG(Error, TEXT("Failed to get session info."));
			return;
		}

		OnSearchGameSessionsSuccesss.Execute(SearchSessionData.SessionInfos);


	}
	
	//if (FJsonSerializer::Deserialize(Reader, JsonObject))
	//{
	//	int32 Size = 0;
	//	if (!JsonObject->TryGetNumberField(TEXT("size"), Size))
	//	{
	//		ZLOG_S(Error);
	//		return;
	//	}

	//	if (Size < 1)
	//	{
	//		ZLOG_S(Error);
	//		return;
	//	}

	//	ZLOG_S(Error);
	//	const TSharedPtr<FJsonObject>* OutObjects;
	//	if (!JsonObject->TryGetObjectField(TEXT("gameSessions"), OutObjects))
	//	{
	//		ZLOG_S(Error);
	//		return;
	//	}

	//	TArray<FZSessionInfo> SessionInfos;
	//	FZSessionInfo SessionInfo;
	//	for (int i = 0; i < Size; ++i)
	//	{
	//		if (!OutObjects[i]->TryGetStringField(TEXT("GameSessionId"), SessionInfo.SessionID))
	//		{
	//			ZLOG_S(Error);
	//			return;
	//		}

	//		if (!OutObjects[i]->TryGetStringField(TEXT("Name"), SessionInfo.SessionName))
	//		{
	//			ZLOG_S(Error);
	//			return;
	//		}

	//		if (!OutObjects[i]->TryGetNumberField(TEXT("MaximumPlayerSessionCount"), SessionInfo.MaxConnection))
	//		{
	//			ZLOG_S(Error);
	//			return;
	//		}

	//		if (!OutObjects[i]->TryGetNumberField(TEXT("CurrentPlayerSessionCount"), SessionInfo.CurrentConnection))
	//		{
	//			ZLOG_S(Error);
	//			return;
	//		}
	//		
	//		ZLOG(Error, TEXT("ID : %s"), *SessionInfo.SessionName);
	//		SessionInfos.Add(SessionInfo);
	//	}

	//	OnSearchGameSessionsSuccesss.Execute(SessionInfos);
	//}
	//else
	//{
	//	ZLOG(Error, TEXT("Deserialize fail.."));
	//}



}
