// Fill out your copyright notice in the Description page of Project Settings.


#include "ZCharacter.h"
#include "ZInteractional.h"
#include "ZCharacterItemStatusComponent.h"
#include "ZPlayerController.h"
#include "ZUserHUD.h"
#include "ZCurrentWeaponInfoWidget.h"
#include "ZGun.h"
#include "ZGrenade.h"
#include "ZProjectile.h"
#include "ZGameInstance.h"
#include "ZCharacterAnimInstance.h"
#include "ZPlayerAnimInstance.h"
#include "ZPlayerController.h"
#include "ZPlayerStatusComponent.h"
#include "ZChangeFireModeInterface.h"
#include "UI/ZUserNameWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
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

	/* Create Widget component */
	NameComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameComponent"));
	NameComponent->bOwnerNoSee = true;
	NameComponent->SetupAttachment(RootComponent);

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

}

// Called every frame
void AZCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* �ðܼ� �ϴ� �ּ�ó��. */
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

	//// For debug
	//PlayerInputComponent->BindAction(TEXT("AddMoney"), IE_Pressed, this, &AZCharacter::AddMoney);
	//PlayerInputComponent->BindAction(TEXT("DamageSelf"), IE_Pressed, this, &AZCharacter::DamageSelf);
	//PlayerInputComponent->BindAction(TEXT("Ragdoll"), IE_Pressed, this, &AZCharacter::Ragdoll);



}

float AZCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	auto MyPC = GetController<AZPlayerController>();
	if (MyPC)
	{
		MyPC->ClientBloodSplatter();
		if (IsDead())
		{
			if (CurrentWeapon)
			{
				CurrentWeapon->OnDropped();
			}
			ClientOnDead();
		}

	}

	return FinalDamage;
}

void AZCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZCharacter, CurrentWeapon);
	DOREPLIFETIME(AZCharacter, bIsAiming);
}

void AZCharacter::Revive()
{
	Super::Revive();

	auto MyPC = GetController<AZPlayerController>();
	if (MyPC)
	{
		EnableInput(MyPC);
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
	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Weapon : %s"), *Weapon->GetItemName()));
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

	//if (EWeaponCategory::Grenade == Weapon->GetWeaponCategory())
	//{
	//	ZLOG_S(Warning);
	//	/* OnGrenadeThrow�� ���ε�. */
	//	auto Grenade = Cast<AZGrenade>(CurrentWeapon);
	//	check(Grenade);
	//	ZLOG(Error, TEXT("Bind ThrowGrenade"));
	//	CharacterAnim->OnGrenadeThrow.BindUObject(Grenade, &AZGrenade::ThrowGrenade);
	//}
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

void AZCharacter::SetUserName(const FString& Name)
{
	ZLOG_S(Error);
	auto UserNameWidget = Cast<UZUserNameWidget>(NameComponent->GetUserWidgetObject());
	if (UserNameWidget)
	{
		UserNameWidget->SetUserName(Name);
	}
	else
	{
		ZLOG_S(Error);
	}

}

void AZCharacter::OnRep_IsAiming()
{
	if (bIsAiming)
	{
		if (bIsCrouched)
		{
			SetCharacterCrouchWalkSpeed(AimingWalkSpeedCrouched);
		}
		else
		{
			SetCharacterWalkSpeed(AimingWalkSpeed);
		}
		//SetCurrentSpeed(AimingWalkSpeed);
	}
	else
	{
		if (bIsCrouched)
		{
			SetCharacterCrouchWalkSpeed(WalkSpeedCrouched);
		}
		else
		{
			SetCharacterWalkSpeed(WalkSpeed);
		}
	}

	auto CharacterAnim = GetCharacterAnimInstance();
	if (!::IsValid(CharacterAnim))
	{
		ZLOG(Error, TEXT("Invalid anim instance.."));
		return;
	}
	CharacterAnim->SetIsAiming(bIsAiming);
}

void AZCharacter::OnRep_IsSprinting()
{
	Super::OnRep_IsSprinting();
	//if (bIsSprinting)
	//{
	//	SetCharacterWalkSpeed(SprintSpeed);
	//}
	//else
	//{
	//	SetCharacterWalkSpeed(WalkSpeed);
	//}

}

void AZCharacter::MulticastUpdatePlayerName_Implementation(const FString & NewName)
{
	SetUserName(NewName);
}

void AZCharacter::SetIsAiming(bool bNewState)
{
	if (!HasAuthority())
	{
		ServerSetAiming(bNewState);
		return;
	}

	/* ���߿��� ���ظ���. */
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	/* Sprint ���� */
	if (IsSprinting())
	{
		SetIsSprinting(false);
	}

	bIsAiming = bNewState;
	OnRep_IsAiming();
}

void AZCharacter::SetIsSwitchingWeapon(bool NewState)
{
	bIsSwitchingWeapon = NewState;
}

void AZCharacter::SetCurrentWeapon(AZWeapon * NewWeapon)
{
	ZLOG_S(Warning);
	/* Client�� Server�� RPC */
	if (!HasAuthority())
	{
		ServerEquipWeapon(NewWeapon);
		return;
	}

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

bool AZCharacter::IsCharacterCrouched() const
{
	return bIsCrouched;
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

FRotator AZCharacter::GetAimOffset() const
{
	return ActorToWorld().InverseTransformVectorNoScale(GetBaseAimRotation().Vector()).Rotation();
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
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return MainWeaponSocketName;
}

const FName & AZCharacter::GetMainWeaponShotgunSocketName() const
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return MainWeaponShotgunSocketName;
}

const FName & AZCharacter::GetSecondaryWeaponSocketName() const
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return SecondaryWeaponSocketName;
}

const FName & AZCharacter::GetThirdWeaponSocketName() const
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return ThirdWeaponSocketName;
}

const FName & AZCharacter::GetKnifeSocketName() const
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return KnifeSocketName;
}

