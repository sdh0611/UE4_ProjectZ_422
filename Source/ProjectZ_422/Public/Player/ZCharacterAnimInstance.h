// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Animation/AnimInstance.h"
#include "ZCharacterAnimInstance.generated.h"

DECLARE_DELEGATE(FOnGrenadeThrow);

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UZCharacterAnimInstance();
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void PlayMontage(const FString& MontageName);
	void PlayFireRifleMontage();
	void PlayThrowGrenadeMontage();
	void BindFireMontage(class AZWeapon* NewWeapon);

public:
	void SetIsEquipGun(bool NewState);
	void SetIsAiming(bool NewState);
	void SetIsSprinting(bool NewState);

public:
	bool IsEquipGun() const;
	bool IsAiming() const;
	bool IsSprinting() const;

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
	float CurrentPawnSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	float CurrentPawnDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	float AimYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	float AimPitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	bool bIsFalling;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	bool bIsCrouching;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	bool bIsEquipGun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	bool bIsAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	bool bIsSprinting;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	class AZCharacter* OwnerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* CurrentPlayMontage;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage, Meta = (AllowPrivateAccess = true))
	TMap<FString, class UAnimMontage*>MontageTable;

};
