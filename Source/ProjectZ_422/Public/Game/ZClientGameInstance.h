// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Game/ZGameInstance.h"

#include "GameLiftClientTypes.h"

#include "ZClientGameInstance.generated.h"

/**
 *  NOTE(11.08) :
		AWS Client용 GameInstance.
 */
UCLASS()
class PROJECTZ_422_API UZClientGameInstance : public UZGameInstance
{
	GENERATED_BODY()
	
public:
	UZClientGameInstance();

public:
	virtual void Init() override;

public:
	/* GameLiftClient 관련 */

	/* Dedicated Server 인스턴스 생성 요청. */
	UFUNCTION(BlueprintCallable)
	void CreateGameSession(const FString& SessionName, int32 MaxPlayer);

	UFUNCTION(BlueprintCallable)
	void DescribeGameSession(const FString& GameSessionID);

	/* Join 요청. */
	UFUNCTION(BlueprintCallable)
	void CreatePlayerSession(const FString& GameSessionID, const FString& UniquePlayerID);

	UFUNCTION(BlueprintCallable)
	void SearchSessions();

private:
	UFUNCTION()
	void OnGameCreationSuccess(const FString& GameSessionID);

	UFUNCTION()
	void OnGameCreationFailed(const FString& ErrorMessage);

	UFUNCTION()
	void OnDescribeGameSessionSuccess(const FString& SessionID, EGameLiftGameSessionStatus SessionStatus);

	UFUNCTION()
	void OnDescribeGameSessionFailed(const FString& ErrorMessage);

	UFUNCTION()
	void OnPlayerSessionCreateSuccess(const FString& IPAddress, const FString& Port, const FString& PlayerSessionID, const int& PlayerSessionStatus);

	UFUNCTION()
	void OnPlayerSessionCreateFailed(const FString& ErrorMessage);
	
	UFUNCTION()
	void OnSearchSessionsSuccess(const TArray<FString>& GameSessionIds);

	UFUNCTION()
	void OnSearchSessionsFailed(const FString& ErrorMessage);


protected:
	UPROPERTY()
	class UGameLiftClientObject* GameLiftClientObject;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString GameLiftFleetAliasID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString GameLiftFleetID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString GameLiftAccessKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString GameLiftSecretAccessKey;

};