const FName & AZCharacter::GetGrenadeWeaponSocketName() const
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return GrenadeWeaponSocketName;
}

void AZCharacter::OnDead()
{
	Super::OnDead();

	auto MyAnim = GetCharacterAnimInstance();
	if (::IsValid(MyAnim))
	{
		MyAnim->bIsDead = true;
	}

	auto MyPC = GetController<AZPlayerController>();
	if (MyPC && MyPC->IsLocalPlayerController())
	{
		//DisableInput(MyPC);
		MyPC->GetUserHUD()->DrawEndGameMenuWidget();

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
	if (NewAxisValue == 0.f)
	{
		return;
	}

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
	if (NewAxisValue == 0.f)
	{
		return;
	}

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
	//if (!HasAuthority())
	//{
	//	ServerToggleCrouch();
	//	return;
	//}
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("ToggleCrouch"));
	// Character�� ���߿� ���ִ��� üũ
	if (GetCharacterMovement()->IsFalling())
	{
		//���߿� ���ִٸ� �״�� ����.
		return;
	}

	// ���� Character�� ���� �������� üũ.
	//if (GetCharacterMovement()->IsCrouching())
	if (bIsCrouched)
	{
		// ���� ������ ���
		ACharacter::UnCrouch();
		//if (IsAiming())
		//{
		//	SetCurrentSpeed(AimingWalkSpeed);
		//}
		//else
		//{
		//	SetCurrentSpeed(WalkSpeed);
		//}
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("UnCrouch"));

	}
	else
	{
		if (IsSprinting())
		{
			SprintRelease();
		}

		// ���� ���°� �ƴ� ���
		ACharacter::Crouch();
		//if (IsAiming())
		//{
		//	SetCurrentSpeed(AimingWalkSpeedCrouched);
		//}
		//else
		//{
		//	SetCurrentSpeed(WalkSpeedCrouched);
		//}
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Crouch"));

	}


	//if (bIsCrouched)
	//{
	//	if (IsAiming())
	//	{
	//		SetCharacterCrouchWalkSpeed(AimingWalkSpeedCrouched);
	//	}
	//	else
	//	{
	//		SetCharacterWalkSpeed(WalkSpeedCrouched);
	//	}
	//}
	//else
	//{
	//	if (IsAiming())
	//	{
	//		SetCharacterCrouchWalkSpeed(AimingWalkSpeed);
	//	}
	//	else
	//	{
	//		SetCharacterWalkSpeed(WalkSpeed);
	//	}
	//}

}

void AZCharacter::Sprint()
{
	if (!HasAuthority())
	{
		ServerSetSprint(true);
		return;
	}

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
		//if (MyCharacterMovement->IsFalling() || MyCharacterMovement->IsCrouching())
		if (MyCharacterMovement->IsFalling() || bIsCrouched)
		{
			return;
		}

		if (!IsSprinting())
		{
			// SprintSpeed�� ���� �����ϰ� Sprint ���� ����
			//MyCharacterMovement->MaxWalkSpeed = SprintSpeed;

			SetIsSprinting(true);

			//auto CharacterAnim = GetCharacterAnimInstance();
			//check(CharacterAnim);
			//CharacterAnim->SetIsSprinting(true);
			//bUseControllerRotationYaw = false
		}

	}

}

