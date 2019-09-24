// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_TurnToTarget.h"
#include "ZBaseZombie.h"
#include "ZZombieAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/ZAITargetableInterface.h"

UBTTaskNode_TurnToTarget::UBTTaskNode_TurnToTarget()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTaskNode_TurnToTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	auto OwnerAI = Cast<AZZombieAIController>(OwnerComp.GetAIOwner());
	if (nullptr == OwnerAI)
	{
		return EBTNodeResult::Failed;
	}

	auto Zombie = Cast<AZBaseZombie>(OwnerAI->GetPawn());
	if (nullptr == Zombie)
	{
		return EBTNodeResult::Failed;
	}
	OwnerPawn = Zombie;


	/* Check is actor. */
	auto Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(OwnerAI->GetTargetActorKey()));
	if (nullptr == Target)
	{
		return EBTNodeResult::Failed;
	}
	
	/* Check is targetable. */
	auto Targetable = Cast<IZAITargetableInterface>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(OwnerAI->GetTargetActorKey()));
	if (nullptr == Targetable)
	{
		return EBTNodeResult::Failed;
	}
	

	/* Turn to target. */
	TargetPos = Target->GetActorLocation();

	return EBTNodeResult::InProgress;
}

void UBTTaskNode_TurnToTarget::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSecond)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSecond);

	FVector Look = TargetPos - OwnerPawn->GetActorLocation();
	Look.Z = 0.f;
	FRotator ToTarget = FRotationMatrix::MakeFromX(Look).Rotator();
	OwnerPawn->SetActorRotation(FMath::RInterpTo(OwnerPawn->GetActorRotation(), ToTarget, GetWorld()->GetDeltaSeconds(), 10.f));

	if (OwnerPawn->GetActorRotation().Yaw >= ToTarget.Yaw - 10.f
		&&OwnerPawn->GetActorRotation().Yaw <= ToTarget.Yaw + 10.f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}
