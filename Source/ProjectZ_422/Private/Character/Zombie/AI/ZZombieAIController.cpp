// Fill out your copyright notice in the Description page of Project Settings.


#include "ZZombieAIController.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AZZombieAIController::AZZombieAIController()
{
	HomePosKey = TEXT("HomePos");
	TargetPosKey = TEXT("TargetPos");
	TargetActorKey = TEXT("TargetPawn");
	CurrentStateKey = TEXT("CurrentState");

	TargetPos = FVector(0.f, 0.f, 0.f);
}

void AZZombieAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);
	ZLOG_S(Warning);
	//if (UseBlackboard(ZombieBB, Blackboard))
	//{
	//	Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
	//	if (!RunBehaviorTree(ZombieBT))
	//	{
	//		ZLOG(Error, TEXT("Couldn't run BT."));
	//		return;
	//	}
	//}

	auto Zombie = Cast<AZBaseZombie>(GetPawn());
	if (Zombie)
	{
		auto BehaviorTree = Zombie->GetZombieBT();
		if (nullptr == BehaviorTree)
		{
			ZLOG(Error, TEXT("BehaviorTree not exist.."));
			return;
		}

		if (!UseBlackboard(Zombie->GetZombieBT()->BlackboardAsset, Blackboard))
		{
			ZLOG(Error, TEXT("Failed to init blackboard."));
			return;
		}
	}

	RunAI();

	if (Blackboard)
	{
		Blackboard->SetValueAsVector(TargetPosKey, TargetPos);
	}
}

void AZZombieAIController::OnUnPossess()
{
	//StopAI(TEXT("UnPossess"));
	StopAI();

	Super::OnUnPossess();
}

bool AZZombieAIController::RunAI()
{
	auto Zombie = Cast<AZBaseZombie>(GetPawn());
	if (nullptr == Zombie)
	{
		return false;
	}
	
	GetBlackboardComponent()->SetValueAsVector(HomePosKey, Zombie->GetActorLocation());
	if (!RunBehaviorTree(Zombie->GetZombieBT()))
	{
		ZLOG(Error, TEXT("Couldn't run BT."));
		return false;
	}
	
	GetBlackboardComponent()->SetValueAsEnum(CurrentStateKey, (uint8)EZombieState::Idle);

	return true;
}

void AZZombieAIController::StopAI(const FString & Reason)
{
	if (GetBrainComponent())
	{
		GetBrainComponent()->StopLogic(Reason);
	}
	/* 
		NOTE(7.27):
			�ӽ÷� �ϵ��ڵ���. 
	*/
	GetBlackboardComponent()->ClearValue(TargetActorKey);
}

void AZZombieAIController::StopAI()
{
	auto BTComponent = Cast<UBehaviorTreeComponent>(GetBrainComponent());
	if (BTComponent)
	{
		BTComponent->StopTree();
	}

}

void AZZombieAIController::SetTargetPawn(APawn* Target)
{
	if (Blackboard)
	{
		Blackboard->SetValueAsObject(TargetActorKey, Target);
		//Blackboard->SetValueAsVector(TargetPosKey, Target->GetActorLocation());
	}

}

void AZZombieAIController::SetZombieCurrentState(EZombieState NewState)
{
	if (Blackboard)
	{
		Blackboard->SetValueAsEnum(CurrentStateKey, (uint8)NewState);
	}

}

UObject * const AZZombieAIController::GetTargetPawn() const
{
	return Blackboard->GetValueAsObject(TargetActorKey);
}

const FName & AZZombieAIController::GetHomePosKey() const
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return HomePosKey;
}

const FName & AZZombieAIController::GetTargetPosKey() const
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return TargetPosKey;
}

const FName & AZZombieAIController::GetTargetActorKey() const
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return TargetActorKey;
}

const FName & AZZombieAIController::GetCurrentStateKey() const
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return CurrentStateKey;
}
