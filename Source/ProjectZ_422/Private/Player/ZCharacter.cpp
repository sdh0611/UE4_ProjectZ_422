// Fill out your copyright notice in the Description page of Project Settings.


#include "ZCharacter.h"
#include "ZInteractional.h"
#include "ZCharacterItemStatusComponent.h"
#include "ZPlayerController.h"
#include "ZHUD.h"
#include "ZUserHUD.h"
#include "ZCurrentWeaponInfoWidget.h"
#include "ZWeapon.h"
#include "ZProjectile.h"
#include "ZCharacterAnimInstance.h"
#include "ZPlayerController.h"
#include "ZCharacterStatusComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

const FName AZCharacter::MainWeaponSocketName = FName(TEXT("weapon_r"));
const FName AZCharacter::SecondaryWeaponSocketName = FName(TEXT("weapon_secondary"));

// Sets default values
AZCharacter::AZCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Init chracter mesh.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_CHARACTER(TEXT("SkeletalMesh'/Game/Animation/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (SK_CHARACTER.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CHARACTER.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88.f), FRotator(0, -90.f, 0));
	}
	else
	{
		ZLOG(Error, TEXT("Cannot find Character Mesh."));
	}

	// Init character anim instance
	static ConstructorHelpers::FClassFinder<UZCharacterAnimInstance>
		ANIM_CHARACTER(TEXT("AnimBlueprint'/Game/Animation/Blueprint/ZCharacter_AnimBlueprint.ZCharacter_AnimBlueprint_C'"));
	if (ANIM_CHARACTER.Succeeded())
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetMesh()->SetAnimInstanceClass(ANIM_CHARACTER.Class);
	}

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ZCharacter"));

	// Create main camera spring arm.
	MainCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MainCameraSpringArm"));
	MainCameraSpringArm->TargetArmLength = 200.f;
	MainCameraSpringArm->SetRelativeLocation(FVector(0.f, 0.f, 64.f));
	MainCameraSpringArm->bUsePawnControlRotation = true;
	MainCameraSpringArm->bInheritPitch = true;
	MainCameraSpringArm->bInheritYaw = true;
	MainCameraSpringArm->bInheritRoll = true;
	MainCameraSpringArm->SetupAttachment(GetCapsuleComponent());
	// Create main camera & attach to main camera spring arm.
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	MainCamera->SetRelativeLocation(FVector(0.f, 50.f, 0.f));
	MainCamera->SetupAttachment(MainCameraSpringArm);

	// Create item status component.
	ItemStatusComponent = CreateDefaultSubobject<UZCharacterItemStatusComponent>(TEXT("ItemStatusComponent"));
	
	// Create character status component
	StatusComponent = CreateDefaultSubobject<UZCharacterStatusComponent>(TEXT("StatusComponent"));
	
	// Set Character Properties
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch= true;
	GetCharacterMovement()->JumpZVelocity = 700.f;
	
	bIsSprinting = false;
	bIsAiming = false;
	WalkSpeed = 500.f;
	WalkSpeedCrouched = 250.f;
	SprintSpeed = 800.f;
	AimingWalkSpeed = 200.f;
	AimingWalkSpeedCrouched = 150.f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = WalkSpeedCrouched;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

	bUseControllerRotationYaw = true;

	CurrentWeapon = nullptr;
}

// Called when the game starts or when spawned
void AZCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = Cast<AZPlayerController>(GetController());
	auto ZAnimInstance = Cast<UZCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	check(nullptr != ZAnimInstance);
	AnimInstance = ZAnimInstance;
	
}

// Called every frame
void AZCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto NewInteractionActor = GetInteractionalInView();
	if (InteractionActor != NewInteractionActor)
	{
		if (InteractionActor)
		{
			InteractionActor->OnFocusEnd();
		}
		
		if (NewInteractionActor)
		{
			NewInteractionActor->OnFocus();
		}
	
		InteractionActor = NewInteractionActor;

	}

	

	//CheckCharacterRotation(DeltaTime);
}

