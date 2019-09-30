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
#include "ZPlayerStatusComponent.h"
#include "ZChangeFireModeInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AZCharacter::AZCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	// Create character status component
	StatusComponent = CreateDefaultSubobject<UZPlayerStatusComponent>(TEXT("StatusComponent"));

	// Set Character Properties
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->JumpZVelocity = 700.f;

	MainWeaponSocketName = FName(TEXT("weapon_r"));
	MainWeaponShotgunSocketName = FName(TEXT("weapon_r_shotgun"));
	SecondaryWeaponSocketName = FName(TEXT("weapon_secondary"));
	ThirdWeaponSocketName = FName(TEXT("weapon_third"));
	KnifeSocketName = FName(TEXT("weapon_knife"));
	GrenadeWeaponSocketName = FName(TEXT("weapon_grenade"));

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

	Tags.Add(TEXT("Player"));
}

// Called when the game starts or when spawned
void AZCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AZCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	PlayerController = Cast<AZPlayerController>(NewController);
}

// Called every frame
void AZCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* 팅겨서 일단 주석처리. */
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
	//PlayerInputComponent->BindAction(TEXT("ToggleInventory"), IE_Pressed, this, &AZCharacter::ToggleInventory);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AZCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Released, this, &AZCharacter::AttackEnd);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Pressed, this, &AZCharacter::Aim);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Released, this, &AZCharacter::AimRelease);
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &AZCharacter::Reload);
	PlayerInputComponent->BindAction(TEXT("DropWeapon"), IE_Pressed, this, &AZCharacter::DropWeapon);
	PlayerInputComponent->BindAction(TEXT("Slot1"), IE_Pressed, this, &AZCharacter::Slot1);
	PlayerInputComponent->BindAction(TEXT("Slot2"), IE_Pressed, this, &AZCharacter::Slot2);
	PlayerInputComponent->BindAction(TEXT("Slot3"), IE_Pressed, this, &AZCharacter::Slot3);
	PlayerInputComponent->BindAction(TEXT("Slot4"), IE_Pressed, this, &AZCharacter::Slot4);
	PlayerInputComponent->BindAction(TEXT("ChangeFireMode"), IE_Pressed, this, &AZCharacter::ChangeFireMode);
	//PlayerInputComponent->BindAction(TEXT("ToggleInGameMenu"), IE_Pressed, this, &AZCharacter::ToggleInGameMenu);
	//PlayerInputComponent->BindAction(TEXT("RemoveWidgetFromTop"), IE_Pressed, this, &AZCharacter::RemoveWidgetFromTop);

	// For debug
	PlayerInputComponent->BindAction(TEXT("AddMoney"), IE_Pressed, this, &AZCharacter::AddMoney);
	PlayerInputComponent->BindAction(TEXT("DamageSelf"), IE_Pressed, this, &AZCharacter::DamageSelf);
	PlayerInputComponent->BindAction(TEXT("Ragdoll"), IE_Pressed, this, &AZCharacter::Ragdoll);



}

float AZCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	auto MyPC = GetController<AZPlayerController>();
	if (MyPC)
	{
		auto MyHUD = MyPC->GetUserHUD();
		if (MyHUD)
		{
			MyHUD->CallFadeOutBloodSplatterAnim();
		}
	}
	//StatusComponent->AdjustCurrentHP(-FinalDamage);

	return FinalDamage;
}

void AZCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZCharacter, CurrentWeapon);
	DOREPLIFETIME_CONDITION(AZCharacter, bIsAiming, COND_OwnerOnly);
}

void AZCharacter::Revive()
{
	Super::Revive();

	if (PlayerController)
	{
		EnableInput(PlayerController);
	}
}

FHitResult AZCharacter::GetTraceHitFromActorCameraView(float Distance)
{
	FVector CamLoc;
	FRotator CamRot;
	auto MyPC = GetController<AZPlayerController>();
	if (nullptr == MyPC)
	{
		return FHitResult();
	}

	MyPC->GetPlayerViewPoint(CamLoc, CamRot);

	const FVector Direction = CamRot.Vector();
	const FVector TraceStart = GetActorLocation();
	const FVector TraceEnd = TraceStart + (Direction * Distance);

	return GetTraceHit(TraceStart, TraceEnd);
}

