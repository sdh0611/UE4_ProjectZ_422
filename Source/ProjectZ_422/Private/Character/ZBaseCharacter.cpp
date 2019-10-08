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
#include "UnrealNetwork.h"

// Sets default values
AZBaseCharacter::AZBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ZCharacter"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetCollisionProfileName(TEXT("ZCharacterMesh"));

	bIsActive = true;
	bIsSprinting = false;
	bIsPooling = false;

	WalkSpeed = 500.f;
	SprintSpeed = 800.f;
	DisappearTime = 5.f;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

	bUseControllerRotationYaw = true;


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

	StatusComponent->AdjustCurrentHP(-FinalDamage);

	if (IsDead())
	{
		ZLOG(Warning, TEXT("Dead!!"));
		
		MulticastOnDead();
	}
	else
	{
		MulticastOnHit();
	}

	return FinalDamage;
}

void AZBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZBaseCharacter, bIsSprinting);
	DOREPLIFETIME(AZBaseCharacter, bIsActive);

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
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetCharacterMovement()->SetComponentTickEnabled(true);
}

void AZBaseCharacter::SetIsSprinting(bool bNewState)
{
	if (!HasAuthority())
	{
		ServerSetSprinting(bNewState);
		return;
	}

	bIsSprinting = bNewState;

	OnRep_IsSprinting();
}

void AZBaseCharacter::SetCharacterWalkSpeed(float NewSpeed)
{
	if (NewSpeed < 0.f)
	{
		return;
	}
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Set Walk Speed : %.2f"), NewSpeed));
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void AZBaseCharacter::SetCharacterCrouchWalkSpeed(float NewSpeed)
{
	if (NewSpeed < 0.f)
	{
		return;
	}

	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Set Crouch Speed : %.2f"), NewSpeed));
	GetCharacterMovement()->MaxWalkSpeedCrouched = NewSpeed;
}

void AZBaseCharacter::SetActive(bool bActive)
{
	//SetActorHiddenInGame(!bActive);
	GetMesh()->SetVisibility(bActive);
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

float AZBaseCharacter::GetWalkSpeed() const
{
	return WalkSpeed;
}

float AZBaseCharacter::GetSprintSpeed() const
{
	return SprintSpeed;
}

float AZBaseCharacter::GetCurrentSpeed() const
{
	float CharacterCurrentSpeed = 0.f;

	if (bIsCrouched)
	{
		CharacterCurrentSpeed = GetCharacterMovement()->MaxWalkSpeedCrouched;
	}
	else
	{
		CharacterCurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
	}

	return CharacterCurrentSpeed;
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

void AZBaseCharacter::MulticastPlayMontage_Implementation(const FString & MontageName)
{
	auto CharacterAnim = GetAnimInstance();
	if (!::IsValid(CharacterAnim))
	{
		ZLOG(Error, TEXT("Invalid AnimInstance."));
		return;
	}

	CharacterAnim->PlayCharacterMontage(MontageName);
}

void AZBaseCharacter::CheckCharacterRotation(float DeltaTime)
{
}

void AZBaseCharacter::OnDead()
{
	//GetAnimInstance()->SetIsDead(true);

	SetActorEnableCollision(false);
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->SetComponentTickEnabled(false);
	//SetActorTickEnabled(false);
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//
	//GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	//GetMesh()->SetSimulatePhysics(true);
	//
	if (DeadSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), DeadSound, GetActorLocation(), GetActorRotation());
	}

	//FTimerDelegate InactiveDelegate;
	//InactiveDelegate.BindLambda([this]() {
	//	if (bIsPooling)
	//	{
	//		SetActive(false);
	//		GetAnimInstance()->SetIsDead(false);
	//	}
	//	else
	//	{
	//		Destroy();
	//	}
	//});

	GetWorld()->GetTimerManager().SetTimer(InactiveTimer, this, &AZBaseCharacter::OnRemoved, DisappearTime, false);

}

void AZBaseCharacter::OnRemoved()
{
	if (bIsPooling)
	{
		//GetAnimInstance()->SetIsDead(false);
		SetActive(false);
	}
	else
	{
		Destroy();
	}
}

bool AZBaseCharacter::ServerSetSprinting_Validate(bool bNewState)
{
	return true;
}

void AZBaseCharacter::ServerSetSprinting_Implementation(bool bNewState)
{
	SetIsSprinting(bNewState);

	OnRep_IsSprinting();
}

void AZBaseCharacter::MulticastOnHit_Implementation()
{
	auto Anim = GetAnimInstance();
	if (::IsValid(Anim))
	{
		Anim->bIsDamaged = true;
	}

	if (HitSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound,
			GetActorLocation(), GetActorRotation());
	}

}

void AZBaseCharacter::MulticastOnDead_Implementation()
{
	OnDead();
}

void AZBaseCharacter::OnRep_IsSprinting()
{
	if (bIsSprinting)
	{
		SetCharacterWalkSpeed(SprintSpeed);
	}
	else
	{
		SetCharacterWalkSpeed(WalkSpeed);
	}

	auto CharacterAnim = GetAnimInstance();
	if (!::IsValid(CharacterAnim))
	{
		ZLOG(Error, TEXT("Invalid anim instance.."));
		return;
	}
	CharacterAnim->SetIsSprinting(bIsSprinting);
}

void AZBaseCharacter::OnRep_IsActive()
{
	SetActive(bIsActive);
}
