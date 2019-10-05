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
	void PlayCharacterMontage(const FString& MontageName);
	void StopCurrentMontage();
	void ResumeCurrentMontage();

public:
	void SetIsSprinting(bool NewState);

public:
	bool IsSprinting() const;
	bool IsMontagePlaying(const FString& MontageName) const;
	class UAnimMontage* const FindMontageByName(const FString& MontageName) const;
	class UAnimMontage* const GetCurrentPlayMontage() const;

public:
	UFUNCTION()
	void AnimNotify_DamagedEndCheck();

public:
	FOnGrenadeThrow OnGrenadeThrow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ZCharacter )
	bool bIsSprinting;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ZCharacter)
	bool bIsDamaged;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ZCharacter )
	float CurrentPawnSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ZCharacter )
	float CurrentPawnDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ZCharacter )
	bool bIsFalling;


	UPROPERTY()
	class UAnimMontage* CurrentPlayMontage;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage )
	TMap<FString, class UAnimMontage*>MontageTable;

};