void AZCharacter::AttachWeapon(class AZWeapon* Weapon, const FName & SocketName)
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Weapon : %s"), *Weapon->GetItemName()));
	if (nullptr == Weapon)
	{
		ZLOG(Error, TEXT("Invalid weapon.."));
		return;
	}
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);

	auto CharacterAnim = GetCharacterAnimInstance();
	if (!::IsValid(CharacterAnim))
	{
		ZLOG(Error, TEXT("Invalid anim instance.."));
		return;
	}

	if (EWeaponCategory::Grenade == Weapon->GetWeaponCategory())
	{
		ZLOG_S(Warning);
		/* OnGrenadeThrow에 바인딩. */
		auto Grenade = Cast<AZGrenade>(CurrentWeapon);
		check(Grenade);
		ZLOG(Error, TEXT("Bind ThrowGrenade"));
		CharacterAnim->OnGrenadeThrow.BindUObject(Grenade, &AZGrenade::ThrowGrenade);
	}
}

void AZCharacter::DetachWeapon(int32 NewWeaponIndex)
{
	auto Weapon = ItemStatusComponent->GetWeaponFromWeaponInventory(NewWeaponIndex);
	if (nullptr == Weapon)
	{
		ZLOG(Error, TEXT("Invalid weapon.."));
		return;
	}

	Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void AZCharacter::OnRep_IsAiming()
{
	if (bIsAiming)
	{
	
	}
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
	/* Client면 Server로 RPC */
	if (!HasAuthority())
	{
		ServerEquipWeapon(NewWeapon);
		return;
	}
	
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("SetWeapon."));
	if (CurrentWeapon)
	{
		CurrentWeapon->SetIsEquipped(false);
	}

	if (NewWeapon)
	{
		NewWeapon->SetIsEquipped(true);
		GetCharacterMovement()->bOrientRotationToMovement = false;

	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

	CurrentWeapon = NewWeapon;

	OnRep_CurrentWeapon();



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

AZInteractional * AZCharacter::GetInteractionalInView()
{
	auto MyPC = GetController<AZPlayerController>();
	if (!MyPC)
	{
		return nullptr;
	}

	FVector CamLoc;
	FRotator CamRot;
	MyPC->GetPlayerViewPoint(CamLoc, CamRot);

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

UZPlayerStatusComponent * const AZCharacter::GetPlayerStatusComponent() const
{
	return Cast<UZPlayerStatusComponent>(StatusComponent);
}

AZWeapon * const AZCharacter::GetCurrentWeapon()
{
	return CurrentWeapon;
}

UZPlayerAnimInstance * const AZCharacter::GetCharacterAnimInstance()
{
	return Cast<UZPlayerAnimInstance>(GetAnimInstance());
}

const FName & AZCharacter::GetMainWeaponSocketName() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return MainWeaponSocketName;
}

const FName & AZCharacter::GetMainWeaponShotgunSocketName() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return MainWeaponShotgunSocketName;
}

const FName & AZCharacter::GetSecondaryWeaponSocketName() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return SecondaryWeaponSocketName;
}

const FName & AZCharacter::GetThirdWeaponSocketName() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return ThirdWeaponSocketName;
}

const FName & AZCharacter::GetKnifeSocketName() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return KnifeSocketName;
}

const FName & AZCharacter::GetGrenadeWeaponSocketName() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return GrenadeWeaponSocketName;
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

	auto MyAnim = GetCharacterAnimInstance();
	if (::IsValid(MyAnim))
	{
		MyAnim->bIsDead = true;
	}

	if (PlayerController)
	{
		DisableInput(PlayerController);
		PlayerController->GetUserHUD()->DrawEndGameMenuWidget();
	}

}

