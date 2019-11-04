// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGameInstance.h"
#include "ZLobbyGameMode.h"
#include "ZPlayerState.h"
#include "ProjectZLoadingScreen.h"
#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#include "MoviePlayer.h"
#include "ModuleManager.h"
#include "ZBasePlayerController.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"
#include "Blueprint/UserWidget.h"
#include "UI/ZGameViewportClient.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystemTypes.h"


static const FName SERVER_NAME_KEY = FName(TEXT("ServerName"));

UZGameInstance::UZGameInstance()
{
	//UFindSessionsCallbackProxy::FindSessions();
	//UCreateSessionCallbackProxy::CreateSession();
	//UDestroySessionCallbackProxy::DestroySession();
}

void UZGameInstance::Init()
{
	Super::Init();
	WebConnector = NewObject<UZWebConnector>(this);
	WebConnector->SetURL(WebURL);

	LoadStaticMesh();
	LoadSkeletalMesh();
	LoadImage();

	//GEngine->GetGameUserSettings()->LoadSettings();
	//GEngine->GetGameUserSettings()->ApplySettings(true);
	//GEngine->GetGameUserSettings()->RequestResolutionChange(1600, 900, EWindowMode::Windowed);

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UZGameInstance::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UZGameInstance::OnPostLoadMap);

	OnCreateSessionCompleteDelegate.BindUObject(this, &UZGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate.BindUObject(this, &UZGameInstance::OnStartSessionComplete);
	OnFindSessionsCompleteDelegate.BindUObject(this, &UZGameInstance::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate.BindUObject(this, &UZGameInstance::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate.BindUObject(this, &UZGameInstance::OnDestroySessionComplete);


}

void UZGameInstance::Shutdown()
{
	WebConnector->Logout();

	Super::Shutdown();
}

UZWebConnector & UZGameInstance::GetWebConnector()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return *WebConnector;
}

bool UZGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, const FString&  ServerName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
	//GetFirstLocalPlayerController()->GetPlayerState<APlayerState>()->UniqueId.GetUniqueNetId();

	IOnlineSubsystem* const OnlineSubSystem = IOnlineSubsystem::Get(TEXT("Steam"));
	if (OnlineSubSystem)
	{
		IOnlineSessionPtr Session = OnlineSubSystem->GetSessionInterface();
		if (Session.IsValid() && UserId.IsValid())
		{
			SessionSettings = MakeShareable(new FOnlineSessionSettings());
			//SessionSettings->bIsLANMatch = true;
			//SessionSettings->bUsesPresence = true;
			//SessionSettings->NumPublicConnections = MaxNumPlayers;
			////SessionSettings->NumPrivateConnections = 0;
			////SessionSettings->bIsDedicated = false;
			////SessionSettings->bAllowInvites = true;
			////SessionSettings->bAllowJoinInProgress = false;
			//SessionSettings->bShouldAdvertise = true;
			////SessionSettings->bAllowJoinViaPresence = true;
			////SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;
			//SessionSettings->Set(SERVER_NAME_KEY, SessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
			////SessionSettings->Set(SETTING_MAPNAME, FString("StartMenu"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = true;
			SessionSettings->bAllowJoinViaPresence = true;

			SessionSettings->Set(SERVER_NAME_KEY, ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
			OnCreateSessionCompleteDelegateHandle = Session->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			//FNamedOnlineSession* ExistSession = Session->GetNamedSession(*SessionName);
			//if (ExistSession)
			//{
			//	Session->DestroySession(*SessionName);
			//}

			return Session->CreateSession(0, NAME_GameSession, *SessionSettings);
		}

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No OnlineSubsystem found!"));
	}

	/* Session 생성 실패 */
	return false;
	
}

void UZGameInstance::FindSession(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence)
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		
		if (Session.IsValid())
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());

			SessionSearch->bIsLanQuery = true;
			SessionSearch->MaxSearchResults = 100;
			//SessionSearch->PingBucketSize = 50;
			SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

			TSharedRef<FOnlineSessionSearch> SearchSettingRef = SessionSearch.ToSharedRef();

			OnFindSessionsCompleteDelegateHandle = Session->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
			Session->FindSessions(0, SearchSettingRef);
		}
	}
	else
	{
		
		OnFindSessionsComplete(false);
	}

}

