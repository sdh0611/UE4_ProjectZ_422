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
#include "ConfigCacheIni.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystemTypes.h"

#include "GameLiftClientSDK.h"
#include "GameLiftClientObject.h"
#include "GameLiftClientApi.h"

static const FName SERVER_NAME_KEY = FName(TEXT("ServerName"));

UZGameInstance::UZGameInstance()
{
	/* Client 관련 */
	GameLiftAccessKey = TEXT("AKIAUMWLOZXE6L2RTDFJ");
	GameLiftSecretAccessKey = TEXT("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");

}

void UZGameInstance::Init()
{
	Super::Init();
	WebConnector = NewObject<UZWebConnector>(this);
	WebConnector->SetURL(WebURL);

	LoadStaticMesh();
	LoadSkeletalMesh();
	LoadImages();

	//GEngine->GetGameUserSettings()->LoadSettings();
	//GEngine->GetGameUserSettings()->ApplySettings(true);
	//GEngine->GetGameUserSettings()->RequestResolutionChange(1600, 900, EWindowMode::Windowed);

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UZGameInstance::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UZGameInstance::OnPostLoadMap);

	if (!IsDedicatedServerInstance())
	{
		/* Client 관련 */
		GameLiftClientObject = UGameLiftClientObject::CreateGameLiftObject(GameLiftAccessKey, GameLiftSecretAccessKey);
		if (nullptr == GameLiftClientObject)
		{
			ZLOG(Error, TEXT("Failed to create GameLiftClientObject.."));
		}
	}

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

void UZGameInstance::CreateGameSession(const FString& AliasID, int32 MaxPlayer)
{
	if (nullptr == GameLiftClientObject)
	{
		return;
	}
	
	FGameLiftGameSessionConfig MySessionConfig;
	MySessionConfig.SetAliasID(AliasID);
	MySessionConfig.SetMaxPlayers(MaxPlayer);
	

	UGameLiftCreateGameSession* MyGameSessionObject = GameLiftClientObject->CreateGameSession(MySessionConfig);
	if (nullptr == MyGameSessionObject)
	{
		ZLOG_S(Error);
		return;
	}
	MyGameSessionObject->OnCreateGameSessionSuccess.AddDynamic(this, &UZGameInstance::OnGameCreationSuccess);
	MyGameSessionObject->OnCreateGameSessionFailed.AddDynamic(this, &UZGameInstance::OnGameCreationFailed);
	MyGameSessionObject->Activate();

}

void UZGameInstance::DescribeGameSession(const FString & GameSessionID)
{
	if (nullptr == GameLiftClientObject)
	{
		return;
	}

	UGameLiftDescribeGameSession* MyDescribeGameSessionObject = GameLiftClientObject->DescribeGameSession(GameSessionID);
	if (nullptr == MyDescribeGameSessionObject)
	{
		ZLOG_S(Error);
		return;
	}
	MyDescribeGameSessionObject->OnDescribeGameSessionStateSuccess.AddDynamic(this, &UZGameInstance::OnDescribeGameSessionSuccess);
	MyDescribeGameSessionObject->OnDescribeGameSessionStateFailed.AddDynamic(this, &UZGameInstance::OnDescribeGameSessionFailed);
	MyDescribeGameSessionObject->Activate();
}

void UZGameInstance::CreatePlayerSession(const FString & GameSessionID, const FString & UniquePlayerID)
{
	if (nullptr == GameLiftClientObject)
	{
		return;
	}

	UGameLiftCreatePlayerSession* MyCreatePlayerSessionObject = GameLiftClientObject->CreatePlayerSession(GameSessionID, UniquePlayerID);
	if(nullptr == MyCreatePlayerSessionObject)
	{
		ZLOG_S(Error);
		return;
	}
	MyCreatePlayerSessionObject->OnCreatePlayerSessionSuccess.AddDynamic(this, &UZGameInstance::OnPlayerSessionCreateSuccess);
	MyCreatePlayerSessionObject->OnCreatePlayerSessionFailed.AddDynamic(this, &UZGameInstance::OnPlayerSessionCreateFail);
	MyCreatePlayerSessionObject->Activate();
}

void UZGameInstance::OnGameCreationSuccess(const FString & GameSessionID)
{
	DescribeGameSession(GameSessionID);
}

void UZGameInstance::OnGameCreationFailed(const FString & ErrorMessage)
{

}

void UZGameInstance::OnDescribeGameSessionSuccess(const FString & SessionID, EGameLiftGameSessionStatus SessionStatus)
{
	if (SessionStatus == EGameLiftGameSessionStatus::STATUS_Active
		|| SessionStatus == EGameLiftGameSessionStatus::STATUS_Activating)
	{
		CreatePlayerSession(SessionID, WebConnector->GetUserNickname());
	}

}

void UZGameInstance::OnDescribeGameSessionFailed(const FString & ErrorMessage)
{


}

void UZGameInstance::OnPlayerSessionCreateSuccess(const FString & IPAddress, const FString & Port, const FString & PlayerSessionID, const int& PlayerSessionStatus)
{
	const FString TravelURL = IPAddress + ":" + Port;
	auto PC = GetFirstGamePlayer()->GetPlayerController(GetWorld());
	if (PC)
	{
		PC->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
	}
}

void UZGameInstance::OnPlayerSessionCreateFail(const FString & ErrorMessage)
{
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

void UZGameInstance::LoadImages()
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
