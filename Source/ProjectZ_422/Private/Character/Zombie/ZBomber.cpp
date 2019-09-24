// Fill out your copyright notice in the Description page of Project Settings.


#include "ZBomber.h"
#include "ZZombieAIController.h"
#include "ZCharacter.h"
#include "ZCharacterStatusComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
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

float AZBomber::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
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

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AZBomber::OnSphereOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, 
	UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		bIsTriggered = true;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
}

void AZBomber::OnSphereOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	bIsTriggered = false;
	ElapsedTime = 0.f;
}

void AZBomber::Explosion()
{
	/* Damage 적용 */
	UGameplayStatics::ApplyRadialDamage(GetWorld(), AttackDamage, GetActorLocation(), AttackRadius, nullptr, TArray<AActor*>(), this,
		GetController());
	
	RadialForce->FireImpulse();

	/* Sound 및 Particle 적용 */
	if (ExplosionSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation(), GetActorRotation());
	}

	if (ExplosionParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation(), GetActorRotation(), 
			FVector(4.f, 4.f, 4.f));
	}

	/* 폭발 후 처리방법 */
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
			/* Target 설정 */
			ZombieController->SetTargetActor(Player);
			ChangeZombieState(EZombieState::Chase);
		}
	}

}

void AZBomber::OnDead()
{
	bIsTriggered = false;
	ElapsedTime = 0.f;

	Super::OnDead();	
}
