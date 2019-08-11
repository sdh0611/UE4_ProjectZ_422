// Fill out your copyright notice in the Description page of Project Settings.


#include "ZBulletProjectile.h"
#include "ZCharacter.h"
#include "ZBaseCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ConstructorHelpers.h"

AZBulletProjectile::AZBulletProjectile()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_PROJECTILE(TEXT("StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/Ammunition/SM_Shell_556x45.SM_Shell_556x45'"));
	if (SM_PROJECTILE.Succeeded())
	{
		ProjectileMesh->SetStaticMesh(SM_PROJECTILE.Object);
	}

	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

void AZBulletProjectile::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	if (OtherActor != this)
	{
		ZLOG_S(Warning);
		Destroy();
	}
}

void AZBulletProjectile::TraceBullet()
{
	FVector CurLocation = GetActorLocation();
	FCollisionQueryParams CollisionParams(NAME_None, false, this);
	CollisionParams.bReturnPhysicalMaterial = true;
	CollisionParams.AddIgnoredActor(Instigator);

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, PreLocation, CurLocation, ECollisionChannel::ECC_GameTraceChannel3, CollisionParams);
	if (Hit.bBlockingHit)
	{
		auto Character = Cast<AZBaseCharacter>(Hit.GetActor());
		if (nullptr == Character)
		{
			return;
		}
		ZLOG(Warning, TEXT("Trace Character"));
		if (Character->IsDead())
		{
			return;
		}			

		FPointDamageEvent DamageEvent;
		DamageEvent.HitInfo = Hit;
		DamageEvent.ShotDirection = CurLocation - PreLocation;
		DamageEvent.Damage = Damage;

		Character->TakeDamage(DamageEvent.Damage, DamageEvent, Instigator->GetController(), this);

		Destroy();
	}
}
