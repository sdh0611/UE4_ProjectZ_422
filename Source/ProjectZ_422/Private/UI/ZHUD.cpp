// Fill out your copyright notice in the Description page of Project Settings.


#include "ZHUD.h"
#include "ZPlayerController.h"
#include "ZUserHUD.h"
#include "ConstructorHelpers.h"


AZHUD::AZHUD()
{	
	PlayerController = nullptr;
}

void AZHUD::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<AZPlayerController>(GetOwningPlayerController());

}