void AZCharacter::OnRemoved()
{
	Super::OnRemoved();

	if (bIsPooling)
	{
		auto MyAnim = GetCharacterAnimInstance();
		if (::IsValid(MyAnim))
		{
			MyAnim->bIsDead = false;
		}
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
		if (IsAiming())
		{
			SetCurrentSpeed(AimingWalkSpeedCrouched);
		}
		else
		{
			SetCurrentSpeed(WalkSpeedCrouched);
		}
	}
	else
	{
		// 앉은 상태가 아닌 경우
		ACharacter::Crouch();
		if (IsAiming())
		{
			SetCurrentSpeed(AimingWalkSpeed);
		}
		else
		{
			SetCurrentSpeed(WalkSpeed);
		}
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
				장전중인 경우 return
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
				수류탄을 깐 상황이면 return
			*/


			break;
		}
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
			//MyCharacterMovement->MaxWalkSpeed = SprintSpeed;
			SetIsSprinting(true);
			auto CharacterAnim = GetCharacterAnimInstance();
			check(CharacterAnim);
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
		// 뛰고있는 상태라면
		if (IsSprinting())
		{
			// WalkSpeed로 값을 변경하고 Sprint 상태 변경
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
				장전중인 경우 return
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
				수류탄을 깐 상황이면 return
			*/


			break;
		}
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
		switch (CurrentWeapon->GetWeaponCategory())
		{
		case EWeaponCategory::Gun:
		{
			/*
				장전중인 경우 return
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
				수류탄을 깐 상황이면 return
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
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Interact."));
		//ServerOnInteract(InteractionActor);
		InteractionActor->OnInteraction(this);
	}
}

//void AZCharacter::ToggleInventory()
//{
//	auto UserHUDWidget = PlayerController->GetUserHUD();
//	if (UserHUDWidget)
//	{
//		UserHUDWidget->ToggleInventoryWidget();
//	}
//
//}

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
			장전중인 경우 return
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
			수류탄을 깐 상황이면 return
		*/


		break;
	}
	}

	if (IsSwitchingWeapon())
	{
		return;
	}

	if (IsSprinting())
	{
		SprintRelease();
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
				장전중인 경우 return
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
				나머지 무기는 Aim불가능이므로 return.
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

		if (nullptr == ItemStatusComponent->GetItemByName(Weapon->GetUseAmmoName()))
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

		auto Montage = Weapon->FindMontage(TEXT("Reload"));
		if (Montage)
		{
			CharacterAnim->Montage_Play(Montage);
		}

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
			장전중인 경우 return
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
			수류탄을 깐 상황이면 return
		*/


		break;
	}
	}

	ItemStatusComponent->DropWeapon(CurrentWeapon->GetWeaponInventoryIndex());

}

