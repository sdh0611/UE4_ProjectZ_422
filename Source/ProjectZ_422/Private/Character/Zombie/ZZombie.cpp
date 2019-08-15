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
	AIControllerClass = AZZombieAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


	Sense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSense"));
	Sense->SetPeripheralVisionAngle(60.f);
	Sense->SightRadius = 2000;
	Sense->HearingThreshold = 600;
	Sense->LOSHearingThreshold = 1200;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

	RightHandSocket = TEXT("attack_r");
	LeftHandSocket = TEXT("attack_l");

	WalkSpeed = 200.f;
	SprintSpeed = 500.f;

	AttackDamage = 10.f;
	
	ZombieState = EZombieState::Idle;

	Tags.Add(TEXT("Enemy"));
}

void AZZombie::BeginPlay()
{
	Super::BeginPlay();

	if (Sense)
	{
		Sense->OnSeePawn.AddDynamic(this, &AZZombie::OnSeePlayer);
	}

	auto ZombieAnim = GetZombieAnimInstance();
	check(nullptr != ZombieAnim);
	ZombieAnim->OnAttackCheck.BindUObject(this, &AZZombie::AttackCheck);
}

float AZZombie::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
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

void AZZombie::OnSeePlayer(APawn * Pawn)
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
			SetZombieState(EZombieState::Chase);
		}
	}
	
}

void AZZombie::Attack()
{
	if (IsDead())
	{
		return;
	}

	SetZombieState(EZombieState::Attack);
	
	//ZLOG_S(Warning);
	//auto ZombieAnim = GetZombieAnimInstance();
	//check(nullptr != ZombieAnim);
	//if (ZombieAnim->IsMontagePlaying(TEXT("Attack")))
	//{
	//	return;
	//}	
	//ZombieAnim->PlayMontage(TEXT("Attack"));
}

void AZZombie::AttackEnd()
{
	OnAttackEnd.Execute();
}

void AZZombie::Revive()
{
	Super::Revive();

	auto ZombieAI = Cast<AZZombieAIController>(GetController());
	if (nullptr == ZombieAI)
	{
		return;
	}
	ZombieAI->RunAI();

	SetZombieState(EZombieState::Idle);
}

void AZZombie::SetActive(bool bActive)
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

void AZZombie::SetZombieState(EZombieState NewState)
{
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

	ZombieState = NewState;

	auto ZombieAI = GetController<AZZombieAIController>();
	if (ZombieAI)
	{
		ZombieAI->SetZombieCurrentState(NewState);
	}
}

UZZombieAnimInstance * const AZZombie::GetZombieAnimInstance() const
{
	return Cast<UZZombieAnimInstance>(GetAnimInstance());
}

UBehaviorTree * const AZZombie::GetZombieBT() const
{
	return ZombieBT;
}

EZombieState AZZombie::GetZombieState() const
{
	return ZombieState;
}

void AZZombie::AttackCheck()
{
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

	auto ZombieAI = Cast<AZZombieAIController>(GetController());
	if(nullptr == ZombieAI)
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

	SetZombieState(EZombieState::Dead);


}

