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
	/* ���� ������ �� ���ƿ� ��ǥ�� Ű���� */
	static const FName HomePosKey;

	/* ���� ����� �ǽð� ��ǥ�� Ű���� */
	static const FName TargetPosKey;

	/* �ǽð� ���� ��� Ű���� */
	static const FName TargetActorKey;



private:
	UPROPERTY()
	class UBehaviorTree* ZombieBT;

	UPROPERTY()
	class UBlackboardData* ZombieBB;

};
