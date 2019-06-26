// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "UObject/NoExportTypes.h"
#include "time.h"
#include "ZBuff.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZBuff : public UObject
{
	GENERATED_BODY()
	
public:
	UZBuff();

public:
	virtual void ApplyBuff(class UZCharacterStatusComponent* StatusComponent);
	virtual void EndBuff(class UZCharacterStatusComponent* StatusComponent);
	void Update(float DeltaTime);
	bool CheckBuffEnd();

private:
	float LifeSpan;
	float BuffTimer;


};
