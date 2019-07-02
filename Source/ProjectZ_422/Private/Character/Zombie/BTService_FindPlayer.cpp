// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FindPlayer.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ZZombie.h"
#include "ZZombieAIController.h"
#include "ZCharacter.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"


UBTService_FindPlayer::UBTService_FindPlayer()
{

}

void UBTService_FindPlayer::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto ZombieController = Cast<AZZombieAIController>(OwnerComp.GetOwner());
	if (nullptr == ZombieController)
	{
		//ZLOG(Error, TEXT("%s"), *OwnerComp.GetOwner()->GetName());
		return;
	}

	auto Player = SweepTrace(ZombieController->GetPawn());
	if (nullptr != Player)
	{
		ZLOG_S(Error);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AZZombieAIController::TargetPosKey, Player->GetActorLocation());

	}

}

AZCharacter* UBTService_FindPlayer::SweepTrace(class APawn* Zombie)
{
	TArray<AZCharacter*> Characters;

	TArray<FHitResult> Hits;

	FVector ActorLoc = Zombie->GetActorLocation();

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Zombie);
	CollisionParams.bTraceComplex = false;
	CollisionParams.bReturnPhysicalMaterial = false;

	FCollisionShape Shape = FCollisionShape::MakeSphere(1500.f);

	DrawDebugSphere(GetWorld(), ActorLoc, Shape.GetSphereRadius(), 100, FColor::Red, false, 0.5f);

	//bool bHit = GetWorld()->SweepMultiByChannel(Hits, ActorLoc, ActorLoc, FQuat::Identity, ECC_GameTraceChannel5, Shape, CollisionParams);
	//if (!bHit)
	//{
	//	/* 범위 내에 Actor가 없는 경우. */
	//	ZLOG_S(Error);
	//	return nullptr;
	//}

	AZCharacter* Player = nullptr;
	for (const auto& Hit : Hits)
	{
		Player = Cast<AZCharacter>(Hit.Actor);
		if (Player)
		{
			return Player;
		}
	}

	return nullptr;
}
