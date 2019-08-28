// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckRange.h"
#include "ZZombieAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
//
//EBTNodeResult::Type UBTTask_CheckRange::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
//{
//	auto Result = Super::ExecuteTask(OwnerComp, NodeMemory);
//
//	auto ZombieAI = Cast<AZZombieAIController>(OwnerComp.GetAIOwner());
//	if (ZombieAI)
//	{
//		auto Zombie = Cast<AZZombie>(ZombieAI->GetPawn());
//		auto Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
//		if (!Zombie || !Player)
//		{
//			return EBTNodeResult::Failed;
//		}
//
//		float Distance = FVector::Distance(Zombie->GetActorLocation(), Player->GetActorLocation());
//		switch (Condition)
//		{
//			case ETaskCondition::Less:
//			{
//				if (Distance <= Range)
//				{
//					Zombie->ChangeZombieState(TargetState);
//					//OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)TargetState);
//				}
//
//				break;
//			}
//			case ETaskCondition::Greater:
//			{
//				if (Distance > Range)
//				{
//					Zombie->ChangeZombieState(TargetState);
//					//OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)TargetState);
//				}
//
//				break;
//			}
//
//		}
//
//	}
//
//
//	return Result;
//}
