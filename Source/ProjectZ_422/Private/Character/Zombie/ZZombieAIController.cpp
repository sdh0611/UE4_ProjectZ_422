// Fill out your copyright notice in the Description page of Project Settings.


#include "ZZombieAIController.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ConstructorHelpers.h"

const FName AZZombieAIController::HomePosKey(TEXT("HomePos"));
const FName AZZombieAIController::TargetPosKey(TEXT("TargetPos"));
const FName AZZombieAIController::TargetActorKey(TEXT("TargetActor"));

AZZombieAIController::AZZombieAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>
		BT_ZOMBIE(TEXT("BehaviorTree'/Game/AI/Zombie/BT_ZZombie.BT_ZZombie'"));
	if (BT_ZOMBIE.Succeeded())
	{
		ZombieBT = BT_ZOMBIE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData>
		BB_ZOMBIE(TEXT("BlackboardData'/Game/AI/Zombie/BB_ZZomble.BB_ZZomble'"));
	if (BB_ZOMBIE.Succeeded())
	{
		ZombieBB = BB_ZOMBIE.Object;
	}

	//HomePosKey = TEXT("HomePos");
	//TargetPosKey = TEXT("TargetPos");
	//TargetActorKey = TEXT("TargetActor");
}

void AZZombieAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);
	if (UseBlackboard(ZombieBB, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(ZombieBT))
		{
			ZLOG(Error, TEXT("Couldn't run BT."));
			return;
		}
	}



}

void AZZombieAIController::OnUnPossess()
{
	Super::OnUnPossess();



}
