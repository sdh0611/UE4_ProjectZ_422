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

	//static ConstructorHelpers::FObjectFinder<UStaticMesh>
	//	SM_PROJECTILE(TEXT("StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/G67_Grenade/SM_G67.SM_G67'"));
	//if (SM_PROJECTILE.Succeeded())
	//{
	//	ProjectileMesh->SetStaticMesh(SM_PROJECTILE.Object);
	//}
	bReplicates = true;

	ProjectileMesh->SetCollisionProfileName(TEXT("Projectile"));
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);	

	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForce->SetupAttachment(RootComponent);
	RadialForce->AddCollisionChannelToAffect(ZCHARACTER);
	RadialForce->Radius = 600.f;
	RadialForce->ForceStrength = 50000.f;
	RadialForce->ImpulseStrength = 50000.f;

	Movement->InitialSpeed = 800.f;
	
	DamageCauser = nullptr;
}

void AZGrenadeProjectile::FireInDirection(const FVector & Direction)
{
	Movement->Velocity = Direction * Movement->InitialSpeed;
	//ZLOG(Error, TEXT("Dir : %.3f, %.3f, %.3f"), Direction.X, Direction.Y, Direction.Z);

	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Set explosion timer."));
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

void AZGrenadeProjectile::SetDamageCauser(AController* DamageInstigator)
{
	DamageCauser = DamageInstigator;
}

void AZGrenadeProjectile::Explosion()
{
	ZLOG_S(Warning);
	TArray<AActor*> IgnoreActors;
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), 600.f, DamageType, IgnoreActors ,this, DamageCauser);

	DrawDebugSphere(GetWorld(), GetActorLocation(), 600.f, 32, FColor::Red, false, 1.5f);

	RadialForce->FireImpulse();

	MulticastSpawnExplosion();

	//SetLifeSpan(3.f);
	Destroy();
}

void AZGrenadeProjectile::MulticastSpawnExplosion_Implementation()
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("SpawnExplosion."));

	if (ExplosionParticle)
	{
		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.Z += 50.f;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, SpawnLocation);
	}

	if (ExplosionSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	}
}
