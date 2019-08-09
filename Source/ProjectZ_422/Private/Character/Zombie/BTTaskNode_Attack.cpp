// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Attack.h"
#include "ZZombie.h"
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

	auto Zombie = Cast<AZZombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Zombie)
	{
		return EBTNodeResult::Failed;
	}

	/* Turn to target. */
	auto Target = Cast<AZBaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AZZombieAIController::TargetActorKey));
	if (nullptr == Target)
	{
		return EBTNodeResult::Failed;
	}

	FVector Look = Target->GetActorLocation() - Zombie->GetActorLocation();
	Look.Z = 0.f;
	FRotator ToTarget = FRotationMatrix::MakeFromX(Look).Rotator();
	Zombie->SetActorRotation(ToTarget);

	/* Attack */
	bIsAttacking = true;
	Zombie->Attack();

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

