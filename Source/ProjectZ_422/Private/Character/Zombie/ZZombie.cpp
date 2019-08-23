// Fill out your copyright notice in the Description page of Project Settings.


#include "ZZombie.h"
#include "ZZombieAIController.h"
#include "ZCharacter.h"
#include "ZZombieAnimInstance.h"
#include "ZGameMode.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ConstructorHelpers.h"


AZZombie::AZZombie()
{

}

void AZZombie::BeginPlay()
{
	Super::BeginPlay();

}

float AZZombie::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);


	return FinalDamage;
}

void AZZombie::OnSensingPlayer(APawn * Pawn)
{
	Super::OnSensingPlayer(Pawn);

	
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
		GetActorLocation() + GetActorForwardVector() * 200.f,
		FQuat::Identity, TEXT("EnemyAttack"), FCollisionShape::MakeSphere(50.f), CollisionParams);

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

