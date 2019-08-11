// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Animation/AnimInstance.h"
#include "ZCharacterAnimInstance.h"
#include "ZPlayerAnimInstance.generated.h"

DECLARE_DELEGATE(FOnGrenadeThrow);

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
	void PlayFireRifleMontage();
	void PlayThrowGrenadeMontage();
	void BindFireMontage(class AZWeapon* NewWeapon);
	void PlayFireMontage(class AZWeapon* NewWeapon);
	void PlayHitMontage();

public:
	void SetIsEquipGun(bool NewState);
	void SetIsAiming(bool NewState);

public:
	bool IsEquipGun() const;
	bool IsAiming() const;

private:
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

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	float AimYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	float AimPitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	bool bIsEquipGun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	bool bIsAiming;

};
