// Fill out your copyright notice in the Description page of Project Settings.


#include "ZZombieAIController.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/ZAITargetableInterface.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Navigation/CrowdFollowingComponent.h"

AZZombieAIController::AZZombieAIController()
{
	SetGenericTeamId(FGenericTeamId(0));

	HomePosKey = TEXT("HomePos");
	TargetPosKey = TEXT("TargetPos");
	TargetActorKey = TEXT("TargetPawn");
	CurrentStateKey = TEXT("CurrentState");

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectEnemies = false;
	SightConfig->DetectionByAffiliation.bDetectEnemies = false;
	SightConfig->SightRadius = 1000.f;
	SightConfig->LoseSightRadius = 1000.f;
	SightConfig->PeripheralVisionAngleDegrees = 80.f;
	SightConfig->SetMaxAge(3.f);

	AISense = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AISense"));
	AISense->ConfigureSense(*SightConfig);

	SetPathFollowingComponent(CreateDefaultSubobject<UCrowdFollowingComponent>(TEXT("PathComponent.")));

}

void AZZombieAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);
	ZLOG_S(Warning);

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
		Blackboard->SetValueAsVector(TargetPosKey, FVector(0.f, 0.f, 0.f));
	}
}

void AZZombieAIController::OnUnPossess()
{
	StopAI();

	Super::OnUnPossess();
}

void AZZombieAIController::BeginPlay()
{
	Super::BeginPlay();

	AISense->OnPerceptionUpdated.AddDynamic(this, &AZZombieAIController::OnPerceptionUpdate);
	AISense->OnTargetPerceptionUpdated.AddDynamic(this, &AZZombieAIController::OnTargetPerceptionUpdate);

}

void AZZombieAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

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

void AZZombieAIController::SetTargetActor(AActor* Target)
{
	if (Blackboard)
	{
		Blackboard->SetValueAsObject(TargetActorKey, Target);
	}

}

void AZZombieAIController::SetZombieCurrentState(EZombieState NewState)
{
	if (Blackboard)
	{
		Blackboard->SetValueAsEnum(CurrentStateKey, (uint8)NewState);
	}

	auto MyPawn = Cast<AZBaseZombie>(GetPawn());
	if (::IsValid(MyPawn))
	{
		MyPawn->ChangeZombieState(NewState);
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

void AZZombieAIController::OnPerceptionUpdate(const TArray<AActor*>& UpdatedActors)
{
	auto MyPawn = Cast<AZBaseZombie>(GetPawn());
	if (!::IsValid(MyPawn))
	{
		return;
	}

	/* 이 부분 나중에 수정. (Queue로 Target목록 받아서 처리하게끔)*/
	if (EZombieState::Idle != MyPawn->GetZombieState())
	{
		return;
	}

	for (const auto& Actor : UpdatedActors)
	{
		auto Targetable = Cast<IZAITargetableInterface>(Actor);
		if(Targetable && !(Targetable->IsDead()))
		{
			SetTargetActor(Actor);
			SetZombieCurrentState(EZombieState::Chase);
		}
	}

}

void AZZombieAIController::OnTargetPerceptionUpdate(AActor * Actor, FAIStimulus Stimulus)
{


}
