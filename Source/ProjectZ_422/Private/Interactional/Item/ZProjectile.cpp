// Fill out your copyright notice in the Description page of Project Settings.


#include "ZProjectile.h"
#include "ZCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ConstructorHelpers.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values
AZProjectile::AZProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	//Sphere->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	//Sphere->InitSphereRadius(4.f);
	//Sphere->OnComponentHit.AddDynamic(this, &AZProjectile::OnHit);
	//RootComponent = Sphere;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	//ProjectileMesh->SetupAttachment(Sphere);
	RootComponent = ProjectileMesh;


	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->MaxSpeed = 3000.f;
	Movement->bRotationFollowsVelocity = true;

	Damage = 10.f;
	LifeSpan = 3.f;

}

// Called when the game starts or when spawned
void AZProjectile::BeginPlay()
{
	Super::BeginPlay();

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

}

void AZProjectile::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	//if (OtherActor != this)
	//{
		ZLOG_S(Warning);
	//	Destroy();
	//}

}
