// Fill out your copyright notice in the Description page of Project Settings.


#include "ZCharacter.h"
#include "ZInteractional.h"
#include "ZCharacterItemStatusComponent.h"
#include "ZPlayerController.h"
#include "ZHUD.h"
#include "ZUserHUD.h"
#include "ZCurrentWeaponInfoWidget.h"
#include "ZWeapon.h"
#include "ZGun.h"
#include "ZGrenade.h"
#include "ZProjectile.h"
#include "ZCharacterAnimInstance.h"
#include "ZPlayerAnimInstance.h"
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
const FName AZCharacter::ThirdWeaponSocketName = FName(TEXT("weapon_third"));
const FName AZCharacter::KnifeSocketName = FName(TEXT("weapon_knife"));
const FName AZCharacter::GrenadeWeaponSocketName = FName(TEXT("weapon_grenade"));

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


	// Create main camera spring arm.
	MainCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MainCameraSpringArm"));
	MainCameraSpringArm->TargetArmLength = 200.f;
	MainCameraSpringArm->SetRelativeLocation(FVector(0.f, 0.f, 64.f));
	MainCameraSpringArm->bUsePawnControlRotation = true;
	MainCameraSpringArm->bInheritPitch = true;
	MainCameraSpringArm->bInheritYaw = true;
	MainCameraSpringArm->bInheritRoll = true;
	//MainCameraSpringArm->bDoCollisionTest = false;
	MainCameraSpringArm->SetupAttachment(GetCapsuleComponent());
	// Create main camera & attach to main camera spring arm.
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	MainCamera->SetRelativeLocation(FVector(0.f, 50.f, 0.f));
	MainCamera->SetupAttachment(MainCameraSpringArm);

	// Create item status component.
	ItemStatusComponent = CreateDefaultSubobject<UZCharacterItemStatusComponent>(TEXT("ItemStatusComponent"));
	
	//// Create character status component
	//StatusComponent = CreateDefaultSubobject<UZCharacterStatusComponent>(TEXT("StatusComponent"));
	
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
	PlayerInputComponent->BindAction(TEXT("Slot4"), IE_Pressed, this, &AZCharacter::Slot4);

	// For debug
	PlayerInputComponent->BindAction(TEXT("AddMoney"), IE_Pressed, this, &AZCharacter::AddMoney);
	PlayerInputComponent->BindAction(TEXT("DamageSelf"), IE_Pressed, this, &AZCharacter::DamageSelf);
	PlayerInputComponent->BindAction(TEXT("Ragdoll"), IE_Pressed, this, &AZCharacter::Ragdoll);



}