bool UZGameInstance::SessionJoin(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult & SearchResult)
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		GetFirstLocalPlayerController()->GetLocalPlayer();
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		if (Session.IsValid())
		{
			OnJoinSessionCompleteDelegateHandle = Session->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			return Session->JoinSession(0, SessionName, SearchResult);
		}

	}

	return false;
}

bool UZGameInstance::SessionJoinByIndex(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, int32 SessionIndex)
{
	if (!SessionSearch->SearchResults.IsValidIndex(SessionIndex))
	{
		return false;
	}

	
	int32 CurrentConnection 
		= SessionSearch->SearchResults[SessionIndex].Session.SessionSettings.NumPublicConnections 
		- SessionSearch->SearchResults[SessionIndex].Session.NumOpenPublicConnections;
	if (CurrentConnection < 1)
	{
		return false;
	}

	return SessionJoin(UserId, SessionName, SessionSearch->SearchResults[SessionIndex]);
}

bool UZGameInstance::DestroySession()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		if (Session.IsValid())
		{
			OnDestroySessionCompleteDelegateHandle = Session->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
			return Session->DestroySession(NAME_GameSession);
		}
	}

	return false;
}

void UZGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnCreateSession : %s, %d"), *SessionName.ToString(), bWasSuccessful));

	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		if (Session.IsValid())
		{
			Session->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				OnStartSessionCompleteDelegateHandle = Session->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
				Session->StartSession(SessionName);
			}
		}
	}

}

void UZGameInstance::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnStartSession : %s, %d"), *SessionName.ToString(), bWasSuccessful));
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		if (Session.IsValid())
		{
			Session->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}

	}

	if (bWasSuccessful)
	{
		GetWorld()->ServerTravel(TEXT("Lobby?listen"), true);

		//auto LobbyGameMode = GetWorld()->GetAuthGameMode<AZLobbyGameMode>();
		//if (LobbyGameMode)
		//{
		//	LobbyGameMode->StartGame();
		//}
	}




}

void UZGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnFindSession : %d"), bWasSuccessful));

	OnFindSessionsEnd.Execute();

	//if (!bWasSuccessful)
	//{
	//	return;
	//}

	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		if (Session.IsValid())
		{
			Session->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnFindSession : %d"), SessionSearch->SearchResults.Num()));

			if (SessionSearch->SearchResults.Num() > 0)
			{
				TArray<FZSessionInfo> SessionsInfo;
				FZSessionInfo SessionInfo;
				for (int i = 0; i < SessionSearch->SearchResults.Num(); ++i)
				{
					/* Session 리스트 갱신 */
					SessionInfo.SessionIndex = i;
					SessionSearch->SearchResults[i].Session.SessionSettings.Get(SERVER_NAME_KEY, SessionInfo.ServerName);
					SessionInfo.HostName = GetWebConnector().GetUserNickname();
					SessionInfo.MaxConnection = SessionSearch->SearchResults[i].Session.SessionSettings.NumPublicConnections;
					SessionInfo.CurrentConnection = SessionInfo.MaxConnection - SessionSearch->SearchResults[i].Session.NumOpenPublicConnections;

					SessionsInfo.Add(SessionInfo);
				}

				if (OnFindSessionsSuccess.IsBound())
				{
					OnFindSessionsSuccess.Execute(SessionsInfo);
				}
			}

		}
	}


}

void UZGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnJoinSession : %s, %d"), *SessionName.ToString(), static_cast<int32>(Result)));
	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("JoinSession Fail."));
		return;
	}

	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		if (Session.IsValid())
		{
			Session->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
			if (nullptr == Session->GetNamedSession(SessionName))
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("JoinSession Fail."));
			}

			/* Debug code. */
			FNamedOnlineSession* MySession = Session->GetNamedSession(SessionName);
			if (NULL == MySession)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("MySession Null."));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("MySession Exist."));
				if (false == MySession->SessionInfo.IsValid())
				{
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Invalid SessionInfo."));

				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Valid SessionInfo."));
				}
			}


			APlayerController* const PC = GetFirstLocalPlayerController();
			FString TravelURL;
			if(!Session->GetResolvedConnectString(SessionName, TravelURL)) 
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Resolve connection fail. : %s"), *SessionName.ToString()));
			}

			if (PC && Session->GetResolvedConnectString(SessionName, TravelURL))
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnJoinSession : %s"), *TravelURL));
				PC->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
			}

		}

	}

}

void UZGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnDestroySession : %s, %d"), *SessionName.ToString(), bWasSuccessful));
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		if (Session.IsValid())
		{
			Session->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
		}
	}
}


void UZGameInstance::ShowLoadingScreen()
{
	if (LoadingWidget)
	{
		LoadingWidget->AddToViewport();
	}
	else
	{
		if (LoadingWidgetClass)
		{
			LoadingWidget = CreateWidget<UUserWidget>(this, LoadingWidgetClass);
			if (LoadingWidget)
			{
				LoadingWidget->AddToViewport();
			}
		}
	}
}

UStaticMesh * const UZGameInstance::GetStaticMesh(const FString & Name)
{
	if (StaticMeshTable.Contains(Name))
	{
		return StaticMeshTable[Name];
	}

	return nullptr;
}

USkeletalMesh * const UZGameInstance::GetSkeletalMesh(const FString & Name)
{
	if (SkeletalMeshTable.Contains(Name))
	{
		return SkeletalMeshTable[Name];
	}

	return nullptr;
}

UTexture2D * const UZGameInstance::GetItemImage(const FString & Name)
{
	if (ItemImageTable.Contains(Name))
	{
		return ItemImageTable[Name];
	}

	return nullptr;
}


const FZItemData * const UZGameInstance::GetItemDataByName(const FString & ItemName)
{
	auto Names = ItemDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = ItemDataTable->FindRow<FZItemData>(Name, TEXT(""));
		if (Data->ItemName == ItemName)
		{
			return Data;
		}
	}


	return nullptr;
}

const FZWeaponData * const UZGameInstance::GetWeaponDataByRowIndex(int32 RowIndex)
{
	return WeaponDataTable->FindRow<FZWeaponData>(*FString::FromInt(RowIndex), TEXT(""));
}

const FZWeaponData * const UZGameInstance::GetWeaponDataByName(const FString & WeaponName)
{
	auto Names = WeaponDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = WeaponDataTable->FindRow<FZWeaponData>(Name, TEXT(""));
		if (Data->ItemName == WeaponName)
		{
			return Data;
		}
	}

	return nullptr;
}

const FZGunData * const UZGameInstance::GetGunDataByRowIndex(int32 RowIndex)
{
	return GunDataTable->FindRow<FZGunData>(*FString::FromInt(RowIndex), TEXT(""));
}

const FZGunData * const UZGameInstance::GetGunDataByName(const FString & GunName)
{
	auto Names = GunDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = GunDataTable->FindRow<FZGunData>(Name, TEXT(""));
		if (Data->ItemName == GunName)
		{
			return Data;
		}
	}

	return nullptr;
}

const FZGrenadeData * const UZGameInstance::GetGrenadeDataByRowIndex(int32 RowIndex)
{
	return GrenadeDataTable->FindRow<FZGrenadeData>(*FString::FromInt(RowIndex), TEXT(""));
}

const FZGrenadeData * const UZGameInstance::GetGrenadeDataByName(const FString & GrenadeName)
{
	auto Names = GrenadeDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = GrenadeDataTable->FindRow<FZGrenadeData>(Name, TEXT(""));
		if (Data->ItemName == GrenadeName)
		{
			return Data;
		}
	}

	return nullptr;
}

const FZRecoveryData * const UZGameInstance::GetRecoveryDataByRowIndex(int32 RowIndex)
{
	return RecoveryDataTable->FindRow<FZRecoveryData>(*FString::FromInt(RowIndex), TEXT(""));
}

const FZRecoveryData * const UZGameInstance::GetRecoveryDataByName(const FString & RecoveryName)
{
	auto Names = RecoveryDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = RecoveryDataTable->FindRow<FZRecoveryData>(Name, TEXT(""));
		if (Data->ItemName == RecoveryName)
		{
			return Data;
		}
	}

	return nullptr;
}

