// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Animation/AnimInstance.h"
#include "ZCharacterAnimInstance.generated.h"

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
	void SetIsEquipWeapon(bool NewState);
	void SetIsAiming(bool NewState);

public:
	bool IsEquipWeapon() const;
	bool IsAiming() const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	float CurrentPawnDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	bool bIsFalling;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	bool bIsCrouching;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	bool bIsEquipWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	bool bIsAiming;


};
