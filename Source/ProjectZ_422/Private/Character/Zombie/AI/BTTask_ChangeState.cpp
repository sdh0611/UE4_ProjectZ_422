// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChangeState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "ZBaseZombie.h"

UBTTask_ChangeState::UBTTask_ChangeState()
{
	NodeName = TEXT("ChangeState");
}

EBTNodeResult::Type UBTTask_ChangeState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	auto Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Zombie = Cast<AZBaseZombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Zombie)
	{
		return EBTNodeResult::Failed;
	}
	Zombie->ChangeZombieState(TargetState);
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)TargetState);


	return EBTNodeResult::Succeeded;
}
