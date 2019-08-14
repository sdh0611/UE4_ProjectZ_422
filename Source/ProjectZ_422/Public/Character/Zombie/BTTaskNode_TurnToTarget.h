// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_TurnToTarget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UBTTaskNode_TurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_TurnToTarget();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*   NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8*   NodeMemory, float DeltaSecond) override;


public:
	bool bIsAttacking = false;

protected:
	UPROPERTY()
	class APawn* OwnerPawn;

	UPROPERTY()
	class APawn* TargetPawn;
	

};