void AZCharacter::SprintRelease()
{
	if (!HasAuthority())
	{
		ServerSetSprint(false);
		return;
	}

	auto MyCharacterMovement = GetCharacterMovement();
	if (MyCharacterMovement)
	{
		// �ٰ��ִ� ���¶��
		if (IsSprinting())
		{
			// WalkSpeed�� ���� �����ϰ� Sprint ���� ����
			//MyCharacterMovement->MaxWalkSpeed = WalkSpeed;
			SetIsSprinting(false);

			//auto CharacterAnim = GetCharacterAnimInstance();
			//check(nullptr != CharacterAnim);
			//CharacterAnim->SetIsSprinting(false);

		}

	}
}

void AZCharacter::Jump()
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Jump"));
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
	//if (GetCharacterMovement()->IsCrouching())
	if (bIsCrouched)
	{
		// ���� ���¶�� UnCrouch() �޼ҵ� ����
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Jump"));
		ACharacter::UnCrouch();
		ToggleCrouch();
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

	if (IsSprinting())
	{
		SprintRelease();
	}

	//CurrentWeapon->SetWantsToFire(true);
	CurrentWeapon->StartFire();
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
		CurrentWeapon->StopFire();
	}
}

void AZCharacter::Aim()
{
	if (!IsEquipWeapon())
	{
		ZLOG(Error, TEXT("Not equip weapon."));
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
				ZLOG(Error, TEXT("Is reloading.."));
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
		ZLOG(Error, TEXT("Is switching.."));
		return;
	}

	if (GetCharacterMovement()->IsFalling())
	{
		ZLOG(Error, TEXT("Is falling.."));
		return;
	}

	//if (IsSprinting())
	//{
	//	SprintRelease();
	//}

	//if (GetCharacterMovement()->IsCrouching())
	//{
	//	GetCharacterMovement()->MaxWalkSpeedCrouched = AimingWalkSpeedCrouched;
	//}
	//else
	//{
	//	GetCharacterMovement()->MaxWalkSpeed = AimingWalkSpeed;
	//}

	SetIsAiming(true);
	//auto CharacterAnim = GetCharacterAnimInstance();
	//check(nullptr != CharacterAnim);
	//CharacterAnim->SetIsAiming(true);
}

void AZCharacter::AimRelease()
{
	//if (GetCharacterMovement()->IsCrouching())
	//{
	//	GetCharacterMovement()->MaxWalkSpeedCrouched = WalkSpeedCrouched;
	//}
	//else
	//{
	//	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	//}
	SetIsAiming(false);
	//auto CharacterAnim = GetCharacterAnimInstance();
	//check(nullptr != CharacterAnim);
	//CharacterAnim->SetIsAiming(false);
}

