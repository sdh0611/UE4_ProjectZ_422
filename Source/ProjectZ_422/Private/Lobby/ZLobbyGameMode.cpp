// Fill out your copyright notice in the Description page of Project Settings.


#include "ZLobbyGameMode.h"
#include "ZLobbyPlayerController.h"
#include "ZLobbyGameState.h"
#include "ZPlayerState.h"
#include "ZServerGameInstance.h"
#include "..\..\Public\Lobby\ZLobbyGameMode.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Json.h"
#include "GameLiftServerSDK.h"
#include "ConfigCacheIni.h"


AZLobbyGameMode::AZLobbyGameMode()
{
	//FGameLiftServerSDKModule* GameLiftSDKModule =
	//	&FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(TEXT("GameLiftServerSDK"));

	//GameLiftSDKModule->InitSDK();

	//auto OnGameSession = [=](Aws::GameLift::Server::Model::GameSession NewGameSession)
	//{
	//	ZLOG(Error, TEXT("Start game session."));
	//	GameLiftSDKModule->ActivateGameSession();
	//};

	//FProcessParameters* Params = new FProcessParameters();
	//Params->OnStartGameSession.BindLambda(OnGameSession);
	//Params->OnTerminate.BindLambda([=]() {
	//	ZLOG(Error, TEXT("Terminate game session."));
	//	GameLiftSDKModule->ProcessEnding();
	//});

	//Params->OnHealthCheck.BindLambda([]() { return true; });
	//FString Port = "7777";
	//if (FParse::Value(FCommandLine::Get(), TEXT("Port="), Port) == false)
	//{
	//	Port = GConfig->GetStr(TEXT("URL"), TEXT("Port"), GEngineIni);
	//}
	//Params->port = FCString::Atoi(*Port);

	//GameLiftSDKModule->ProcessReady(*Params);

}

void AZLobbyGameMode::InitGame(const FString & MapName, const FString & Options, FString & ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Port : %d"), GetWorld()->URL.Port));
	ZLOG(Error, TEXT("Port : %d"), GetWorld()->URL.Port);

	auto GameLiftSDKModule =
		&FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(TEXT("GameLiftServerSDK"));

	auto OnGameSession = [=](Aws::GameLift::Server::Model::GameSession NewGameSession)
	{
		ZLOG(Error, TEXT("onStartGameSession."));
		GameLiftSDKModule->ActivateGameSession();
	};

	FProcessParameters* Params = new FProcessParameters();
	Params->OnStartGameSession.BindLambda(OnGameSession);
	Params->OnTerminate.BindLambda([=]() {
		ZLOG(Error, TEXT("Terminate sessions."));
		GameLiftSDKModule->ProcessEnding();
	});

	Params->OnHealthCheck.BindLambda([&]() { return true; });

	Params->port = GetWorld()->URL.Port;

	TArray<FString> logfiles;
	logfiles.Add(TEXT("ZLogFile.txt"));
	Params->logParameters = logfiles;

	GameLiftSDKModule->ProcessReady(*Params);

}

void AZLobbyGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ZLOG_S(Error);
	auto PC = Cast<AZLobbyPlayerController>(NewPlayer);
	if (PC)
	{
		PC->ClientReceiveAcceptPlayerSession();
		ZLOG_S(Error);
		if (ConnectNumber == 1)
		{
			ZLOG_S(Error);
			PC->bIsHost = true;
		}
	}

	auto MyGameState = GetGameState<AZLobbyGameState>();
	if (MyGameState)
	{
		MyGameState->SetConnectNumber(ConnectNumber);
	}

}

void AZLobbyGameMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);

	if (ConnectNumber > 0)
	{
		if (!bIsStartGame)
		{
			auto PC = Cast<AZLobbyPlayerController>(Exiting);
			if (PC)
			{
				PC->ClientReceiveRemovePlayerSession();
			}

			auto NextHost = Cast<AZLobbyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 1));
			if (NextHost)
			{
				NextHost->bIsHost = true;
				NextHost->ClientActiveStartButton();
			}

		}

	}
	else
	{
		if (!bIsStartGame)
		{
			ZLOG(Error, TEXT("Non start game."));
			auto MyGameInstance = GetGameInstance<UZServerGameInstance>();
			if (MyGameInstance)
			{
				MyGameInstance->TerminateSession();
				MyGameInstance->ProcessEnd();
				
				ZLOG(Error, TEXT("Quit game."));
				GIsRequestingExit = true;
			}
			//UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
		}
		else
		{
			ZLOG(Error, TEXT("Start game."));
		}

	}

	auto MyGameState = GetGameState<AZLobbyGameState>();
	if (MyGameState)
	{
		MyGameState->SetConnectNumber(ConnectNumber);
	}

	auto MyPS = Exiting->GetPlayerState<AZPlayerState>();
	if (nullptr == MyPS)
	{
		ZLOG(Error, TEXT("Invalid player state.."));
		return;
	}

	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		auto LobbyPC = Cast<AZLobbyPlayerController>(*Iter);
		if (LobbyPC)
		{
			LobbyPC->ClientUpdateJoinPlayer(MyPS->GetPlayerName(), true);
		}
	}



}

void AZLobbyGameMode::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	//auto MyGameInstance = GetGameInstance<UZGameInstance>();
	//if (MyGameInstance)
	//{
	//	ZLOG(Error, TEXT("Host : %s"), *MyGameInstance->GetWebConnector().GetIP());

	//	FString URL = *MyGameInstance->GetWebConnector().GetWebURL();
	//	URL.Append(TEXT("/delete_game"));

	//	FString PostParam = FString::Printf(TEXT("ip=%s"), *MyGameInstance->GetWebConnector().GetIP());

	//	MyGameInstance->GetWebConnector().HttpPost(URL, PostParam);

	//}

	Super::EndPlay(EndPlayReason);
}

void AZLobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	ZLOG(Error, TEXT("Port : %d"), GetWorld()->URL.Port);
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Port : %d"), GetWorld()->URL.Port));

	ZLOG_S(Error);
}

void AZLobbyGameMode::StartGame()
{
	ZLOG_S(Error);

	bIsStartGame = true;

	if (!GetWorld()->ServerTravel(TEXT("Stage1"), true))
	{
		ZLOG(Error, TEXT("Server travel fail."));
		return;
	}


	//for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	//{
	//	auto PC = Cast < AZBasePlayerController>(*Iter);
	//	if (PC)
	//	{
	//		PC->ClientRemoveAllWidget();
	//	}
	//}

}
