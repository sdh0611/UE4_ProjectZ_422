// Fill out your copyright notice in the Description page of Project Settings.


#include "ZBaseGameMode.h"
#include "ZBasePlayerController.h"
#include "ZCharacter.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
//#include "GameLiftServerSDK.h"
//#include "ConfigCacheIni.h"

AZBaseGameMode::AZBaseGameMode()
{
	//GameLiftSDKModule = 
	//	&FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(TEXT("GameLiftServerSDK"));

	//GameLiftSDKModule->InitSDK();

	//auto OnGameSession = [=](Aws::GameLift::Server::Model::GameSession NewGameSession)
	//{
	//	GameLiftSDKModule->ActivateGameSession();
	//};
	//GameLiftSDKModule->TerminateGameSession();

	//FProcessParameters* Params = new FProcessParameters();
	//Params->OnStartGameSession.BindLambda(OnGameSession);
	//Params->OnTerminate.BindLambda([=]() {
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
	//
}

void AZBaseGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto BasePC = Cast<AZBasePlayerController>(NewPlayer);
	if (BasePC)
	{
		BasePC->ClientReceiveGetUserName();
		++ConnectNumber;
	}

}

void AZBaseGameMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);

	--ConnectNumber;
	//if (ConnectNumber < 1)
	//{
	//	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
	//}
}

void AZBaseGameMode::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	//for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	//{
	//	auto PC = Cast <AZBasePlayerController>(*Iter);
	//	if (PC)
	//	{
	//		PC->ClientDestroySession();
	//	}
	//}
	
	Super::EndPlay(EndPlayReason);
}

void AZBaseGameMode::UpdatePlayersName()
{
	//for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	//{
	//	auto Character = Cast<AZCharacter>(Iter->Get()->GetPawn());
	//	if (::IsValid(Character))
	//	{
	//		Character->MulticastUpdatePlayerName();
	//	}

	//	//auto BasePC = Cast<AZBasePlayerController>(*Iter);
	//	//if (BasePC)
	//	//{
	//	//	BasePC->ClientReceiveSetUserName();
	//	//}
	//}
}

int32 AZBaseGameMode::GetConnectNumber() const
{
	return ConnectNumber;
}