// Called to bind functionality to input
void AZCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind axis
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AZCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AZCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AZCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AZCharacter::LookRight);

	// Bind action
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AZCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("ToggleCrouch"), IE_Pressed, this, &AZCharacter::ToggleCrouch);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AZCharacter::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AZCharacter::SprintRelease);
	PlayerInputComponent->BindAction(TEXT("Interaction"), IE_Pressed, this, &AZCharacter::Interaction);
	PlayerInputComponent->BindAction(TEXT("ToggleInventory"), IE_Pressed, this, &AZCharacter::ToggleInventory);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AZCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Released, this, &AZCharacter::AttackEnd);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Pressed, this, &AZCharacter::Aim);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Released, this, &AZCharacter::AimRelease);
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &AZCharacter::Reload);
	PlayerInputComponent->BindAction(TEXT("DropWeapon"), IE_Pressed, this, &AZCharacter::DropWeapon);
	PlayerInputComponent->BindAction(TEXT("Slot1"), IE_Pressed, this, &AZCharacter::Slot1);
	PlayerInputComponent->BindAction(TEXT("Slot2"), IE_Pressed, this, &AZCharacter::Slot2);

	// For debug
	PlayerInputComponent->BindAction(TEXT("AddMoney"), IE_Pressed, this, &AZCharacter::AddMoney);
	PlayerInputComponent->BindAction(TEXT("DamageSelf"), IE_Pressed, this, &AZCharacter::DamageSelf);



}

float AZCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
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

FHitResult AZCharacter::GetTraceHit(const FVector & TraceStart, const FVector & TraceEnd)
{
	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = false;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.AddIgnoredActor(this);

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

	return Hit;
}

FHitResult AZCharacter::GetTraceHitFromActorCameraView(float Distance)
{
	FVector CamLoc;
	FRotator CamRot;
	PlayerController->GetPlayerViewPoint(CamLoc, CamRot);

	const FVector Direction = CamRot.Vector();
	const FVector TraceStart = GetActorLocation();
	const FVector TraceEnd = TraceStart + (Direction * Distance);

	return GetTraceHit(TraceStart, TraceEnd);
}

void AZCharacter::SetIsSprinting(bool NewState)
{
	bIsSprinting = NewState;
}

void AZCharacter::SetIsAiming(bool NewState)
{
	bIsAiming = NewState;
}

void AZCharacter::SetIsSwitchingWeapon(bool NewState)
{
	bIsSwitchingWeapon = NewState;
}

void AZCharacter::SetCurrentWeapon(AZWeapon * NewWeapon)
{
	ZLOG_S(Warning);
	if (CurrentWeapon)
	{
		CurrentWeapon->SetIsEquipped(false);
	}

	CurrentWeapon = NewWeapon;
	if (NewWeapon)
	{
		NewWeapon->SetIsEquipped(true);
		AnimInstance->SetIsEquipWeapon(true);
		NewWeapon->OnWeaponFired.AddUObject(AnimInstance, &UZCharacterAnimInstance::PlayFireMontage);
		//bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		// CurrentWeaponInfo widget에 바인딩
		PlayerController->GetZHUD()->GetUserHUD()->GetCurrentWeaponInfoWidget()->BindWeapon(CurrentWeapon);
	}
	else
	{
		AnimInstance->SetIsEquipWeapon(false);
		//bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		PlayerController->GetZHUD()->GetUserHUD()->GetCurrentWeaponInfoWidget()->ClearWidget();
	}
}

void AZCharacter::SetCurrentSpeed(float NewSpeed)
{
	if (GetCharacterMovement()->IsCrouching())
	{
		GetCharacterMovement()->MaxWalkSpeedCrouched = NewSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed= NewSpeed;
	}

}



bool AZCharacter::IsSprinting()
{
	return bIsSprinting;
}

bool AZCharacter::IsEquipWeapon()
{
	return (CurrentWeapon != nullptr);
}

bool AZCharacter::IsAiming()
{
	return bIsAiming;
}

bool AZCharacter::IsSwitchingWeapon()
{
	return bIsSwitchingWeapon;
}

float AZCharacter::GetCurrentSpeed() const
{
	float CurrentSpeed = 0.f;

	if (GetCharacterMovement()->IsCrouching())
	{
		CurrentSpeed = GetCharacterMovement()->MaxWalkSpeedCrouched;
	}
	else
	{
		CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
	}

	return CurrentSpeed;
}

