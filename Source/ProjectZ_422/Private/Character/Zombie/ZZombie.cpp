// Fill out your copyright notice in the Description page of Project Settings.


#include "ZZombie.h"
#include "ZZombieAIController.h"
#include "ZCharacter.h"
#include "ZZombieAnimInstance.h"
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

	Sense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSense"));
	Sense->SetPeripheralVisionAngle(60.f);
	Sense->SightRadius = 2000;
	Sense->HearingThreshold = 600;
	Sense->LOSHearingThreshold = 1200;

	bUseControllerRotationYaw = false;
	//GetCharacterMovement()->bUseControllerDesiredRotation = false;
	//GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

	RightHandSocket = TEXT("attack_r");
	LeftHandSocket = TEXT("attack_l");

	//AttackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollision"));
	//AttackCollision->SetSphereRadius(25.f);
	//AttackCollision->SetRelativeLocation(FVector(-10.f, 0.f, 0.f));
	//AttackCollision->SetCollisionProfileName(TEXT("EnemyAttack"));
	//AttackCollision->SetupAttachment(GetMesh(), RightHandSocket);

	AttackDamage = 10.f;
	bIsAttacking = false;

}

void AZZombie::BeginPlay()
{
	Super::BeginPlay();

	if (Sense)
	{
		Sense->OnSeePawn.AddDynamic(this, &AZZombie::OnSeePlayer);
	}

	auto ZombieAnim = GetZombieAnimInstance();
	check(nullptr != ZombieAnim);
	ZombieAnim->OnAttackCheck.BindUObject(this, &AZZombie::AttackCheck);
}

void AZZombie::OnSeePlayer(APawn * Pawn)
{
	//ZLOG_S(Warning);
	auto ZombieController = Cast<AZZombieAIController>(GetController());

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

void AZZombie::Attack()
{
	//ZLOG_S(Warning);
	auto ZombieAnim = GetZombieAnimInstance();
	check(nullptr != ZombieAnim);
	if (ZombieAnim->IsMontagePlaying(TEXT("Attack")))
	{
		return;
	}

	ZombieAnim->PlayMontage(TEXT("Attack"));
}

void AZZombie::AttackEnd()
{
	OnAttackEnd.Execute();
}

void AZZombie::SetActive(bool bActive)
{
	if (bActive)
	{
		GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
		GetMesh()->SetSimulatePhysics(false);
		//GetMesh()->SetRelativeRotation(GetCapsuleComponent()->GetRelativeTransform().Rotator());
		//GetMesh()->SetRelativeRotation(FQuat::Identity);
	}

	Super::SetActive(bActive);
}

UZZombieAnimInstance * const AZZombie::GetZombieAnimInstance() const
{
	return Cast<UZZombieAnimInstance>(GetAnimInstance());
}

void AZZombie::AttackCheck()
{
	TArray<FHitResult> Hits;
	FCollisionQueryParams CollisionParams(TEXT("EnemyAttackParam"), false, this);
	CollisionParams.bReturnPhysicalMaterial = false;
	CollisionParams.bTraceComplex = false;

	bool bResult = GetWorld()->SweepMultiByProfile(Hits, GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 200.f,
		FQuat::Identity, TEXT("EnemyAttack"), FCollisionShape::MakeSphere(50.f), CollisionParams);

	ZLOG(Error, TEXT("Hits : %d"), Hits.Num());
	if (!bResult)
	{
		return;
	}

	for (const auto& Hit : Hits)
	{
		ZLOG(Error, TEXT("Name : %s"), *Hit.Actor->GetName());
		auto Character = Cast<AZCharacter>(Hit.Actor);
		if (nullptr == Character)
		{
			continue;
		}
		Character->TakeDamage(AttackDamage, FDamageEvent(), Controller, this);

	}


}

