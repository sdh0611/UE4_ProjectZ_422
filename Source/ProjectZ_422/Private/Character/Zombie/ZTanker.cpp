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
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"


AZTanker::AZTanker()
{
	//bIsRushing = false;
	bIsRushCooldown = true;
	ImpulseStrength = 10000.f;
	RushSpeed = 1000.f;
	RushDamage = 60.f;
	RushDelay = 30.f;

	ImpulseSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ImpulseSphere"));
	ImpulseSphere->SetupAttachment(GetCapsuleComponent());

	StatusComponent->SetMaxHP(1000.f);
}

void AZTanker::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		ZLOG_S(Error);
		ImpulseSphere->OnComponentBeginOverlap.AddDynamic(this, &AZTanker::OnSphereOverlap);
	}
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

void AZTanker::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AZTanker, bIsRushing);
}

void AZTanker::SetActive(bool bActive)
{
	Super::SetActive(bActive);

	if (bActive)
	{
		GetWorld()->GetTimerManager().ClearTimer(RushTimer);
		bIsRushCooldown = true;
	}
	//else
	//{
		//bIsRushing = false;
	//}
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

	bool bResult = GetWorld()->SweepMultiByChannel(Hits, GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity, ENEMY_ATTACK, FCollisionShape::MakeSphere(AttackRadius), CollisionParams);

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

}

void AZTanker::OnSphereOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor,
	UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("SphereOverlap"));
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
		Character->TakeDamage(RushDamage * CurrentSpeedRatio, FDamageEvent(), GetController(), this);
		auto AI = GetController<AZZombieAIController>();
		if (AI)
		{
			auto Target = AI->GetTargetPawn();
			if (Target == Character)
			{
				ToggleRush(false);
			}
		}

	}
}

//bool AZTanker::IsRushing() const
//{
//	return bIsRushing;
//}

bool AZTanker::IsRushCooldown() const
{
	return bIsRushCooldown;
}

void AZTanker::ToggleRush(bool bInRush)
{
	if (!HasAuthority())
	{
		return;
	}

	if (bInRush)
	{
		ImpulseSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SetCurrentSpeed(RushSpeed);
		bIsRushCooldown = false;
		//auto TankerAnim = Cast<UZTankerAnimInstance>(GetAnimInstance());
		//if (::IsValid(TankerAnim))
		//{
		//	TankerAnim->bIsRushing = true;
		//}

		auto RushCooldownLambda = [this]()
		{
			bIsRushCooldown = true;
		};

		GetWorld()->GetTimerManager().SetTimer(RushTimer, RushCooldownLambda, RushDelay, false);

		MulticastPlayMontage(TEXT("Scream"));
	}
	else
	{
		ImpulseSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetCurrentSpeed(SprintSpeed);

	}

	//bIsRushing = bInRush;


}

