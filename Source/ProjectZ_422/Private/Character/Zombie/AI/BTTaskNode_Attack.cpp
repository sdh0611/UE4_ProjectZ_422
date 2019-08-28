// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Attack.h"
#include "ZBaseZombie.h"
#include "ZZombieAIController.h"
#include "ZZombieAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_Attack::UBTTaskNode_Attack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Zombie = Cast<AZBaseZombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Zombie)
	{
		return EBTNodeResult::Failed;
	}

	/* Attack */
	bIsAttacking = true;
	//Zombie->Attack();

	Zombie->OnAttackEnd.BindLambda([this]() { bIsAttacking = false; });


	return EBTNodeResult::InProgress;
}

void UBTTaskNode_Attack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSecond)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSecond);

	if (!bIsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}

