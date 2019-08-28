// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CheckRange::UBTDecorator_CheckRange()
{
	NodeName = TEXT("CheckRange");
}

bool UBTDecorator_CheckRange::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	auto Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
	if(nullptr == Target)
	{
		return false;
	}

	float Distance = FVector::Distance(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), Target->GetActorLocation());

	bool bResult = false;	

	switch (Condition)
	{
		case ETaskCondition::Less:
		{
			if (Distance <= Range)
			{
				bResult = true;
			}
			break;
		}
		case ETaskCondition::Greater:
		{
			if (Distance > Range)
			{
				bResult = true;
			}
			break;
		}

	}

	return bResult;
}
