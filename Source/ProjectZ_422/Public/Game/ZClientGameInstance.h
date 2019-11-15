// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Game/ZGameInstance.h"
#include "ZClientGameInstance.generated.h"

DECLARE_DELEGATE_OneParam(FOnSearchGameSessionsSuccess, const TArray<FZSessionInfo>&);
DECLARE_DELEGATE(FOnSearchGameSessionsEnd);


USTRUCT(BlueprintType)
struct PROJECTZ_422_API FZSessionInfo
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString SessionID;

	UPROPERTY()
	FString SessionName;

	UPROPERTY()
	int32 MaxConnection = 4;

	UPROPERTY()
	int32 CurrentConnection = 0;
};

USTRUCT()
struct PROJECTZ_422_API FZSearchSessionData
{
	GENERATED_BODY()
	
	UPROPERTY()
	bool Result;

	UPROPERTY()
	TArray<FZSessionInfo> SessionInfos;
	
};


/**
 *  NOTE(11.08) :
		AWS Client�� GameInstance.
 */
UCLASS()
class PROJECTZ_422_API UZClientGameInstance : public UZGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Shutdown() override;


public:
	void CreateGameSession(const FString& SessionName, int32 MaxPlayer);
	
	void DescribeGameSession(const FString& SessionID);

	UFUNCTION(BlueprintCallable)
	void CreatePlayerSession(const FString& SessionID);

	void SearchGameSessions(const FString& FilterExpression, const FString& SortExpression);
	
	const FString& GetPlayerSessionID() const;

private:
	void OnCreateGameSessionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void OnDescribeGameSessionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void OnCreatePlayerSessionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void OnSearchGameSessionsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
public:
	FOnSearchGameSessionsSuccess OnSearchGameSessionsSuccesss;

	FOnSearchGameSessionsEnd OnSearchGameSessionsEnd;
	

private:
	FString PlayerSessionID;

};
