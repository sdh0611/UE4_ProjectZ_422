// Fill out your copyright notice in the Description page of Project Settings.


#include "ZBaseCharacter.h"
#include "ZCharacterStatusComponent.h"
#include "ZCharacterAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "TimerManager.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"

// Sets default values
AZBaseCharacter::AZBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ZCharacter"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	// Create character status component
	StatusComponent = CreateDefaultSubobject<UZCharacterStatusComponent>(TEXT("StatusComponent"));

	bIsActive = true;
	bIsSprinting = false;
	bIsPooling = false;

	WalkSpeed = 500.f;
	SprintSpeed = 800.f;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

	bUseControllerRotationYaw = true;

	CurrentPlaySound = nullptr;

	Tags.Add(TEXT("Character"));
}

// Called when the game starts or when spawned
void AZBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	auto ZAnimInstance = Cast<UZCharacterAnimInstance>(GetMesh()->GetAnimInstance());
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
		FPointDamageEvent* PointDamage = (FPointDamageEvent*)(&DamageEvent);
		UPhysicalMaterial* PhysicalMaterial = PointDamage->HitInfo.PhysMaterial.Get();
		if (PhysicalMaterial->SurfaceType == SURFACE_HEAD)
		{
			ZLOG(Error, TEXT("Headshot."));
			FinalDamage *= 5.f;
		}

		if (HitEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, 
				PointDamage->HitInfo.ImpactPoint, PointDamage->HitInfo.ImpactNormal.Rotation());
		}

		if (HitSound)
		{
			if (nullptr == CurrentPlaySound)
			{
				CurrentPlaySound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound,
					PointDamage->HitInfo.ImpactPoint, PointDamage->HitInfo.ImpactNormal.Rotation());
			}
			else
			{
				if (!CurrentPlaySound->IsPlaying())
				{
					CurrentPlaySound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound,
						PointDamage->HitInfo.ImpactPoint, PointDamage->HitInfo.ImpactNormal.Rotation());
				}
			}
		}
	}

	StatusComponent->AdjustCurrentHP(-FinalDamage);
	if (IsDead())
	{
		ZLOG(Warning, TEXT("Dead!!"));
		OnDead();
	}

	return FinalDamage;
}

FHitResult AZBaseCharacter::GetTraceHit(const FVector & TraceStart, const FVector & TraceEnd)
{
	FHitResult Hit;

	if (!IsDead())
	{
		FCollisionQueryParams TraceParams;
		TraceParams.bTraceComplex = false;
		TraceParams.bReturnPhysicalMaterial = false;
		TraceParams.AddIgnoredActor(this);

		GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);
	}

	return Hit;
}

void AZBaseCharacter::Revive()
{
	//GetAnimInstance()->SetIsDead(false);
	StatusComponent->SetCurrentHP(StatusComponent->GetMaxHP());
	//FVector Location = GetActorLocation();
	//Location.Z -= 88.f;
	//GetMesh()->SetWorldLocation(Location);
	SetActive(true);
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

void AZBaseCharacter::SetActive(bool bActive)
{
	SetActorHiddenInGame(!bActive);
	SetActorEnableCollision(bActive);
	SetActorTickEnabled(bActive);

	//if (bActive)
	//{
	//	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//	SetActorHiddenInGame(false);
	//	SetActorEnableCollision(true);
	//	SetActorTickEnabled(false);
	//}
	//else
	//{
	//	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//	SetActorHiddenInGame(true);
	//	SetActorEnableCollision(false);
	//}

	bIsActive = bActive;
}

bool AZBaseCharacter::IsDead() const
{
	return StatusComponent->IsDead();
}

bool AZBaseCharacter::IsActive() const
{
	return bIsActive;
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

UZCharacterAnimInstance * AZBaseCharacter::GetAnimInstance() const
{
	if (!AnimInstance->IsValidLowLevel())
	{
		return nullptr;
	}

	return AnimInstance;
}

void AZBaseCharacter::CheckCharacterRotation(float DeltaTime)
{
}

void AZBaseCharacter::OnDead()
{
	GetAnimInstance()->SetIsDead(true);

	SetActorEnableCollision(false);
	//SetActorTickEnabled(false);
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//
	//GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	//GetMesh()->SetSimulatePhysics(true);
	//
	FTimerDelegate InactiveDelegate;
	InactiveDelegate.BindLambda([this]() {
		if (bIsPooling)
		{
			SetActive(false);
			GetAnimInstance()->SetIsDead(false);
		}
		else
		{
			Destroy();
		}
	});

	GetWorld()->GetTimerManager().SetTimer(InactiveTimer, InactiveDelegate, 5.f, false);

}