AZInteractional * AZCharacter::GetInteractionalInView()
{
	FVector CamLoc;
	FRotator CamRot;
	GetController()->GetPlayerViewPoint(CamLoc, CamRot);
	
	const FVector Direction = CamRot.Vector();
	const FVector TraceStart = CamLoc;
	const FVector TraceEnd = TraceStart + (Direction * 600.f);

	FCollisionQueryParams TraceParams(TEXT("TracePickupActor"), true, this);
	TraceParams.bTraceComplex = false;
	TraceParams.bReturnPhysicalMaterial = false;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, TraceParams);

	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.1f);

	return Cast<AZInteractional>(HitResult.GetActor());
}

UZCharacterItemStatusComponent * const AZCharacter::GetItemStatusComponent() const
{
	return ItemStatusComponent;
}

UZCharacterStatusComponent * const AZCharacter::GetStatusComponent() const
{
	return StatusComponent;
}

AZWeapon * const AZCharacter::GetCurrentWeapon()
{
	return CurrentWeapon;
}

void AZCharacter::CheckCharacterRotation(float DeltaTime)
{
	const FRotator ActorRotation = GetActorRotation();
	const FRotator ControlRotation = GetControlRotation();
	if (FMath::Abs<float>(ActorRotation.Yaw - ControlRotation.Yaw) > 90.f)
	{
		//Rotate = ActorRotation.Yaw - ControlRotation.Yaw;
		FRotator Rot = FMath::RInterpTo(ActorRotation, FRotator(0.f, ControlRotation.Yaw, 0.f), DeltaTime, 15.f);
		AddActorLocalRotation(Rot);
	}
	

}

void AZCharacter::MoveForward(float NewAxisValue)
{
	const FRotator Rotation = GetController()->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	if (NewAxisValue < 0)
	{
		SprintRelease();
	}

	AddMovementInput(Direction, NewAxisValue);

}

void AZCharacter::MoveRight(float NewAxisValue)
{
	const FRotator Rotation = GetController()->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Direction, NewAxisValue);

}

void AZCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void AZCharacter::LookRight(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

void AZCharacter::ToggleCrouch()
{
	// Character가 공중에 떠있는지 체크
	if (GetCharacterMovement()->IsFalling())
	{
		//공중에 떠있다면 그대로 종료.
		return;
	}

	// 현재 Character가 앉은 상태인지 체크.
	if (GetCharacterMovement()->IsCrouching())
	{
		// 앉은 상태인 경우
		ACharacter::UnCrouch();
	}
	else
	{
		// 앉은 상태가 아닌 경우
		ACharacter::Crouch();
	}

}

void AZCharacter::Sprint()
{
	if (IsAiming())
	{
		return;
	}

	if (IsEquipWeapon())
	{
		if (CurrentWeapon->IsReloading())
		{
			return;
		}
	}
	
	auto MyCharacterMovement = GetCharacterMovement();
	if (MyCharacterMovement)
	{
		// Character가 공중에 있거나 앉은 상태라면 그대로 종료.
		if (MyCharacterMovement->IsFalling() || MyCharacterMovement->IsCrouching())
		{
			return;
		}

		if (!IsSprinting())
		{
			// SprintSpeed로 값을 변경하고 Sprint 상태 변경
			MyCharacterMovement->MaxWalkSpeed = SprintSpeed;
			SetIsSprinting(true);
			AnimInstance->SetIsSprinting(true);
			//bUseControllerRotationYaw = false
		}

	}

}

void AZCharacter::SprintRelease()
{
	auto MyCharacterMovement = GetCharacterMovement();
	if (MyCharacterMovement)
	{
		// 뛰고있는 상태라면
		if (IsSprinting())
		{
			// WalkSpeed로 값을 변경하고 Sprint 상태 변경
			MyCharacterMovement->MaxWalkSpeed = WalkSpeed;
			SetIsSprinting(false);
			AnimInstance->SetIsSprinting(false);
			//if (IsEquipWeapon())
			//{
			//	bUseControllerRotationYaw = true
			//}
		}

	}
}

void AZCharacter::Jump()
{
	if (IsEquipWeapon())
	{
		if (CurrentWeapon->IsReloading())
		{
			return;
		}
	}

	if (IsAiming())
	{
		AimRelease();
	}

	// Character가 현재 앉은 상태인지 체크.
	if (GetCharacterMovement()->IsCrouching())
	{
		// 앉은 상태라면 UnCrouch() 메소드 실행
		ACharacter::UnCrouch();
	}
	else
	{
		Super::Jump();
	}
}

void AZCharacter::Interaction()
{
	if (IsEquipWeapon())
	{
		if (CurrentWeapon->IsReloading())
		{
			return;
		}

		if (IsSwitchingWeapon())
		{
			return;
		}
	}

	if (InteractionActor)
	{
		InteractionActor->OnInteraction(this);
	}
}

void AZCharacter::ToggleInventory()
{
	auto HUD = PlayerController->GetZHUD();
	if (HUD)
	{
		auto UserHUDWidget = HUD->GetUserHUD();
		if (UserHUDWidget)
		{
			if (UserHUDWidget->IsInventoryOnScreen())
			{
				// Inventory가 화면에 있는 경우.
				UserHUDWidget->RemoveInventoryWidget();
			}
			else
			{
				// Inventory가 화면에 없는 경우.
				UserHUDWidget->DrawInventoryWidget();
			}
		}
	}

}

void AZCharacter::Attack()
{
	if (!IsEquipWeapon())
	{
		return;
	}

	if (CurrentWeapon->IsReloading())
	{
		return;
	}

	if (IsSwitchingWeapon())
	{
		return;
	}

	CurrentWeapon->SetWantsToFire(true);
}

void AZCharacter::AttackEnd()
{
	if (!IsEquipWeapon())
	{
		return;
	}

	CurrentWeapon->SetWantsToFire(false);
}

void AZCharacter::Aim()
{
	if (!IsEquipWeapon())
	{
		return;
	}
	else
	{
		if (CurrentWeapon->IsReloading())
		{
			return;
		}
	}

	if (IsSwitchingWeapon())
	{
		return;
	}

	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (IsSprinting())
	{
		SprintRelease();
	}

	if (GetCharacterMovement()->IsCrouching())
	{
		GetCharacterMovement()->MaxWalkSpeedCrouched = AimingWalkSpeedCrouched;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = AimingWalkSpeed;
	}

	SetIsAiming(true);
	AnimInstance->SetIsAiming(true);
}

void AZCharacter::AimRelease()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		GetCharacterMovement()->MaxWalkSpeedCrouched = WalkSpeedCrouched;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
	SetIsAiming(false);
	AnimInstance->SetIsAiming(false);
}

