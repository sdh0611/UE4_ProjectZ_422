// Fill out your copyright notice in the Description page of Project Settings.


#include "ZInteractional.h"

// Sets default values
AZInteractional::AZInteractional()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AZInteractional::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZInteractional::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AZInteractional::OnInteraction(AZCharacter * NewCharacter)
{
	// Test codes
	ZLOG(Warning, TEXT("OnInteraction!"));

	Destroy();
}

void AZInteractional::OnFocus()
{
	ZLOG(Warning, TEXT("Focus."));
}

void AZInteractional::OnFocusEnd()
{
	ZLOG(Warning, TEXT("FocusEnd."));
}