void AZCharacter::Reload()
{
	if (!HasAuthority())
	{
		ServerReload();
		return;
	}

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
		MulticastPlayItemMontage(TEXT("Reload"));
		//auto CharacterAnim = GetCharacterAnimInstance();
		//check(nullptr != CharacterAnim);

		//auto Montage = Weapon->FindMontage(TEXT("Reload"));
		//if (Montage)
		//{
		//	CharacterAnim->Montage_Play(Montage);
		//}

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
	ZLOG_S(Error);
	/* Server���� �����ؾ���. */
	if (!HasAuthority())
	{
		ServerSwitchWeapon(NewWeaponIndex);
		return;
	}
	ZLOG_S(Error);
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
		CurrentWeapon->StopFire();

		/*
			Weapon slot�� ���� �޶���.
		*/

		switch (CurrentWeapon->GetWeaponInventoryIndex())
		{
		case EWeaponSlot::Main1:
		{
			/*
				�������� ��� ���� ���
			*/
			auto Gun = Cast<AZGun>(CurrentWeapon);
			check(Gun != nullptr);
			if (Gun->IsReloading())
			{
				Gun->SetIsReloading(false);
			}

			// ���� Weapon�� Secondary socket���� �ű�.
			AttachWeapon(CurrentWeapon, SecondaryWeaponSocketName);
			MulticastAttachWeapon(CurrentWeapon, SecondaryWeaponSocketName);
			break;
		}
		case EWeaponSlot::Main2:
		{
			/*
				�������� ��� ���� ���
			*/
			auto Gun = Cast<AZGun>(CurrentWeapon);
			check(Gun != nullptr);
			if (Gun->IsReloading())
			{
				Gun->SetIsReloading(false);
			}

			// ���� Weapon�� Third socket���� �ű�.
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
		// Gun�̹Ƿ� true ����
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
	//		����ź�� �� ��Ȳ�̸� return
	//	*/
	//	ZLOG_S(Warning);
	//	// Gun�� �ƴϹǷ� false ����
	//	CharacterAnim->SetIsEquipGun(false);
	//	// OnGrenadeThrow�� ���ε�.
	//	auto Grenade = Cast<AZGrenade>(CurrentWeapon);
	//	check(nullptr != Grenade);
	//	ZLOG(Error, TEXT("Bind ThrowGrenade"));
	//	CharacterAnim->OnGrenadeThrow.BindUObject(Grenade, &AZGrenade::ThrowGrenade);

	//	break;
	//}
	}

	// �� Weapon�� Main socket���� �ű�.
	AttachWeapon(CurrentWeapon, SocketName);
	MulticastAttachWeapon(CurrentWeapon, SocketName);

	SetIsSwitchingWeapon(true);

	MulticastPlayMontage(TEXT("EquipRifle"));
	CharacterAnim->PlayCharacterMontage(TEXT("EquipRifle"));

}

void AZCharacter::Slot1()
{
	SwitchWeapon(0);
}

void AZCharacter::Slot2()
{
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

bool AZCharacter::ServerReload_Validate()
{
	return true;
}

void AZCharacter::ServerReload_Implementation()
{
	Reload();
}



bool AZCharacter::ServerSetAiming_Validate(bool bAiming)
{
	return true;
}

void AZCharacter::ServerSetAiming_Implementation(bool bAiming)
{
	SetIsAiming(bAiming);
}

bool AZCharacter::ServerSetSprint_Validate(bool bSprint)
{
	return true;
}

void AZCharacter::ServerSetSprint_Implementation(bool bSprint)
{
	if (bSprint)
	{
		Sprint();
	}
	else
	{
		SprintRelease();
	}
}

bool AZCharacter::ServerSetUserName_Validate(const FString & NewUserName)
{
	return true;
}

void AZCharacter::ServerSetUserName_Implementation(const FString & NewUserName)
{
	auto UserNameWidget = Cast<UZUserNameWidget>(NameComponent->GetUserWidgetObject());
	if (UserNameWidget)
	{

		UserNameWidget->SetUserName(NewUserName);

	}
}

bool AZCharacter::ClientOnDead_Validate()
{
	return true;
}

void AZCharacter::ClientOnDead_Implementation()
{
	auto MyPC = GetController<AZPlayerController>();
	if (MyPC && MyPC->IsLocalPlayerController())
	{
		DisableInput(MyPC);
		//MyPC->GetUserHUD()->DrawEndGameMenuWidget();
	}
}

void AZCharacter::MulticastPlayItemMontage_Implementation(const FString & MontageName)
{
	if (!IsEquipWeapon())
	{
		ZLOG(Error, TEXT("Weapon not exis.."));
		return;
	}

	auto Montage = CurrentWeapon->FindMontage(MontageName);
	if (nullptr == Montage)
	{
		ZLOG(Error, TEXT("Montage not exist.."));
		return;
	}

	auto PlayerAnim = GetCharacterAnimInstance();
	if (!::IsValid(PlayerAnim))
	{
		ZLOG(Error, TEXT("PlayerAnim not valid."));
		return;
	}

	PlayerAnim->Montage_Play(Montage);
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

		if (HasAuthority())
		{
			if (EWeaponCategory::Grenade == CurrentWeapon->GetWeaponCategory())
			{
				ZLOG_S(Warning);
				/* OnGrenadeThrow�� ���ε�. */
				auto Grenade = Cast<AZGrenade>(CurrentWeapon);
				check(Grenade);
				ZLOG(Error, TEXT("Bind ThrowGrenade"));
				CharacterAnim->OnGrenadeThrow.BindUObject(Grenade, &AZGrenade::ThrowGrenade);
			}
		}

		// CurrentWeaponInfo widget�� ���ε�
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

void AZCharacter::OnRep_IsCrouched()
{
	Super::OnRep_IsCrouched();

	if (bIsCrouched)
	{
		if (IsAiming())
		{
			SetCharacterCrouchWalkSpeed(AimingWalkSpeedCrouched);
		}
		else
		{
			SetCharacterCrouchWalkSpeed(WalkSpeedCrouched);
		}
	}
	else
	{
		if (IsAiming())
		{
			SetCharacterWalkSpeed(AimingWalkSpeed);
		}
		else
		{
			SetCharacterWalkSpeed(WalkSpeed);
		}
	}

}

