// Fill out your copyright notice in the Description page of Project Settings.


#include "ZBaseZombie.h"
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



AZBaseZombie::AZBaseZombie()
{
	AIControllerClass = AZZombieAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Sense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSense"));
	Sense->SetPeripheralVisionAngle(60.f);
	Sense->SightRadius = 2000;
	Sense->HearingThreshold = 600;
	Sense->LOSHearingThreshold = 1200;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

	WalkSpeed = 200.f;
	SprintSpeed = 500.f;

	AttackDamage = 10.f;

	ZombieState = EZombieState::Idle;

	Tags.Add(TEXT("Enemy"));
}

void AZBaseZombie::BeginPlay()
{
	Super::BeginPlay();

	if (Sense)
	{
		Sense->OnSeePawn.AddDynamic(this, &AZBaseZombie::OnSeePlayer);
	}

	auto ZombieAnim = GetZombieAnimInstance();
	check(nullptr != ZombieAnim);
	ZombieAnim->OnAttackCheck.BindUObject(this, &AZBaseZombie::AttackCheck);
}

float AZBaseZombie::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (IsDead())
	{
		ZLOG_S(Warning);
		auto GameMode = Cast<AZGameMode>(GetWorld()->GetAuthGameMode());
		if (nullptr != GameMode)
		{
			GameMode->AdjustKillScore(EventInstigator, GetController(), this);
		}
	}

	return FinalDamage;
}

void AZBaseZombie::OnSeePlayer(APawn * Pawn)
{
	OnSensingPlayer(Pawn);
}

void AZBaseZombie::Attack()
{
	if (IsDead())
	{
		return;
	}

	ChangeZombieState(EZombieState::Attack);

}

void AZBaseZombie::AttackEnd()
{
	OnAttackEnd.Execute();
}

void AZBaseZombie::Revive()
{
	Super::Revive();

	auto ZombieAI = Cast<AZZombieAIController>(GetController());
	if (nullptr == ZombieAI)
	{
		return;
	}
	ZombieAI->RunAI();

	ChangeZombieState(EZombieState::Idle);
}

void AZBaseZombie::SetActive(bool bActive)
{
	Super::SetActive(bActive);

	if (bActive)
	{
		ZLOG_S(Error);
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("ZCharacter"));
		//GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
		//GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	}
}

void AZBaseZombie::ChangeZombieState(EZombieState NewState)
{
	ZombieState = NewState;

	auto ZombieAI = GetController<AZZombieAIController>();
	if (ZombieAI)
	{
		ZLOG_S(Error);
		ZombieAI->SetZombieCurrentState(NewState);
	}
}

UZZombieAnimInstance * const AZBaseZombie::GetZombieAnimInstance() const
{
	return Cast<UZZombieAnimInstance>(GetAnimInstance());
}

UBehaviorTree * const AZBaseZombie::GetZombieBT() const
{
	return ZombieBT;
}

EZombieState AZBaseZombie::GetZombieState() const
{
	return ZombieState;
}

void AZBaseZombie::AttackCheck()
{

}

void AZBaseZombie::OnDead()
{
	auto ZombieAI = Cast<AZZombieAIController>(GetController());
	if (nullptr == ZombieAI)
	{
		return;
	}
	ZombieAI->StopAI();

	//auto ZombieAnim = GetZombieAnimInstance();
	//if (nullptr == ZombieAnim)
	//{
	//	return;
	//}

	//if (ZombieAnim->GetCurrentPlayMontage())
	//{
	//	ZombieAnim->StopAllMontages(ZombieAnim->GetCurrentPlayMontage()->BlendOut.GetBlendTime());
	//}
	////ZombieAnim->PlayCharacterMontage(TEXT("Die"));

	ChangeZombieState(EZombieState::Dead);

}

void AZBaseZombie::OnSensingPlayer(APawn * Pawn)
{
	if (IsDead())
	{
		return;
	}

	if (GetZombieState() != EZombieState::Idle)
	{
		return;
	}

	//ZLOG_S(Warning);
	auto ZombieController = Cast<AZZombieAIController>(GetController());
	auto Player = Cast<AZCharacter>(Pawn);
	if (ZombieController && Player)
	{
		if (!Player->IsDead())
		{
			/* Target ¼³Á¤ */
			ZombieController->SetTargetPawn(Player);
			ChangeZombieState(EZombieState::Chase);
		}
	}
}