const FZDopingData * const UZGameInstance::GetDopingDataByRowIndex(int32 RowIndex)
{
	return DopingDataTable->FindRow<FZDopingData>(*FString::FromInt(RowIndex), TEXT(""));
}

const FZDopingData * const UZGameInstance::GetDopingDataByName(const FString & DopingName)
{
	auto Names = DopingDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = DopingDataTable->FindRow<FZDopingData>(Name, TEXT(""));
		if (Data->ItemName == DopingName)
		{
			return Data;
		}
	}

	return nullptr;
}

const FZItemData * const UZGameInstance::GetAmmoDataByRowIndex(int32 RowIndex)
{
	return AmmoDataTable->FindRow<FZItemData>(*FString::FromInt(RowIndex), TEXT(""));
}

const FZItemData * const UZGameInstance::GetAmmoDataByName(const FString & AmmoName)
{
	auto Names = AmmoDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = AmmoDataTable->FindRow<FZItemData>(Name, TEXT(""));
		if (Data->ItemName == AmmoName)
		{
			return Data;
		}
	}

	return nullptr;
}

UDataTable * const UZGameInstance::GetShopItemDataTable() const
{
	return ShopItemDataTable;
}

const FZShopItemData * const UZGameInstance::GetShopItemDataByName(const FString & ShopItemName)
{
	auto Names = ShopItemDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = ShopItemDataTable->FindRow<FZShopItemData>(Name, TEXT(""));
		if (Data->ItemName == ShopItemName)
		{
			return Data;
		}
	}

	return nullptr;
}

void UZGameInstance::LoadStaticMesh()
{
	if (nullptr == StaticMeshDataTable)
	{
		ZLOG(Error, TEXT("StaticMeshDataTable not exist.."));
		return;
	}


	TArray<FName> Names = StaticMeshDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = StaticMeshDataTable->FindRow<FZStaticMeshData>(Name, TEXT(""));

		if (!Data->StaticMeshPath.IsNull())
		{
			StaticMeshTable.Add(Data->Name, AssetLoader.LoadSynchronous<UStaticMesh>(Data->StaticMeshPath));
		}
	}
}

void UZGameInstance::LoadSkeletalMesh()
{
	if (nullptr == SkeletalMeshDataTable)
	{
		ZLOG(Error, TEXT("SkeletalMeshDataTable not exist.."));
		return;
	}


	TArray<FName> Names = SkeletalMeshDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = SkeletalMeshDataTable->FindRow<FZSkeletalMeshData>(Name, TEXT(""));
		if (!Data->SkeletalMeshPath.IsNull())
		{
			SkeletalMeshTable.Add(Data->Name, AssetLoader.LoadSynchronous<USkeletalMesh>(Data->SkeletalMeshPath));
		}
	}
}

void UZGameInstance::LoadImage()
{
	if (nullptr == ItemImageDataTable)
	{
		ZLOG(Error, TEXT("ItemImageTable not exist.."));
		return;
	}


	TArray<FName> Names = ItemImageDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = ItemImageDataTable->FindRow<FZImageData>(Name, TEXT(""));

		if (!Data->ImagePath.IsNull())
		{
			ItemImageTable.Add(Data->Name, AssetLoader.LoadSynchronous(Data->ImagePath));
		}

	}


}

void UZGameInstance::OnPreLoadMap(const FString & MapName)
{
	ZLOG_S(Error);
	//IProjectZLoadingScreenModule* const LoadingScreenModule = FModuleManager::LoadModulePtr<IProjectZLoadingScreenModule>(TEXT("ProjectZLoadingScreen"));;
	//if (LoadingScreenModule)
	//{
	//	LoadingScreenModule->StartInGameLoadingScreen();
	//}

	//auto MyGameViewport = Cast<UZGameViewportClient>(GetGameViewportClient());
	//if (MyGameViewport)
	//{
	//	MyGameViewport->ShowLoadingScreen();
	//}

}

void UZGameInstance::OnPostLoadMap(UWorld * NewWorld)
{
	ZLOG_S(Error);

	//auto MyGameViewport = Cast<UZGameViewportClient>(GetGameViewportClient());
	//if (MyGameViewport)
	//{
	//	MyGameViewport->HideLoadingScreen();
	//}

}
