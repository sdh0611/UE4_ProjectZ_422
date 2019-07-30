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
	void StopCurrentMontage();
	void ResumeCurrentMontage();

public:
	void SetIsSprinting(bool NewState);
	void SetIsDead(bool NewState);

public:
	bool IsSprinting() const;
	bool IsMontagePlaying(const FString& MontageName) const;
	class UAnimMontage* const FindMontageByName(const FString& MontageName) const;
	class UAnimMontage* const GetCurrentPlayMontage() const;

public:
	FOnGrenadeThrow OnGrenadeThrow;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	float CurrentPawnDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	bool bIsFalling;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	bool bIsCrouching;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	bool bIsSprinting;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	bool bIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ZCharacter, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* CurrentPlayMontage;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage, Meta = (AllowPrivateAccess = true))
	TMap<FString, class UAnimMontage*>MontageTable;

};
