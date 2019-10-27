// Fill out your copyright notice in the Description page of Project Settings.


#include "ZTitleGameMode.h"
#include "ZBasePlayerController.h"
#include "Engine/World.h"

void AZTitleGameMode::ToLobby(const FString & URL)
{
	GetWorld()->ServerTravel(URL);

}

void AZTitleGameMode::BeginPlay()
{
	Super::BeginPlay();

}

