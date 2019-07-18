// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_TurnToTarget.h"
#include "ZZombie.h"
#include "ZZombieAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_TurnToTarget::UBTTaskNode_TurnToTarget()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTaskNode_TurnToTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	auto Owner = Cast<AZZombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Owner)
	{
		return EBTNodeResult::Failed;
	}
	OwnerPawn = Owner;

	/* Turn to target. */
	auto Target = Cast<AZBaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AZZombieAIController::TargetActorKey));
	if (nullptr == Target)
	{
		return EBTNodeResult::Failed;
	}
	TargetPawn = Target;

	//FVector Look = Target->GetActorLocation() - Owner->GetActorLocation();
	//Look.Z = 0.f;
	//FRotator ToTarget = FRotationMatrix::MakeFromX(Look).Rotator();
	//Owner->SetActorRotation(FMath::RInterpTo(Owner->GetActorRotation(), ToTarget, GetWorld()->GetDeltaSeconds(), 2.f));


	return EBTNodeResult::InProgress;
}

void UBTTaskNode_TurnToTarget::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSecond)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSecond);

	FVector Look = TargetPawn->GetActorLocation() - OwnerPawn->GetActorLocation();
	Look.Z = 0.f;
	FRotator ToTarget = FRotationMatrix::MakeFromX(Look).Rotator();
	OwnerPawn->SetActorRotation(FMath::RInterpTo(OwnerPawn->GetActorRotation(), ToTarget, GetWorld()->GetDeltaSeconds(), 10.f));

	if (OwnerPawn->GetActorRotation().Yaw >= ToTarget.Yaw - 10.f
		&&OwnerPawn->GetActorRotation().Yaw <= ToTarget.Yaw + 10.f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
