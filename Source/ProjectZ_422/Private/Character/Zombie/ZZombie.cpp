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

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_ZOMBIE(TEXT("SkeletalMesh'/Game/Animation/Zombie/derrick.derrick'"));
	if (SK_ZOMBIE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_ZOMBIE.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88.f), FRotator(0, -90.f, 0));
	}

	static ConstructorHelpers::FClassFinder<UZCharacterAnimInstance>
		ANIM_ZOMBIE(TEXT("AnimBlueprint'/Game/Animation/Blueprint/ZZombie_AnimBlueprint.ZZombie_AnimBlueprint_C'"));
	if (ANIM_ZOMBIE.Succeeded())
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetMesh()->SetAnimInstanceClass(ANIM_ZOMBIE.Class);
	}

	Sense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSense"));
	Sense->SetPeripheralVisionAngle(60.f);
	Sense->SightRadius = 2000;
	Sense->HearingThreshold = 600;
	Sense->LOSHearingThreshold = 1200;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

	RightHandSocket = TEXT("attack_r");
	LeftHandSocket = TEXT("attack_l");

	AttackDamage = 10.f;
	bIsAttacking = false;
	
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

	//ZLOG_S(Warning);
	auto ZombieController = Cast<AZZombieAIController>(GetController());

	auto Player = Cast<AZCharacter>(Pawn);
	if (ZombieController && Player)
	{
		if (!Player->IsDead())
		{
			/* Target ���� */
			ZombieController->SetTargetPawn(Player);
		}
	}
	
}

void AZZombie::Attack()
{
	if (IsDead())
	{
		return;
	}

	//ZLOG_S(Warning);
	auto ZombieAnim = GetZombieAnimInstance();
	check(nullptr != ZombieAnim);
	if (ZombieAnim->IsMontagePlaying(TEXT("Attack")))
	{
		return;
	}
	
	ZombieAnim->PlayMontage(TEXT("Attack"));
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

UZZombieAnimInstance * const AZZombie::GetZombieAnimInstance() const
{
	return Cast<UZZombieAnimInstance>(GetAnimInstance());
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
	ZombieAI->StopAI(TEXT("Dead"));

	auto ZombieAnim = GetZombieAnimInstance();
	if (nullptr == ZombieAnim)
	{
		return;
	}

	if (ZombieAnim->GetCurrentPlayMontage())
	{
		ZombieAnim->StopAllMontages(ZombieAnim->GetCurrentPlayMontage()->BlendOut.GetBlendTime());
	}
	ZombieAnim->PlayMontage(TEXT("Die"));
	

}

