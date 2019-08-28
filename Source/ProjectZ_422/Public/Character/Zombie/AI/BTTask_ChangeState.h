// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "ZBaseZombie.h"
#include "BTTask_ChangeState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UBTTask_ChangeState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_ChangeState();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EZombieState TargetState;

};
