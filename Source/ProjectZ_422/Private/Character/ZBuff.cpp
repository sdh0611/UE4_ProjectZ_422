// Fill out your copyright notice in the Description page of Project Settings.


#include "ZBuff.h"
#include "ZCharacterStatusComponent.h"
#include "Engine/World.h"


UZBuff::UZBuff()
{
	
	LifeSpan = 3.f;
	BuffTimer = 0.f;
}

void UZBuff::ApplyBuff(UZCharacterStatusComponent * StatusComponent)
{
	
}

void UZBuff::EndBuff(UZCharacterStatusComponent * StatusComponent)
{

}

void UZBuff::Update(float DeltaTime)
{
	BuffTimer += DeltaTime;
}

bool UZBuff::CheckBuffEnd()
{
	return BuffTimer > LifeSpan;
}
