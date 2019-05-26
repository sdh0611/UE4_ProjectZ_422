// Fill out your copyright notice in the Description page of Project Settings.


#include "ZProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ConstructorHelpers.h"


// Sets default values
AZProjectile::AZProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	Sphere->InitSphereRadius(5.f);
	Sphere->OnComponentHit.AddDynamic(this, &AZProjectile::OnHit);
	RootComponent = Sphere;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(Sphere);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_PROJECTILE(TEXT("StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/Ammunition/SM_Shell_556x45.SM_Shell_556x45'"));
	if (SM_PROJECTILE.Succeeded())
	{
		ProjectileMesh->SetStaticMesh(SM_PROJECTILE.Object);
	}	

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = 3000.f;
	Movement->MaxSpeed = 3000.f;
	Movement->bRotationFollowsVelocity = true;
	// Code for test
	Movement->ProjectileGravityScale = 0.f;

	Damage = 10.f;
	LifeSpan = 3.f;
}

// Called when the game starts or when spawned
void AZProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);
}

// Called every frame
void AZProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AZProjectile::SetDamage(float NewDamage)
{
	if (NewDamage < 0.f)
	{
		return;
	}

	Damage = NewDamage;

}

void AZProjectile::FireInDirection(const FVector & Direction)
{
	Movement->Velocity = Direction * Movement->InitialSpeed;
}

void AZProjectile::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	if (OtherActor != this)
	{
		ZLOG(Warning, TEXT("Hit at %s, Damage : %f"), *OtherActor->GetName(), Damage);
		Destroy();
	}

}

