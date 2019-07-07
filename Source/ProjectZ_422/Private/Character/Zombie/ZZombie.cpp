// Fill out your copyright notice in the Description page of Project Settings.


#include "ZZombie.h"
#include "ZZombieAIController.h"
#include "ZCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ConstructorHelpers.h"


AZZombie::AZZombie()
{
	AIControllerClass = AZZombieAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_ZOMBIE(TEXT("SkeletalMesh'/Game/Animation/Zombie/derrick.derrick'"));
	if (SK_ZOMBIE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_ZOMBIE.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88.f), FRotator(0, -90.f, 0));
	}

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("TraceSphere"));
	Sphere->SetRelativeLocation(GetActorLocation());
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetupAttachment(GetCapsuleComponent());
	 
	Sense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSense"));
	Sense->SetPeripheralVisionAngle(60.f);
	Sense->SightRadius = 2000;
	Sense->HearingThreshold = 600;
	Sense->LOSHearingThreshold = 1200;
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

}

void AZZombie::BeginPlay()
{
	Super::BeginPlay();

	if (Sense)
	{
		Sense->OnSeePawn.AddDynamic(this, &AZZombie::OnSeePlayer);
	}

}

void AZZombie::OnSeePlayer(APawn * Pawn)
{
	ZLOG_S(Warning);
	auto ZombieController = Cast<AZZombieAIController>(GetController());

	auto Player = Cast<AZCharacter>(Pawn);
	if (ZombieController && Player)
	{
		if (!Player->IsDead())
		{
			/* Target ���� */
			ZombieController->SetTargetPawn(Player);
		}
	}

}
