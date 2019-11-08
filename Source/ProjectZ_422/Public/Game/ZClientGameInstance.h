// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Game/ZGameInstance.h"

#include "GameLiftClientTypes.h"

#include "ZClientGameInstance.generated.h"

/**
 *  NOTE(11.08) :
		AWS Client�� GameInstance.
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
	/* GameLiftClient ���� */

	/* Dedicated Server �ν��Ͻ� ���� ��û. */
	UFUNCTION(BlueprintCallable)
	void CreateGameSession(const FString& AliasID, int32 MaxPlayer);

	UFUNCTION(BlueprintCallable)
	void DescribeGameSession(const FString& GameSessionID);

	/* Join ��û. */
	UFUNCTION(BlueprintCallable)
	void CreatePlayerSession(const FString& GameSessionID, const FString& UniquePlayerID);

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
	void OnPlayerSessionCreateFail(const FString& ErrorMessage);

protected:
	UPROPERTY()
	class UGameLiftClientObject* GameLiftClientObject;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString GameLiftAccessKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString GameLiftSecretAccessKey;

};
