// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/Character.h"
#include "ZBaseCharacter.h"
#include "ZCharacter.generated.h"

UCLASS()
class PROJECTZ_422_API AZCharacter : public AZBaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, 
	//	class AController* EventInstigator, class AActor* DamageCauser) override;

public:
	FHitResult GetTraceHitFromActorCameraView(float Distance);

public:
	void SetIsAiming(bool NewState);
	void SetIsSwitchingWeapon(bool NewState);
	void SetCurrentWeapon(class AZWeapon* NewWeapon);
	virtual void SetCurrentSpeed(float NewSpeed) override;
	virtual void SetActive(bool bActive) override;

public:
	bool IsEquipWeapon();
	bool IsAiming();
	bool IsSwitchingWeapon();
	float GetCurrentSpeed() const;
	class AZInteractional* GetInteractionalInView();
	class UZCharacterItemStatusComponent* const GetItemStatusComponent() const;
	class AZWeapon* const GetCurrentWeapon();
	class UZPlayerAnimInstance* const GetCharacterAnimInstance();

private:
	void CheckCharacterRotation(float DeltaTime);
	virtual void OnDead() override;

private:
	/*
		키 입력과 관련된 메소드들
	*/
	/*
		Character 기본 움직임들
	*/
	void MoveForward(float NewAxisValue);
	void MoveRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void LookRight(float NewAxisValue);
	void ToggleCrouch();
	void Sprint();
	void SprintRelease();
	virtual void Jump() override;

	/*
		Item 관련 동작들
	*/
	void Interaction();
	void ToggleInventory();
	void Attack();
	void AttackEnd();
	void Aim();
	void AimRelease();
	void Reload();
	void DropWeapon();
	void SwitchWeapon(int32 NewWeaponIndex);
	void Slot1();
	void Slot2();
	void Slot4();

	/*
		디버깅용
	*/
	void AddMoney();
	void DamageSelf();
	void Ragdoll();


public:
	static const FName MainWeaponSocketName;
	static const FName SecondaryWeaponSocketName;
	static const FName ThirdWeaponSocketName;
	static const FName KnifeSocketName;
	static const FName GrenadeWeaponSocketName;

private:	
	UPROPERTY(VisibleAnywhere, Category = State)
	bool bIsAiming;

	UPROPERTY(VisibleAnywhere, Category = State)
	bool bIsSwitchingWeapon;

	UPROPERTY(EditAnywhere, Category = Stat)
	float WalkSpeedCrouched;

	UPROPERTY(EditAnywhere, Category = Stat)
	float AimingWalkSpeed;

	UPROPERTY(EditAnywhere, Category = Stat)
	float AimingWalkSpeedCrouched;

	UPROPERTY()
	class AZInteractional* InteractionActor;

	UPROPERTY()
	class AZPlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere, Category = Character, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	class AZWeapon* CurrentWeapon;

	//UPROPERTY(VisibleAnywhere, Category = Character)
	//class UZCharacterAnimInstance* AnimInstance;

	FRotator Rotate;

private:
	UPROPERTY(VisibleAnywhere, Category = ZCharacter)
	class USpringArmComponent* MainCameraSpringArm;

	UPROPERTY(VisibleAnywhere, Category = ZCharacter)
	class UCameraComponent* MainCamera;

	UPROPERTY(VisibleAnywhere, Category = ZCharacter)
	class UZCharacterItemStatusComponent* ItemStatusComponent;

	//UPROPERTY(VisibleAnywhere, Category = ZCharacter)
	//class UZCharacterStatusComponent* StatusComponent;

};
