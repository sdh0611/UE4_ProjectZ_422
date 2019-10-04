// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Zombie/ZZombieAnimInstance.h"
#include "ZTankerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZTankerAnimInstance : public UZZombieAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UFUNCTION()
	void AnimNotify_ScreamStart();

	UFUNCTION()
	void AnimNotify_ScreamEnd();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsScreaming;
	
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//bool bIsRushing;


};
