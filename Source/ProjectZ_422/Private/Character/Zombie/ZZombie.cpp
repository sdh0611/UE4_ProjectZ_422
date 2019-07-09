// Fill out your copyright notice in the Description page of Project Settings.


#include "ZZombie.h"
#include "ZZombieAIController.h"
#include "ZCharacter.h"
<<<<<<< HEAD
#include "ZCharacterAnimInstance.h"
=======
>>>>>>> origin/branch0621
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

	static ConstructorHelpers::FClassFinder<UZCharacterAnimInstance>
		ANIM_ZOMBIE(TEXT("AnimBlueprint'/Game/Animation/Blueprint/ZZombie_AnimBlueprint.ZZombie_AnimBlueprint_C'"));
	if (ANIM_ZOMBIE.Succeeded())
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetMesh()->SetAnimInstanceClass(ANIM_ZOMBIE.Class);
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
<<<<<<< HEAD

	AttackDamage = 10.f;

}

void AZZombie::BeginPlay()
{
	Super::BeginPlay();

	auto NewAnimInstance = Cast<UZCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	check(nullptr != NewAnimInstance);
	AnimInstance = NewAnimInstance;

	if (Sense)
	{
		Sense->OnSeePawn.AddDynamic(this, &AZZombie::OnSeePlayer);
	}


}

void AZZombie::OnSeePlayer(APawn * Pawn)
{
	ZLOG_S(Warning);
	auto ZombieController = Cast<AZZombieAIController>(GetController());

=======

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

>>>>>>> origin/branch0621
	auto Player = Cast<AZCharacter>(Pawn);
	if (ZombieController && Player)
	{
		if (!Player->IsDead())
		{
			/* Target ¼³Á¤ */
			ZombieController->SetTargetPawn(Player);
		}
	}

}
<<<<<<< HEAD

UZCharacterAnimInstance * const AZZombie::GetZombieAnimInstance() const
{
	return Cast<UZCharacterAnimInstance>(AnimInstance);
}
=======
>>>>>>> origin/branch0621
