// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "ZZombie.h"
#include "ZZombieAIController.h"
#include "ZCharacter.h"
#include "BehaviorTree/BlackBoardComponent.h"


UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{

}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	auto OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == OwnerPawn)
	{
		return false;
	}

	auto TargetPawn = Cast<AZCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AZZombieAIController::TargetActorKey));
	if (nullptr == TargetPawn)
	{
		return false;
	}

	if (TargetPawn->IsDead())
	{
		return false;
	}

	bResult = (TargetPawn->GetDistanceTo(OwnerPawn) <= 150.f);

	return bResult;
}
