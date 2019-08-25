// Fill out your copyright notice in the Description page of Project Settings.


#include "ZZombieAIController.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ConstructorHelpers.h"

AZZombieAIController::AZZombieAIController()
{
	//static ConstructorHelpers::FObjectFinder<UBehaviorTree>
	//	BT_ZOMBIE(TEXT("BehaviorTree'/Game/AI/Zombie/BT_ZZombie.BT_ZZombie'"));
	//if (BT_ZOMBIE.Succeeded())
	//{
	//	ZombieBT = BT_ZOMBIE.Object;
	//}

	//static ConstructorHelpers::FObjectFinder<UBlackboardData>
	//	BB_ZOMBIE(TEXT("BlackboardData'/Game/AI/Zombie/BB_ZZomble.BB_ZZomble'"));
	//if (BB_ZOMBIE.Succeeded())
	//{
	//	ZombieBB = BB_ZOMBIE.Object;
	//}

	HomePosKey = TEXT("HomePos");
	TargetPosKey = TEXT("TargetPos");
	TargetActorKey = TEXT("TargetPawn");
	CurrentStateKey = TEXT("CurrentState");
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
		if (!UseBlackboard(Zombie->GetZombieBT()->BlackboardAsset, Blackboard))
		{
			ZLOG(Error, TEXT("Failed to init blackboard."));
			return;
		}
	}

	RunAI();

}

void AZZombieAIController::OnUnPossess()
{
	//StopAI(TEXT("UnPossess"));
	StopAI();

	Super::OnUnPossess();
}

bool AZZombieAIController::RunAI()
{
	//if (!UseBlackboard(ZombieBB, Blackboard))
	//{
	//	return false;
	//}
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
			임시로 하드코딩함. 
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
	// TODO: 여기에 반환 구문을 삽입합니다.
	return HomePosKey;
}

const FName & AZZombieAIController::GetTargetPosKey() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return TargetPosKey;
}

const FName & AZZombieAIController::GetTargetActorKey() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return TargetActorKey;
}

const FName & AZZombieAIController::GetCurrentStateKey() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return CurrentStateKey;
}