void AZCharacter::SwitchWeapon(int32 NewWeaponIndex)
{
	ZLOG_S(Error);
	/* Server에서 동작해야함. */
	if (!HasAuthority())
	{
		ServerSwitchWeapon(NewWeaponIndex);
		return;
	}
	ZLOG_S(Error);
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
		/*
			Fire상태인 경우 Fire상태 해제.
		*/
		CurrentWeapon->FireEnd();

		/*
			Weapon slot에 따라 달라짐.
		*/

		switch (CurrentWeapon->GetWeaponInventoryIndex())
		{
			case EWeaponSlot::Main1:
			{
				/*
					장전중인 경우 장전 취소
				*/
				auto Gun = Cast<AZGun>(CurrentWeapon);
				check(Gun != nullptr);
				if (Gun->IsReloading())
				{
					Gun->SetIsReloading(false);
				}

				// 기존 Weapon은 Secondary socket으로 옮김.
				AttachWeapon(CurrentWeapon, SecondaryWeaponSocketName);
				MulticastAttachWeapon(CurrentWeapon, SecondaryWeaponSocketName);
				break;
			}
			case EWeaponSlot::Main2:
			{
				/*
					장전중인 경우 장전 취소
				*/
				auto Gun = Cast<AZGun>(CurrentWeapon);
				check(Gun != nullptr);
				if (Gun->IsReloading())
				{
					Gun->SetIsReloading(false);
				}

				// 기존 Weapon은 Third socket으로 옮김.
				AttachWeapon(CurrentWeapon, ThirdWeaponSocketName);
				MulticastAttachWeapon(CurrentWeapon, ThirdWeaponSocketName);
				break;
			}
			case EWeaponSlot::Knife:
			{
				break;
			}
			case EWeaponSlot::Grenade:
			{
				AttachWeapon(CurrentWeapon, GrenadeWeaponSocketName);
				MulticastAttachWeapon(CurrentWeapon, GrenadeWeaponSocketName);
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
	if (!::IsValid(CharacterAnim))
	{
		ZLOG(Error, TEXT("AnimInstance invalid."));
		return;
	}

	FName SocketName = GetMainWeaponSocketName();
	switch (CurrentWeapon->GetWeaponCategory())
	{
	case EWeaponCategory::Gun:
	{
		// Gun이므로 true 셋팅
		CharacterAnim->SetIsEquipGun(true);
		auto Gun = Cast<AZGun>(CurrentWeapon);
		check(Gun != nullptr);
		if (EGunType::Shotgun == Gun->GetGunType())
		{
			SocketName = GetMainWeaponShotgunSocketName();
		}

		break;
	}
	//case EWeaponCategory::Grenade:
	//{
	//	/*
	//		수류탄을 깐 상황이면 return
	//	*/
	//	ZLOG_S(Warning);
	//	// Gun이 아니므로 false 셋팅
	//	CharacterAnim->SetIsEquipGun(false);
	//	// OnGrenadeThrow에 바인딩.
	//	auto Grenade = Cast<AZGrenade>(CurrentWeapon);
	//	check(nullptr != Grenade);
	//	ZLOG(Error, TEXT("Bind ThrowGrenade"));
	//	CharacterAnim->OnGrenadeThrow.BindUObject(Grenade, &AZGrenade::ThrowGrenade);

	//	break;
	//}
	}

	// 새 Weapon은 Main socket으로 옮김.
	AttachWeapon(CurrentWeapon, SocketName);
	MulticastAttachWeapon(CurrentWeapon, SocketName);

	SetIsSwitchingWeapon(true);

	MulticastPlayMontage(TEXT("EquipRifle"));
	CharacterAnim->PlayCharacterMontage(TEXT("EquipRifle"));

}

void AZCharacter::Slot1()
{
	ZLOG_S(Error);
	SwitchWeapon(0);
}

void AZCharacter::Slot2()
{
	ZLOG_S(Error);
	SwitchWeapon(1);
}

void AZCharacter::Slot3()
{
	SwitchWeapon(2);
}

void AZCharacter::Slot4()
{
	SwitchWeapon(3);
}

void AZCharacter::ChangeFireMode()
{
	if (nullptr == CurrentWeapon)
	{
		return;
	}

	auto ChangeModeInterface = Cast<IZChangeFireModeInterface>(CurrentWeapon);
	if (ChangeModeInterface)
	{
		ChangeModeInterface->ChangeFireMode();
	}

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
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}

bool AZCharacter::ServerEquipWeapon_Validate(AZWeapon * NewWeapon)
{
	return true;
}

void AZCharacter::ServerEquipWeapon_Implementation(AZWeapon * NewWeapon)
{
	SetCurrentWeapon(NewWeapon);
}

bool AZCharacter::ServerSwitchWeapon_Validate(int32 NewWeaponIndex)
{
	return true;
}

void AZCharacter::ServerSwitchWeapon_Implementation(int32 NewWeaponIndex)
{
	SwitchWeapon(NewWeaponIndex);
}

bool AZCharacter::ServerSetCrouch_Validate(bool bCrouch)
{
	return true;
}

void AZCharacter::ServerSetCrouch_Implementation(bool bCrouch)
{
	if (bCrouch)
	{

	}
	else
	{
		
	}

}

void AZCharacter::MulticastPlayMontage_Implementation(const FString & MontageName)
{
	auto CharacterAnim = GetCharacterAnimInstance();
	if (!::IsValid(CharacterAnim))
	{
		ZLOG(Error, TEXT("Invalid AnimInstance."));
		return;
	}

	CharacterAnim->PlayCharacterMontage(TEXT("EquipRifle"));
}

bool AZCharacter::MulticastAttachWeapon_Validate(AZWeapon* Weapon, FName SocketName)
{
	return true;
}

void AZCharacter::MulticastAttachWeapon_Implementation(AZWeapon* Weapon, FName SocketName)
{
	AttachWeapon(Weapon, SocketName);
}

void AZCharacter::OnRep_CurrentWeapon()
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("OnRep_CurrentWeapon."));
	auto CharacterAnim = GetCharacterAnimInstance();
	if (!::IsValid(CharacterAnim))
	{
		ZLOG(Error, TEXT("Invalid AnimInstance."));
		return;
	}

	if (CurrentWeapon)
	{
		if (EWeaponCategory::Gun == CurrentWeapon->GetWeaponCategory())
		{
			CharacterAnim->SetIsEquipGun(true);
		}
		else
		{
			CharacterAnim->SetIsEquipGun(false);
		}

		// CurrentWeaponInfo widget에 바인딩
		auto MyPC = GetController<AZPlayerController>();
		if (MyPC)
		{
			auto UserHUD = MyPC->GetUserHUD();
			if (UserHUD)
			{
				UserHUD->GetCurrentWeaponInfoWidget()->BindWeapon(CurrentWeapon);
			}
		}
	}
	else
	{
		CharacterAnim->SetIsEquipGun(false);

		auto MyPC = GetController<AZPlayerController>();
		if (MyPC)
		{
			auto UserHUD = MyPC->GetUserHUD();
			if (UserHUD)
			{
				UserHUD->GetCurrentWeaponInfoWidget()->ClearWidget();
			}
		}
	}
}

