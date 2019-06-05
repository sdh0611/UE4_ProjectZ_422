// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/Character.h"
#include "ZCharacter.generated.h"

UCLASS()
class PROJECTZ_422_API AZCharacter : public ACharacter
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

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, 
		class AController* EventInstigator, class AActor* DamageCauser) override;

public:
	FHitResult GetTraceHit(const FVector& TraceStart, const FVector& TraceEnd);
	FHitResult GetTraceHitFromActorCameraView(float Distance);

public:
	void SetIsSprinting(bool NewState);
	void SetIsAiming(bool NewState);
	void SetIsSwitchingWeapon(bool NewState);
	void SetCurrentWeapon(class AZWeapon* NewWeapon);
	void SetCurrentSpeed(float NewSpeed);


public:
	bool IsSprinting();
	bool IsEquipWeapon();
	bool IsAiming();
	bool IsSwitchingWeapon();
	float GetCurrentSpeed() const;
	class AZInteractional* GetInteractionalInView();
	class UZCharacterItemStatusComponent* const GetItemStatusComponent() const;
	class UZCharacterStatusComponent* const GetStatusComponent() const;
	class AZWeapon* const GetCurrentWeapon();


private:
	void CheckCharacterRotation(float DeltaTime);

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

	/*
		디버깅용
	*/
	void AddMoney();
	void DamageSelf();


public:
	static const FName MainWeaponSocketName;
	static const FName SecondaryWeaponSocketName;

private:
	UPROPERTY(VisibleAnywhere, Category = State)
	bool bIsSprinting;
	
	UPROPERTY(VisibleAnywhere, Category = State)
	bool bIsAiming;

	UPROPERTY(VisibleAnywhere, Category = State)
	bool bIsSwitchingWeapon;

	UPROPERTY(EditAnywhere, Category = Stat)
	float WalkSpeed;

	UPROPERTY(EditAnywhere, Category = Stat)
	float WalkSpeedCrouched;

	UPROPERTY(EditAnywhere, Category = Stat)
	float SprintSpeed;

	UPROPERTY(EditAnywhere, Category = Stat)
	float AimingWalkSpeed;

	UPROPERTY(EditAnywhere, Category = Stat)
	float AimingWalkSpeedCrouched;

	UPROPERTY()
	class AZInteractional* InteractionActor;

	UPROPERTY()
	class AZPlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere, Category = Character)
	class AZWeapon* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, Category = Character)
	class UZCharacterAnimInstance* AnimInstance;

	FRotator Rotate;

private:
	UPROPERTY(VisibleAnywhere, Category = ZCharacter)
	class USpringArmComponent* MainCameraSpringArm;

	UPROPERTY(VisibleAnywhere, Category = ZCharacter)
	class UCameraComponent* MainCamera;

	UPROPERTY(VisibleAnywhere, Category = ZCharacter)
	class UZCharacterItemStatusComponent* ItemStatusComponent;

	UPROPERTY(VisibleAnywhere, Category = ZCharacter)
	class UZCharacterStatusComponent* StatusComponent;

};
