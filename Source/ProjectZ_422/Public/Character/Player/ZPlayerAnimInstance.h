// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Animation/AnimInstance.h"
#include "ZCharacterAnimInstance.h"
#include "ZPlayerAnimInstance.generated.h"

DECLARE_DELEGATE(FOnGrenadeThrow);
DECLARE_DELEGATE(FOnKnifeAttack);

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZPlayerAnimInstance : public UZCharacterAnimInstance
{
	GENERATED_BODY()
	
public:
	UZPlayerAnimInstance();

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void BindFireMontage(class AZWeapon* NewWeapon);
	void PlayHitMontage();

public:
	void SetIsEquipGun(bool NewState);
	void SetIsAiming(bool NewState);

public:
	bool IsEquipGun() const;
	bool IsAiming() const;

private:
	void PlayFireGunMontage();
	void PlayThrowGrenadeMontage();
	void PlayAttackKnifeMontage();

	UFUNCTION()
	void AnimNotify_ReloadCheck();

	UFUNCTION()
	void AnimNotify_WeaponSwitchCheck();

	UFUNCTION()
	void AnimNotify_SwitchEndCheck();

	UFUNCTION()
	void AnimNotify_ThrowCheck();

public:
	FOnGrenadeThrow OnGrenadeThrow;

	FOnKnifeAttack OnKnifeAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	bool bIsEquipGun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	bool bIsAiming;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	float AimYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	float AimPitch;


};
