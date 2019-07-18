// Fill out your copyright notice in the Description page of Project Settings.


#include "ZBaseCharacter.h"
#include "ZCharacterStatusComponent.h"
#include "ZCharacterAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"


// Sets default values
AZBaseCharacter::AZBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create character status component
	StatusComponent = CreateDefaultSubobject<UZCharacterStatusComponent>(TEXT("StatusComponent"));

	bIsSprinting = false;
	WalkSpeed = 500.f;
	SprintSpeed = 800.f;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

	bUseControllerRotationYaw = true;

}

// Called when the game starts or when spawned
void AZBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	auto ZAnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
	check(nullptr != ZAnimInstance);
	AnimInstance = ZAnimInstance;
}

// Called every frame
void AZBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AZBaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	ZLOG_S(Warning);
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent PointDamage = static_cast<const FPointDamageEvent&>(DamageEvent);
		ZLOG(Warning, TEXT("Hit at %s"), *PointDamage.HitInfo.BoneName.ToString());
	}

	StatusComponent->AdjustCurrentHP(-FinalDamage);
	if (StatusComponent->IsDead())
	{
		ZLOG(Warning, TEXT("Dead!!"));
	}

	return FinalDamage;
}

FHitResult AZBaseCharacter::GetTraceHit(const FVector & TraceStart, const FVector & TraceEnd)
{
	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = false;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.AddIgnoredActor(this);

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

	return Hit;
}

void AZBaseCharacter::SetIsSprinting(bool NewState)
{
	bIsSprinting = NewState;
}

void AZBaseCharacter::SetCurrentSpeed(float NewSpeed)
{
	if (NewSpeed < 0.f)
	{
		return;
	}

	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

bool AZBaseCharacter::IsDead() const
{
	return StatusComponent->IsDead();
}

bool AZBaseCharacter::IsSprinting() const
{
	return bIsSprinting;
}

float AZBaseCharacter::GetCurrentSpeed() const
{
	return GetCharacterMovement()->MaxWalkSpeed;
}

UZCharacterStatusComponent * const AZBaseCharacter::GetStatusComponent() const
{
	return StatusComponent;
}

UAnimInstance * AZBaseCharacter::GetAnimInstance() const
{
	return AnimInstance;
}

void AZBaseCharacter::CheckCharacterRotation(float DeltaTime)
{
}

