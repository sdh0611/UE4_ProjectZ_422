// Fill out your copyright notice in the Description page of Project Settings.


#include "ZTitleGameMode.h"

void AZTitleGameMode::BeginPlay()
{
	Super::BeginPlay();

	Http = &FHttpModule::Get();

}

void AZTitleGameMode::HttpPost(const FString & URL, const FString & UserID, const FString & UserPW, FHttpRequestCompleteDelegate RequestDelegate)
{


}
