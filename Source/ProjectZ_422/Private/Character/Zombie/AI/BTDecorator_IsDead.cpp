// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsDead.h"
#include "ZBaseCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

bool UBTDecorator_IsDead::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto Target = Cast<AZBaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
	if (!::IsValid(Target))
	{
		return false;
	}

	//if (Target->IsDead())
	//{
	//	ZLOG(Error, TEXT("Target Dead."));
	//}
	//else
	//{
	//	ZLOG(Error, TEXT("Target Alive."));
	//}

	return Target->IsDead();
}
