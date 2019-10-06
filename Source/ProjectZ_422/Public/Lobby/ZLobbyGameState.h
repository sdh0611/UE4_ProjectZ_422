// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/GameStateBase.h"
#include "ZLobbyGameState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZLobbyGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:
	void SetConnectNumber(int32 NewNumber);
	int32 GetConnectNumber() const;

private:
	UFUNCTION()
	void OnRep_ConnectNumber();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_ConnectNumber)
	int32 ConnectNumber = 0;

};
