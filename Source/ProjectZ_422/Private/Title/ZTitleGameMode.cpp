// Fill out your copyright notice in the Description page of Project Settings.


#include "ZTitleGameMode.h"
#include "ZBasePlayerController.h"


void AZTitleGameMode::BeginPlay()
{
	Super::BeginPlay();

	Http = &FHttpModule::Get();

}

void AZTitleGameMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);

	/* DB에 Logout 상태로 변경. */
	//auto MyPC = Cast<AZBasePlayerController>(Exiting);
	//if (MyPC)
	//{
	//	MyPC->ClientRequestLogout();
	//}

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
