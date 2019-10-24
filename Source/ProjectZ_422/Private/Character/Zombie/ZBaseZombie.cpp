// Fill out your copyright notice in the Description page of Project Settings.


#include "ZBaseZombie.h"
#include "ZZombieAIController.h"
#include "ZCharacter.h"
#include "ZCharacterStatusComponent.h"
#include "ZZombieAnimInstance.h"
#include "ZGameMode.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealNetwork.h"

AZBaseZombie::AZBaseZombie()
{
	AIControllerClass = AZZombieAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Sense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSense"));
	Sense->SetPeripheralVisionAngle(60.f);
	Sense->SightRadius = 2000;
	Sense->HearingThreshold = 600;
	Sense->LOSHearingThreshold = 1200;

	// Create character status component
	StatusComponent = CreateDefaultSubobject<UZCharacterStatusComponent>(TEXT("StatusComponent"));

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ZEnemy"));
	GetMesh()->SetCollisionProfileName(TEXT("ZEnemyMesh"));

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);
	GetCharacterMovement()->bUseRVOAvoidance = true;

	WalkSpeed = 200.f;
	SprintSpeed = 500.f;

	ZombieValue = 100;
	ZombieScore = 100;

	AttackDamage = 10.f;
	AttackRange = 200.f;
	AttackRadius = 50.f;

	ZombieState = EZombieState::Idle;

	Tags.Add(TEXT("Enemy"));
}

void AZBaseZombie::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (Sense)
		{
			Sense->OnSeePawn.AddDynamic(this, &AZBaseZombie::OnSeePlayer);
		}

		auto ZombieAnim = GetZombieAnimInstance();
		if (!::IsValid(ZombieAnim))
		{
			ZLOG(Error, TEXT("Invalid zombie anim.."));
			return;
		}

		ZombieAnim->OnAttackCheck.BindUObject(this, &AZBaseZombie::AttackCheck);
	}
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

void AZBaseZombie::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZBaseZombie, ZombieState);

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
	if (HasAuthority())
	{
		OnAttackEnd.Execute();
	}
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

	ZLOG_S(Error);
	
	Sense->SetSensingUpdatesEnabled(bActive);

	if (bActive)
	{
		//GetCapsuleComponent()->SetCollisionProfileName(TEXT("ZCharacter"));
		//GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
		//GetMesh()->SetSimulatePhysics(false);
		//GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	}
	else
	{


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

int32 AZBaseZombie::GetZombieValue() const
{
	return ZombieValue;
}

int32 AZBaseZombie::GetZombieScore() const
{
	return ZombieScore;
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
	if (AttackSound)
	{
		UGameplayStatics::SpawnSoundAttached(AttackSound, RootComponent);
	}
}

void AZBaseZombie::OnDead()
{
	Super::OnDead();

	auto ZombieAI = Cast<AZZombieAIController>(GetController());
	if (nullptr == ZombieAI)
	{
		return;
	}
	ZombieAI->StopAI();

	if (OnEnemyDead.IsBound())
	{
		OnEnemyDead.Execute();
	}

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

void AZBaseZombie::OnRemoved()
{
	Super::OnRemoved();

	if (bIsPooling)
	{
		ChangeZombieState(EZombieState::Idle);
	}

}

void AZBaseZombie::OnSensingPlayer(APawn * Pawn)
{
	if (!IsActive())
	{
		return;
	}

	if (IsDead())
	{
		return;
	}

	if (GetZombieState() != EZombieState::Idle)
	{
		return;
	}

	auto ZombieController = Cast<AZZombieAIController>(GetController());
	if (nullptr == ZombieController)
	{
		return;
	}

	auto Player = Cast<AZCharacter>(Pawn);
	if (Player)
	{
		if (!Player->IsDead())
		{
			/* Target ¼³Á¤ */
			ZombieController->SetTargetPawn(Player);
			ChangeZombieState(EZombieState::Chase);
			if (DetectSound)
			{
				UGameplayStatics::SpawnSoundAttached(DetectSound, RootComponent);
			}

		}
	}

}

bool AZBaseZombie::ClientSetLocationAndRotation_Validate(const FVector & Location, const FRotator Rotation)
{
	return true;
}

void AZBaseZombie::ClientSetLocationAndRotation_Implementation(const FVector & Location, const FRotator Rotation)
{
	SetActorLocationAndRotation(Location, Rotation);
}
