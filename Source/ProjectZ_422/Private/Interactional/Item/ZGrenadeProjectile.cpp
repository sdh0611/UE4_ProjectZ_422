// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGrenadeProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "TimerManager.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"


AZGrenadeProjectile::AZGrenadeProjectile()
{
	Movement->ProjectileGravityScale = 0.f;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_PROJECTILE(TEXT("StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/G67_Grenade/SM_G67.SM_G67'"));
	if (SM_PROJECTILE.Succeeded())
	{
		ProjectileMesh->SetStaticMesh(SM_PROJECTILE.Object);
	}
	ProjectileMesh->SetCollisionProfileName(TEXT("Projectile"));
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProjectileMesh->SetSimulatePhysics(true);
	
	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForce->SetupAttachment(RootComponent);
	RadialForce->AddCollisionChannelToAffect(ECollisionChannel::ECC_GameTraceChannel5);
	RadialForce->Radius = 600.f;
	RadialForce->ForceStrength = 50000.f;
	RadialForce->ImpulseStrength = 50000.f;
	
	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	Particle->bAutoActivate = false;
	Particle->bAutoDestroy = false;
	Particle->SetupAttachment(RootComponent);

	/* Get grenade paticle */
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		PS_Grenade(TEXT("ParticleSystem'/Game/Effects/P_Explosion_Large.P_Explosion_Large'"));
	if (PS_Grenade.Succeeded())
	{
		//PSGrenade = PS_Grenade.Object;
		Particle->SetTemplate(PS_Grenade.Object);
	}

	Movement->InitialSpeed = 800.f;

}

void AZGrenadeProjectile::FireInDirection(const FVector & Direction)
{
	Movement->Velocity = Direction * Movement->InitialSpeed;
	
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimer, this, &AZGrenadeProjectile::Explosion, FireDelay, false);
}

void AZGrenadeProjectile::SetFireDelay(float NewFireDelay)
{
	if (NewFireDelay < 0.f)
	{
		FireDelay = 0.f;
	}
	else
	{
		FireDelay = NewFireDelay;
	}
	
}

void AZGrenadeProjectile::Explosion()
{
	ZLOG_S(Warning);
	TArray<AActor*> IgnoreActors;
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), 600.f, DamageType, IgnoreActors ,this, nullptr);
	DrawDebugSphere(GetWorld(), GetActorLocation(), 600.f, 32, FColor::Red, false, 1.5f);
	RadialForce->FireImpulse();
	Particle->ActivateSystem(true);
	SetLifeSpan(3.f);
	//Destroy();
}
