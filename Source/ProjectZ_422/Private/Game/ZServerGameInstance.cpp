// Fill out your copyright notice in the Description page of Project Settings.


#include "ZServerGameInstance.h"
#include "ConfigCacheIni.h"
#include "Misc/CommandLine.h"
#include "Engine/Engine.h"

#include "GameLiftServerSDK.h"

UZServerGameInstance::UZServerGameInstance()
{

}

void UZServerGameInstance::Init()
{	
	Super::Init();

	GameLiftSDKModule = 
		&FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(TEXT("GameLiftServerSDK"));

	GameLiftSDKModule->InitSDK();

	auto OnGameSession = [=](Aws::GameLift::Server::Model::GameSession NewGameSession)
	{
		GameLiftSDKModule->ActivateGameSession();
	};
	GameLiftSDKModule->TerminateGameSession();

	FProcessParameters* Params = new FProcessParameters();
	Params->OnStartGameSession.BindLambda(OnGameSession);
	Params->OnTerminate.BindLambda([=]() {
		GameLiftSDKModule->ProcessEnding();
	});

	Params->OnHealthCheck.BindLambda([]() { return true; });
	FString Port = "7777";
	if (FParse::Value(FCommandLine::Get(), TEXT("Port="), Port) == false)
	{
		Port = GConfig->GetStr(TEXT("URL"), TEXT("Port"), GEngineIni);
	}
	Params->port = FCString::Atoi(*Port);

	GameLiftSDKModule->ProcessReady(*Params);

}

void UZServerGameInstance::TerminateSession()
{
	GameLiftSDKModule->TerminateGameSession();

}

