// Fill out your copyright notice in the Description page of Project Settings.


#include "ZTanker.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"


AZTanker::AZTanker()
{
	bIsRushing = false;
	ImpulseStrength = 10000.f;
	RushDamage = 60.f;

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
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			break;
		}
		case EZombieState::Chase:
		{
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
			ToggleRush(true);
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

void AZTanker::AttackCheck()
{
	Super::AttackCheck();

}

void AZTanker::OnDead()
{
	Super::OnDead();
}

void AZTanker::OnSensingPlayer(APawn * Pawn)
{
	Super::OnSensingPlayer(Pawn);

	
}

void AZTanker::OnSphereOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ZLOG_S(Error);
	auto Character = Cast<AZBaseCharacter>(OtherActor);
	if (nullptr == Character)
	{
		return;
	}

	ZLOG_S(Error);

	float CurrentSpeedRatio = GetVelocity().Size() / GetCharacterMovement()->MaxWalkSpeed;

	FVector ImpulseDir = (OtherActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	ImpulseDir *= (ImpulseStrength * CurrentSpeedRatio);

	Character->GetCharacterMovement()->AddImpulse(ImpulseDir, true);

	if (Character->ActorHasTag(TEXT("Player")))
	{
		Character->TakeDamage(RushDamage * CurrentSpeedRatio, FDamageEvent(), GetController(), this);
		ToggleRush(false);
	}
}

void AZTanker::ToggleRush(bool bInRush)
{
	if (bInRush)
	{
		ImpulseSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		ImpulseSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	bIsRushing = bInRush;

}

