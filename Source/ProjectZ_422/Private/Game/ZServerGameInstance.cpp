// Fill out your copyright notice in the Description page of Project Settings.


#include "ZServerGameInstance.h"
#include "ConfigCacheIni.h"
#include "Misc/CommandLine.h"
#include "Engine/Engine.h"

#include "GameLiftServerSDK.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"

UZServerGameInstance::UZServerGameInstance()
{
	GameLiftSDKModule =
		&FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(TEXT("GameLiftServerSDK"));

	GameLiftSDKModule->InitSDK();

	//auto OnGameSession = [=](Aws::GameLift::Server::Model::GameSession NewGameSession)
	//{
	//	ZLOG(Error, TEXT("onStartGameSession."));
	//	GameLiftSDKModule->ActivateGameSession();
	//};

	//FProcessParameters* Params = new FProcessParameters();
	//Params->OnStartGameSession.BindLambda(OnGameSession);
	//Params->OnTerminate.BindLambda([=]() {
	//	ZLOG(Error, TEXT("Terminate sessions."));
	//	GameLiftSDKModule->ProcessEnding();
	//});

	//Params->OnHealthCheck.BindLambda([&]() { return bIsHealthyProcess; });

	//FString Port = "7777";
	//if (FParse::Value(FCommandLine::Get(), TEXT("Port="), Port) == false)
	//{
	//	Port = GConfig->GetStr(TEXT("URL"), TEXT("Port"), GEngineIni);
	//}
	//Params->port = FCString::Atoi(*Port);

	//TArray<FString> logfiles;
	//logfiles.Add(TEXT("ZLogFile.txt"));
	//Params->logParameters = logfiles;
	//
	//GameLiftSDKModule->ProcessReady(*Params);
}

void UZServerGameInstance::Init()
{	
	Super::Init();
	ZLOG(Error, TEXT("%d"), GetWorld()->URL.Port);
	
}

void UZServerGameInstance::Shutdown()
{
	TerminateSession();

	Super::Shutdown();
}

void UZServerGameInstance::TerminateSession()
{
	ZLOG(Error, TEXT("Terminate game session."));
	if (GameLiftSDKModule)
	{
		GameLiftSDKModule->TerminateGameSession();
		//UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
	}

}

void UZServerGameInstance::ProcessEnd()
{
	if (GameLiftSDKModule)
	{
		ZLOG(Error, TEXT("Process end."));
		GameLiftSDKModule->ProcessEnding();
	}

}

void UZServerGameInstance::RemovePlayerSession(const FString & PlayerSessionID)
{
	ZLOG(Error, TEXT("Remove player session."));
	if (GameLiftSDKModule)
	{
		GameLiftSDKModule->RemovePlayerSession(PlayerSessionID);
	}

}

void UZServerGameInstance::AcceptPlayerSession(const FString & PlayerSessionID)
{
	if (GameLiftSDKModule)
	{
		if (!GameLiftSDKModule->AcceptPlayerSession(PlayerSessionID).IsSuccess())
		{
			ZLOG(Error, TEXT("Failed to accept player session."));
		}
	}
}

