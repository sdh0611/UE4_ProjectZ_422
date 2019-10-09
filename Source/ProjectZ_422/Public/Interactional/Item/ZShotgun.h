// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Interactional/Item/ZGun.h"
#include "ZShotgun.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZShotgun : public AZGun
{
	GENERATED_BODY()
	
public:
	AZShotgun();

protected:
	virtual void Fire() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ShotNumber;

	/* ��ź ���� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ShotRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DistantConstant = 0.025f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FVector> DirList;
	

};
