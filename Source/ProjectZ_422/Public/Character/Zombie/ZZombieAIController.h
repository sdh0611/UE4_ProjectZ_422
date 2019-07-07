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
	void SetTargetPawn(class APawn* Target);

public:
	/* 추적 실패할 시 돌아올 좌표값 키네임 */
	static const FName HomePosKey;

	/* 추적 대상의 실시간 좌표값 키네임 */
	static const FName TargetPosKey;

	/* 실시간 추적 대상 키네임 */
	static const FName TargetActorKey;
	

private:
	UPROPERTY()
	class UBehaviorTree* ZombieBT;

	UPROPERTY()
	class UBlackboardData* ZombieBB;

};
