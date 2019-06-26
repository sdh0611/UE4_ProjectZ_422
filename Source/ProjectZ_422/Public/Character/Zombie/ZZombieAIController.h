// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "AIController.h"
#include "ZZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZZombieAIController : public AAIController
{
	GENERATED_BODY()

public:
	AZZombieAIController();

public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
public:
	static const FName HomePosKey;
	static const FName PatrolPosKey;


private:
	UPROPERTY()
	class UBehaviorTree* ZombieBT;

	UPROPERTY()
	class UBlackboardData* ZombieBB;

};
