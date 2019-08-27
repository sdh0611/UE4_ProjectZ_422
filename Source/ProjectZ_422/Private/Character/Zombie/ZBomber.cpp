// Fill out your copyright notice in the Description page of Project Settings.


#include "ZBomber.h"
#include "ZZombieAIController.h"
#include "ZCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"


AZBomber::AZBomber()
{
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(RootComponent);

	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForce->SetupAttachment(RootComponent);

	bIsTriggered = false;
	ExplosionDelay = 2.f;
	ElapsedTime = 0.f;
}

void AZBomber::BeginPlay()
{
	Super::BeginPlay();

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AZBomber::OnSphereOverlap);
	RadialForce->Radius = AttackRadius;
	//CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AZBomber::OnSphereOverlapEnd);
}

void AZBomber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsTriggered)
	{
		ElapsedTime += DeltaTime;
		if (ElapsedTime >= ExplosionDelay)
		{
			Explosion();
		}
	}

}

void AZBomber::OnSphereOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, 
	UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	bIsTriggered = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void AZBomber::OnSphereOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	bIsTriggered = false;
	ElapsedTime = 0.f;
}

void AZBomber::Explosion()
{
	/* Damage 利侩 */
	UGameplayStatics::ApplyRadialDamage(GetWorld(), AttackDamage, GetActorLocation(), AttackRadius, nullptr, TArray<AActor*>(), this,
		GetController());
	
	RadialForce->FireImpulse();

	/* Sound 棺 Particle 利侩 */
	if (ExplosionSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation(), GetActorRotation());
	}

	if (ExplosionParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation(), GetActorRotation(), 
			FVector(3.f, 3.f, 3.f));
	}

	/* 气惯 饶 贸府规过 */
	if (bIsPooling)
	{
		ElapsedTime = 0.f;
		bIsTriggered = false;
		SetActive(false);
	}
	else
	{
		Destroy();
	}

}

void AZBomber::ChangeZombieState(EZombieState NewState)
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

void AZBomber::OnSensingPlayer(APawn * Pawn)
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

	auto Player = Cast<AZCharacter>(Pawn);
	if (Player)
	{
		if (!Player->IsDead())
		{
			/* Target 汲沥 */
			ZombieController->SetTargetPawn(Player);
			ChangeZombieState(EZombieState::Chase);
		}
	}

}
