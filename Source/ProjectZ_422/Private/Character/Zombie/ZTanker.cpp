// Fill out your copyright notice in the Description page of Project Settings.


#include "ZTanker.h"
#include "ZTankerAnimInstance.h"
#include "ZCharacter.h"
#include "ZZombieAIController.h"
#include "ZCharacterStatusComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Controller.h"
#include "TimerManager.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"


AZTanker::AZTanker()
{
	bIsRushing = false;
	bIsScreaming = false;
	bIsRushCooldown = true;
	ImpulseStrength = 10000.f;
	RushSpeed = 1000.f;
	RushDamage = 60.f;
	RushDelay = 30.f;

	ImpulseSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ImpulseSphere"));
	ImpulseSphere->SetupAttachment(GetCapsuleComponent());
}

void AZTanker::BeginPlay()
{
	Super::BeginPlay();

	ImpulseSphere->OnComponentBeginOverlap.AddDynamic(this, &AZTanker::OnSphereOverlap);

}

void AZTanker::ChangeZombieState(EZombieState NewState)
{
	Super::ChangeZombieState(NewState);

	switch (NewState)
	{
		case EZombieState::Idle:
		{
			SetCurrentSpeed(WalkSpeed);
			break;
		}
		case EZombieState::Chase:
		{
			//SetCurrentSpeed(SprintSpeed);
			if (bIsRushCooldown)
			{
				ToggleRush(true);
			}
			else
			{
				ToggleRush(false);
			}
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

float AZTanker::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* PointDamage = (FPointDamageEvent*)(&DamageEvent);
		UPhysicalMaterial* PhysicalMaterial = PointDamage->HitInfo.PhysMaterial.Get();
		/* 헤드샷에는 데미지 250%, 나머지는 80% 적용 */
		if (PhysicalMaterial->SurfaceType == SURFACE_HEAD)
		{
			ZLOG(Error, TEXT("Headshot."));
			DamageAmount *= 5.f;
		}
		else
		{
			DamageAmount *= 0.8f;
		}

	}

	//StatusComponent->AdjustCurrentHP(-DamageAmount);

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AZTanker::AttackCheck()
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

void AZTanker::OnDead()
{
	Super::OnDead();
}

void AZTanker::OnSensingPlayer(APawn * Pawn)
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
			bIsScreaming = true;
		}
	}
	
}

void AZTanker::OnSphereOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, 
	UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ZLOG_S(Error);
	auto Character = Cast<AZBaseCharacter>(OtherActor);
	if (nullptr == Character)
	{
		return;
	}

	ZLOG(Warning, TEXT("%s"), *OtherComp->GetName());

	float CurrentSpeedRatio = GetVelocity().Size() / GetCharacterMovement()->MaxWalkSpeed;

	FVector ImpulseDir = (OtherActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	ImpulseDir *= (ImpulseStrength * CurrentSpeedRatio);

	Character->GetController()->StopMovement();
	Character->GetCharacterMovement()->AddImpulse(ImpulseDir, true);

	if (Character->ActorHasTag(TEXT("Player")))
	{
		//FTransform SpawnTransform;
		//SpawnTransform.SetLocation(SweepResult.ImpactPoint);
		//SpawnTransform.SetRotation(SweepResult.ImpactNormal.Rotation().Quaternion());
		//SpawnTransform.SetScale3D(FVector(3.f, 3.f, 3.f));

		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RushExplosionParticle, SpawnTransform);
		Character->TakeDamage(RushDamage * CurrentSpeedRatio, FDamageEvent(), GetController(), this);
		ToggleRush(false);
	}
}

bool AZTanker::IsRushCooldown() const
{
	return bIsRushCooldown;
}

void AZTanker::ToggleRush(bool bInRush)
{
	if (bInRush)
	{
		ImpulseSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SetCurrentSpeed(RushSpeed);
		bIsRushCooldown = false;
		//auto TankerAnim = Cast<UZTankerAnimInstance>(GetAnimInstance());
		//if (::IsValid(TankerAnim))
		//{
		//	TankerAnim->bIsRushCooldown = false;
		//}

		auto RushCooldownLambda = [this]()
		{
			bIsRushCooldown = true;
		};

		GetWorld()->GetTimerManager().SetTimer(RushTimer, RushCooldownLambda, RushDelay, false);
	}
	else
	{
		ImpulseSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetCurrentSpeed(SprintSpeed);

	}

	bIsRushing = bInRush;

}

