// Fill out your copyright notice in the Description page of Project Settings.


#include "ZBulletProjectile.h"
#include "ZCharacter.h"
#include "ZBaseCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInterface.h"
#include "Components/DecalComponent.h"

AZBulletProjectile::AZBulletProjectile()
{
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMesh->SetupAttachment(RootComponent);
	//Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Movement->InitialSpeed = 3000.f;
	Movement->ProjectileGravityScale = 0.f;

	PreLocation = FVector::ZeroVector;
}

void AZBulletProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);

	PreLocation = GetActorLocation();
}

void AZBulletProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TraceBullet();
}

void AZBulletProjectile::FireInDirection(const FVector & Direction)
{
	Movement->Velocity = Direction * Movement->InitialSpeed;
}

void AZBulletProjectile::SetBulletSpeed(float NewSpeed)
{
	if (NewSpeed < 0)
	{
		return;
	}

	Movement->MaxSpeed = NewSpeed;
	Movement->InitialSpeed = NewSpeed;

}

void AZBulletProjectile::SetBulletLifeSpan(float NewLifeSpan)
{
	if (NewLifeSpan < 0)
	{
		return;
	}

	SetLifeSpan(NewLifeSpan);

}

void AZBulletProjectile::TraceBullet()
{
	FVector CurLocation = GetActorLocation();

	FCollisionQueryParams CollisionParams(NAME_None, false, this);
	CollisionParams.bReturnPhysicalMaterial = true;
	CollisionParams.AddIgnoredActor(Instigator);

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, PreLocation, CurLocation, PROJECTILE_TRACE, CollisionParams);
	if (Hit.bBlockingHit)
	{
		bool bHitCharacter = false;
		if (Hit.GetActor()->ActorHasTag(TEXT("Character")))
		{
			auto Character = Cast<AZBaseCharacter>(Hit.GetActor());
			if (nullptr == Character)
			{
				return;
			}

			if (Character->IsDead() || !Character->IsActive())
			{
				return;
			}
			
			bHitCharacter = true;

			FPointDamageEvent DamageEvent;
			DamageEvent.HitInfo = Hit;
			/* 날아온 방향 */
			DamageEvent.ShotDirection = CurLocation - PreLocation;
			DamageEvent.Damage = Damage;

			Character->TakeDamage(DamageEvent.Damage, DamageEvent, Instigator->GetController(), this);
		}
		else
		{
			bHitCharacter = false;
		}
		MulticastSpawnHitEffect(bHitCharacter, Hit.ImpactPoint, Hit.ImpactNormal);
		Destroy();

	}
	else
	{
		PreLocation = GetActorLocation();
	}
	//ProjectileTrailParticle->SetVectorParameter(TEXT("EndPoint"), PreLocation);
}

void AZBulletProjectile::MulticastSpawnHitEffect_Implementation(bool bHitCharacter, const FVector & ImpactPoint, const FVector & HitDir)
{
	if (bHitCharacter)
	{
		if (PSHitCharacter)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PSHitCharacter, ImpactPoint + HitDir * 5.f, HitDir.Rotation());
		}
		
	}
	else
	{
		if (PSHitProps)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PSHitProps, ImpactPoint + HitDir * 5.f, HitDir.Rotation());
		}

		auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), Decal, FVector(5.f, 5.f, 5.f), ImpactPoint, HitDir.Rotation());
		if (DecalComponent)
		{
			DecalComponent->SetFadeScreenSize(0.0005f);
			DecalComponent->SetLifeSpan(4.f);
		}

	}

}

