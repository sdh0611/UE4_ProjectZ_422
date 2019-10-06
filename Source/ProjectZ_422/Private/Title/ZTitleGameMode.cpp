// Fill out your copyright notice in the Description page of Project Settings.


#include "ZTitleGameMode.h"

void AZTitleGameMode::BeginPlay()
{
	Super::BeginPlay();

	Http = &FHttpModule::Get();

}

void AZTitleGameMode::HttpPost(const FString & URL, const FString & UserID, const FString & UserPW, FHttpRequestCompleteDelegate RequestDelegate)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete() = RequestDelegate;

	FString PostParameters = FString::Printf(TEXT("id=%s&password=%s"), *UserID, *UserPW);

	Request->SetURL(URL);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();

}