//float AZCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
//{
//	ZLOG_S(Warning);
//	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
//	
//	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
//	{
//		FPointDamageEvent PointDamage = static_cast<const FPointDamageEvent&>(DamageEvent);
//		ZLOG(Warning, TEXT("Hit at %s"), *PointDamage.HitInfo.BoneName.ToString());
//	}
//
//	StatusComponent->AdjustCurrentHP(-FinalDamage);
//	if (StatusComponent->IsDead())
//	{
//		ZLOG(Warning, TEXT("Dead!!"));
//	}
//
//	return FinalDamage;
//}

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
	/* AnimInstance üũ */
	auto CharacterAnim = GetCharacterAnimInstance();
	check(CharacterAnim != nullptr);

	if (CurrentWeapon)
	{
		CurrentWeapon->SetIsEquipped(false);
		/*
			�����ϰ� �ִ� Weapon�� �ٲ� �� ���� AnimInstance�� ���õ� Delegate�� ���� �����.
			RemoveAll�� �ڽ�Ʈ�� ��� �������� �𸣱⶧���� �����ϴٸ� �ٲٴ°ɷ�
		*/
		CurrentWeapon->OnWeaponFired.RemoveAll(CharacterAnim);
	}

	CurrentWeapon = NewWeapon;

	if (NewWeapon)
	{
		NewWeapon->SetIsEquipped(true);
		/*
			WeaponCategory�� Gun�� ��쿡�� AnimInstance�� �� ����.
		*/
		if (EWeaponCategory::Gun == NewWeapon->GetWeaponCategory())
		{
			CharacterAnim->SetIsEquipGun(true);
		}
		CharacterAnim->BindFireMontage(NewWeapon);
		//bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		// CurrentWeaponInfo widget�� ���ε�
		PlayerController->GetZHUD()->GetUserHUD()->GetCurrentWeaponInfoWidget()->BindWeapon(CurrentWeapon);
	}
	else
	{
		CharacterAnim->SetIsEquipGun(false);
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

void AZCharacter::SetActive(bool bActive)
{
	if (bActive)
	{
		GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
		GetMesh()->SetSimulatePhysics(false);
	}

	Super::SetActive(bActive);
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

AZWeapon * const AZCharacter::GetCurrentWeapon()
{
	return CurrentWeapon;
}

UZPlayerAnimInstance * const AZCharacter::GetCharacterAnimInstance()
{
	return Cast<UZPlayerAnimInstance>(GetAnimInstance());
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

void AZCharacter::OnDead()
{
	Super::OnDead();

	//PlayerController->SetCinematicMode(true, false, false, true, true);

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
	// Character�� ���߿� ���ִ��� üũ
	if (GetCharacterMovement()->IsFalling())
	{
		//���߿� ���ִٸ� �״�� ����.
		return;
	}

	// ���� Character�� ���� �������� üũ.
	if (GetCharacterMovement()->IsCrouching())
	{
		// ���� ������ ���
		ACharacter::UnCrouch();
	}
	else
	{
		// ���� ���°� �ƴ� ���
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
		switch (CurrentWeapon->GetWeaponCategory())
		{
			case EWeaponCategory::Gun:
			{
				/*
					�������� ��� return
				*/
				auto Gun = Cast<AZGun>(CurrentWeapon);
				check(Gun != nullptr);
				if (Gun->IsReloading())
				{
					return;
				}
			}
			case EWeaponCategory::Grenade:
			{
				/*
					����ź�� �� ��Ȳ�̸� return
				*/
				

				break;
			}
		}
	}
	
	auto MyCharacterMovement = GetCharacterMovement();
	if (MyCharacterMovement)
	{
		// Character�� ���߿� �ְų� ���� ���¶�� �״�� ����.
		if (MyCharacterMovement->IsFalling() || MyCharacterMovement->IsCrouching())
		{
			return;
		}

		if (!IsSprinting())
		{
			// SprintSpeed�� ���� �����ϰ� Sprint ���� ����
			MyCharacterMovement->MaxWalkSpeed = SprintSpeed;
			SetIsSprinting(true);
			auto CharacterAnim = GetCharacterAnimInstance();
			check(nullptr != CharacterAnim);
			CharacterAnim->SetIsSprinting(true);
			//bUseControllerRotationYaw = false
		}

	}

}

void AZCharacter::SprintRelease()
{
	auto MyCharacterMovement = GetCharacterMovement();
	if (MyCharacterMovement)
	{
		// �ٰ��ִ� ���¶��
		if (IsSprinting())
		{
			// WalkSpeed�� ���� �����ϰ� Sprint ���� ����
			MyCharacterMovement->MaxWalkSpeed = WalkSpeed;
			SetIsSprinting(false);

			auto CharacterAnim = GetCharacterAnimInstance();
			check(nullptr != CharacterAnim);
			CharacterAnim->SetIsSprinting(false);
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
		switch (CurrentWeapon->GetWeaponCategory())
		{
			case EWeaponCategory::Gun:
			{
				/*
					�������� ��� return
				*/
				auto Gun = Cast<AZGun>(CurrentWeapon);
				check(Gun != nullptr);
				if (Gun->IsReloading())
				{
					return;
				}
			}
			case EWeaponCategory::Grenade:
			{
				/*
					����ź�� �� ��Ȳ�̸� return
				*/


				break;
			}
		}
	}

	if (IsAiming())
	{
		AimRelease();
	}

	// Character�� ���� ���� �������� üũ.
	if (GetCharacterMovement()->IsCrouching())
	{
		// ���� ���¶�� UnCrouch() �޼ҵ� ����
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
		switch (CurrentWeapon->GetWeaponCategory())
		{
			case EWeaponCategory::Gun:
			{
				/*
					�������� ��� return
				*/
				auto Gun = Cast<AZGun>(CurrentWeapon);
				check(Gun != nullptr);
				if (Gun->IsReloading())
				{
					return;
				}
			}
			case EWeaponCategory::Grenade:
			{
				/*
					����ź�� �� ��Ȳ�̸� return
				*/


				break;
			}
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
				// Inventory�� ȭ�鿡 �ִ� ���.
				UserHUDWidget->RemoveInventoryWidget();
			}
			else
			{
				// Inventory�� ȭ�鿡 ���� ���.
				UserHUDWidget->DrawInventoryWidget();
			}
		}
	}

}

void AZCharacter::Attack()
{
	//AnimInstance->PlayMontage(TEXT("EquipRifle"));

	if (!IsEquipWeapon())
	{
		return;
	}

	switch (CurrentWeapon->GetWeaponCategory())
	{
		case EWeaponCategory::Gun:
		{
			/*
				�������� ��� return
			*/
			auto Gun = Cast<AZGun>(CurrentWeapon);
			check(Gun != nullptr);
			if (Gun->IsReloading())
			{
				return;
			}
		}
		case EWeaponCategory::Grenade:
		{
			/*
				����ź�� �� ��Ȳ�̸� return
			*/


			break;
		}
	}

	if (IsSwitchingWeapon())
	{
		return;
	}

	//CurrentWeapon->SetWantsToFire(true);
	CurrentWeapon->Fire();
	//GetCharacterAnimInstance()->PlayFireMontage(CurrentWeapon);
}

void AZCharacter::AttackEnd()
{
	if (!IsEquipWeapon())
	{
		return;
	}

	if (EWeaponCategory::Gun == CurrentWeapon->GetWeaponCategory())
	{
		//CurrentWeapon->SetWantsToFire(false);
		CurrentWeapon->FireEnd();
	}
}

void AZCharacter::Aim()
{
	if (!IsEquipWeapon())
	{
		return;
	}
	else
	{
		if (CurrentWeapon->GetWeaponCategory() == EWeaponCategory::Gun)
		{
			/*
				�������� ��� return
			*/
			auto Gun = Cast<AZGun>(CurrentWeapon);
			check(Gun != nullptr);
			if (Gun->IsReloading())
			{
				return;
			}
		}
		else
		{
			/*
				������ ����� Aim�Ұ����̹Ƿ� return.
			*/
			ZLOG(Error, TEXT("No Aim"));
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
	auto CharacterAnim = GetCharacterAnimInstance();
	check(nullptr != CharacterAnim);
	CharacterAnim->SetIsAiming(true);
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
	auto CharacterAnim = GetCharacterAnimInstance();
	check(nullptr != CharacterAnim); 
	CharacterAnim->SetIsAiming(false);
}

void AZCharacter::Reload()
{
	if (!IsEquipWeapon())
	{
		return;
	}
	else
	{
		auto Weapon = Cast<AZGun>(CurrentWeapon);
		if (nullptr == Weapon)
		{
			return;
		}

		if (Weapon->IsReloading())
		{
			return;
		}
		else
		{
			if (!Weapon->IsCanReload())
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

		if (IsAiming())
		{
			AimRelease();
		}

		Weapon->SetIsReloading(true);
		auto CharacterAnim = GetCharacterAnimInstance();
		check(nullptr != CharacterAnim);
		CharacterAnim->PlayMontage(TEXT("ReloadRifle"));

	}

}

void AZCharacter::DropWeapon()
{
	if (!IsEquipWeapon())
	{
		return;
	}

	switch (CurrentWeapon->GetWeaponCategory())
	{
		case EWeaponCategory::Gun:
		{
			/*
				�������� ��� return
			*/
			auto Gun = Cast<AZGun>(CurrentWeapon);
			check(Gun != nullptr);
			if (Gun->IsReloading())
			{
				return;
			}
		}
		case EWeaponCategory::Grenade:
		{
			/*
				����ź�� �� ��Ȳ�̸� return
			*/


			break;
	}
	}

	ItemStatusComponent->DropWeapon(CurrentWeapon->GetWeaponInventoryIndex());

}

void AZCharacter::SwitchWeapon(int32 NewWeaponIndex)
{
	// �ش� Slot�� Weapon�� ���� ���(null�� ���)
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
		/*
			Fire������ ��� Fire���� ����.
		*/
		CurrentWeapon->FireEnd();

		/*
			Weapon slot�� ���� �޶���.
		*/
		switch (CurrentWeapon->GetWeaponInventoryIndex())
		{
			case 0:
			{
				// ���� Weapon�� Secondary socket���� �ű�.
				CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SecondaryWeaponSocketName);
				break;
			}
			case 1:
			{
				// ���� Weapon�� Third socket���� �ű�.
				CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ThirdWeaponSocketName);
				break;
			}
			case 2:
			{
				break;
			}
			case 3:
			{
				CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, GrenadeWeaponSocketName);
				break;
			}
			default:
			{
				break;
			}
		}



	}

	SetCurrentWeapon(ItemStatusComponent->GetWeaponFromWeaponInventory(NewWeaponIndex));
	auto CharacterAnim = GetCharacterAnimInstance();
	check(nullptr != CharacterAnim);
	switch (CurrentWeapon->GetWeaponCategory())
	{
		case EWeaponCategory::Gun:
		{
			/*
				�������� ��� return
			*/
			auto Gun = Cast<AZGun>(CurrentWeapon);
			check(Gun != nullptr);
			if (Gun->IsReloading())
			{
				return;
			}
			// Gun�̹Ƿ� true ����
			CharacterAnim->SetIsEquipGun(true);

			break;
		}
		case EWeaponCategory::Grenade:
		{
			/*
				����ź�� �� ��Ȳ�̸� return
			*/
			ZLOG_S(Warning);
			// Gun�� �ƴϹǷ� false ����
			CharacterAnim->SetIsEquipGun(false);
			// OnGrenadeThrow�� ���ε�.
			auto Grenade = Cast<AZGrenade>(CurrentWeapon);
			check(nullptr != Grenade);
			ZLOG(Error, TEXT("Bind ThrowGrenade"));
			CharacterAnim->OnGrenadeThrow.BindUObject(Grenade, &AZGrenade::ThrowGrenade);

			break;
		}
	}
	// �� Weapon�� Main socket���� �ű�.
	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, MainWeaponSocketName);
	SetIsSwitchingWeapon(true);
	CharacterAnim->PlayMontage(TEXT("EquipRifle"));

}

void AZCharacter::Slot1()
{
	SwitchWeapon(0);
}

void AZCharacter::Slot2()
{
	SwitchWeapon(1);
}

void AZCharacter::Slot4()
{
	SwitchWeapon(3);
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

void AZCharacter::Ragdoll()
{
	//PlayerController->SetCinematicMode(true, false, false, true, true);
	//GetCharacterMovement()->NavAgentProps.bCanJump = false;

	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}