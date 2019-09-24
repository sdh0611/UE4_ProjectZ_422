// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/ZProp.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AZProp::AZProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BodyCollision"));
	RootComponent = BodyCollision;
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(RootComponent);

	TeamId = FGenericTeamId(1);
	
	Tags.Add(TEXT("Prop"));
}

// Called when the game starts or when spawned
void AZProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AZProp::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	CurrentHP -= FinalDamage;
	if (CurrentHP <= 0.f)
	{
		SetActorEnableCollision(false);
		SetActorHiddenInGame(true);
		SetLifeSpan(2.f);
	}

	return FinalDamage;
}

bool AZProp::IsTargetable() const
{
	return true;
}

bool AZProp::IsDead() const
{
	return CurrentHP <= 0.f;
}

FGenericTeamId AZProp::GetGenericTeamId() const
{
	return TeamId;
}

