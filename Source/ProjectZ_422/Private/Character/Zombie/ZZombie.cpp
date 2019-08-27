// Fill out your copyright notice in the Description page of Project Settings.


#include "ZZombie.h"
#include "ZZombieAIController.h"
#include "ZCharacter.h"
#include "ZZombieAnimInstance.h"
#include "ZGameMode.h"
#include "ZCharacterStatusComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ConstructorHelpers.h"
#include "PhysicalMaterials/PhysicalMaterial.h"


AZZombie::AZZombie()
{

}

void AZZombie::BeginPlay()
{
	Super::BeginPlay();

}

float AZZombie::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* PointDamage = (FPointDamageEvent*)(&DamageEvent);
		UPhysicalMaterial* PhysicalMaterial = PointDamage->HitInfo.PhysMaterial.Get();
		/* Headshot은 즉사 */
		if (PhysicalMaterial->SurfaceType == SURFACE_HEAD)
		{
			ZLOG(Error, TEXT("Headshot."));
			DamageAmount = StatusComponent->GetMaxHP();
		}
		
	}
	
	//StatusComponent->AdjustCurrentHP(-FinalDamage);

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AZZombie::OnSensingPlayer(APawn * Pawn)
{
	Super::OnSensingPlayer(Pawn);

	if (GetZombieState() != EZombieState::Idle)
	{
		return;
	}

	auto ZombieController = Cast<AZZombieAIController>(GetController());
	if (nullptr == ZombieController)
	{
		return;
	}

	//if (ZombieController->GetTargetPawn())
	//{
	//	return;
	//}

	auto Player = Cast<AZCharacter>(Pawn);
	if (Player)
	{
		if (!Player->IsDead())
		{
			/* Target 설정 */
			ZombieController->SetTargetPawn(Player);
			ChangeZombieState(EZombieState::Chase);
		}
	}
}

void AZZombie::Revive()
{
	Super::Revive();

}

void AZZombie::SetActive(bool bActive)
{
	Super::SetActive(bActive);

}

void AZZombie::ChangeZombieState(EZombieState NewState)
{
	Super::ChangeZombieState(NewState);

	switch (NewState)
	{
		case EZombieState::Idle:
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			break;
		}
		case EZombieState::Chase:
		{
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
			break;
		}
		case EZombieState::Attack:
		{
			break;
		}
		case EZombieState::Dead:
		{
			break;
		}
		default:
		{
			break;
		}

	}

}


void AZZombie::AttackCheck()
{
	Super::AttackCheck();

	if (IsDead())
	{
		return;
	}

	TArray<FHitResult> Hits;
	FCollisionQueryParams CollisionParams(TEXT("EnemyAttackParam"), false, this);
	CollisionParams.bReturnPhysicalMaterial = false;
	CollisionParams.bTraceComplex = false;

	bool bResult = GetWorld()->SweepMultiByProfile(Hits, GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity, TEXT("EnemyAttack"), FCollisionShape::MakeSphere(AttackRadius), CollisionParams);

	ZLOG(Error, TEXT("Hits : %d"), Hits.Num());
	if (!bResult)
	{
		return;
	}

	for (const auto& Hit : Hits)
	{
		ZLOG(Error, TEXT("Name : %s"), *Hit.Actor->GetName());
		auto Character = Cast<AZCharacter>(Hit.Actor);
		if (nullptr == Character)
		{
			continue;
		}
		Character->TakeDamage(AttackDamage, FDamageEvent(), Controller, this);

	}


}

void AZZombie::OnDead()
{
	Super::OnDead();



}