void AZCharacter::Reload()
{
	if (!IsEquipWeapon())
	{
		return;
	}
	else
	{
		if (CurrentWeapon->IsReloading())
		{
			return;
		}
		else
		{
			if (!CurrentWeapon->IsCanReload())
			{
				return;
			}
		}
	}

	if (IsSwitchingWeapon())
	{
		return;
	}

	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (IsSprinting())
	{
		SprintRelease();
	}

	if (IsAiming())
	{
		AimRelease();
	}


	CurrentWeapon->SetIsReloading(true);
	AnimInstance->PlayMontage(TEXT("ReloadRifle"));

}

void AZCharacter::DropWeapon()
{
	if (!IsEquipWeapon())
	{
		return;
	}

	if (CurrentWeapon->IsReloading())
	{
		return;
	}

	ItemStatusComponent->DropWeapon(CurrentWeapon->GetWeaponInventoryIndex());

}

void AZCharacter::SwitchWeapon(int32 NewWeaponIndex)
{
	// 해당 Slot에 Weapon이 없는 경우(null인 경우)
	if (nullptr == ItemStatusComponent->GetWeaponFromWeaponInventory(NewWeaponIndex))
	{
		return;
	}

	if (IsEquipWeapon())
	{
		if (CurrentWeapon->GetWeaponInventoryIndex() == NewWeaponIndex)
		{
			return;
		}

		if (CurrentWeapon->IsReloading())
		{
			return;
		}

		// 기존 Weapon은 Secondary socket으로 옮김.
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SecondaryWeaponSocketName);
	}

	SetCurrentWeapon(ItemStatusComponent->GetWeaponFromWeaponInventory(NewWeaponIndex));
	// 새 Weapon은 Main socket으로 옮김.
	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, MainWeaponSocketName);
	SetIsSwitchingWeapon(true);
	AnimInstance->PlayMontage(TEXT("EquipRifle"));

}

void AZCharacter::Slot1()
{
	SwitchWeapon(0);
}

void AZCharacter::Slot2()
{
	SwitchWeapon(1);
}

void AZCharacter::AddMoney()
{
	ZLOG_S(Warning);
	ItemStatusComponent->AdjustMoney(500);

}

void AZCharacter::DamageSelf()
{
	StatusComponent->AdjustCurrentHP(-10.f);
}
