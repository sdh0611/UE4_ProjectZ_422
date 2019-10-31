// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "ZItem.h"

#include "Http.h"
#include "IHttpRequest.h"
#include "IHttpResponse.h"
#include "ZWebConnector.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"

#include "ZGameInstance.generated.h"

DECLARE_DELEGATE_OneParam(FOnFindSessionsSuccess, const TArray<FZSessionInfo>&);
DECLARE_DELEGATE(FOnFindSessionsEnd);


USTRUCT(BlueprintType)
struct PROJECTZ_422_API FZSessionInfo
{
	GENERATED_BODY()

public:
	int32 SessionIndex = -1;

	FString ServerName;

	FString HostName;

	int32 MaxConnection = 4;

	int32 CurrentConnection = 0;
};

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UZGameInstance();

public:
	virtual void Init() override;
	virtual void Shutdown() override;

public:
	/* Login server ��� ����. */
	UZWebConnector& GetWebConnector();

public:
	/* Session ���� */
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, const FString& ServerName, bool bIsLAN,
		bool bIsPresence, int32 MaxNumPlayers);
	void FindSession(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);
	bool SessionJoin(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);
	bool SessionJoinByIndex(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, int32 SessionIndex);

	UFUNCTION(BlueprintCallable)
	bool DestroySession();

private:
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

public:
	FOnFindSessionsSuccess OnFindSessionsSuccess;
	FOnFindSessionsEnd OnFindSessionsEnd;

private:
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	/* Session �ɼ� ���ÿ�. */
	TSharedPtr<class FOnlineSessionSettings> SessionSettings;
	/* Session Search ����� �����ϱ� ���� ����. */
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	IOnlineSessionPtr SessionInterface;

public:
	/*
		MeshTable ����
	*/
	class UStaticMesh* const GetStaticMesh(const FString& Name);
	class USkeletalMesh* const GetSkeletalMesh(const FString& Name);
	class UTexture2D* const GetItemImage(const FString& Name);

	/*
		��ü ItemData �˻�
	*/
	const FZItemData* const GetItemDataByName(const FString& ItemName);

	/*
		WeaponTable ����
	*/
	const FZWeaponData* const GetWeaponDataByRowIndex(int32 RowIndex);
	const FZWeaponData* const GetWeaponDataByName(const FString& WeaponName);

	const FZGunData* const GetGunDataByRowIndex(int32 RowIndex);
	const FZGunData* const GetGunDataByName(const FString& GunName);

	const FZGrenadeData* const GetGrenadeDataByRowIndex(int32 RowIndex);
	const FZGrenadeData* const GetGrenadeDataByName(const FString& GrenadeName);

	/*
		RecoveryTable ����
	*/
	const FZRecoveryData* const GetRecoveryDataByRowIndex(int32 RowIndex);
	const FZRecoveryData* const GetRecoveryDataByName(const FString& RecoveryName);

	/*
		DopingTable ����
	*/
	const FZDopingData* const GetDopingDataByRowIndex(int32 RowIndex);
	const FZDopingData* const GetDopingDataByName(const FString& DopingName);

	/*
		AmmoTable ����
	*/
	const FZItemData* const GetAmmoDataByRowIndex(int32 RowIndex);
	const FZItemData* const GetAmmoDataByName(const FString& AmmoName);

	/*
		ShopTable ����
	*/
	class UDataTable* const GetShopItemDataTable() const;
	const FZShopItemData* const GetShopItemDataByName(const FString& ShopItemName);

private:
	/* �ּ� �ε� */
	void LoadStaticMesh();
	void LoadSkeletalMesh();
	void LoadImage();

	/* �� �ε� */
	void OnPreLoadMap(const FString& MapName);
	void OnPostLoadMap(UWorld* NewWorld);


public:
	FStreamableManager AssetLoader;

	UPROPERTY(BlueprintReadOnly)
	UZWebConnector* WebConnector;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString WebURL = TEXT("25.43.176.172:8000");

protected:
	UPROPERTY(EditAnywhere, Category = MeshDataTable)
	class UDataTable* StaticMeshDataTable;

	UPROPERTY(EditAnywhere, Category = MeshDataTable)
	class UDataTable* SkeletalMeshDataTable;

	UPROPERTY(EditAnywhere, Category = ImageDataTable)
	class UDataTable* ItemImageDataTable;

	UPROPERTY(EditAnywhere, Category = WeaponDataTable)
	class UDataTable* ItemDataTable;

	UPROPERTY(EditAnywhere, Category = WeaponDataTable)
	class UDataTable* WeaponDataTable;

	UPROPERTY(EditAnywhere, Category = WeaponDataTable)
	class UDataTable* GunDataTable;

	UPROPERTY(EditAnywhere, Category = WeaponDataTable)
	class UDataTable* GrenadeDataTable;

	UPROPERTY(EditAnywhere, Category = RecoveryDataTable)
	class UDataTable* RecoveryDataTable;

	UPROPERTY(EditAnywhere, Category = DopingDataTable)
	class UDataTable* DopingDataTable;

	UPROPERTY(EditAnywhere, Category = AmmoDataTable)
	class UDataTable* AmmoDataTable;

	UPROPERTY(EditAnywhere, Category = ShopDataTable)
	class UDataTable* ShopItemDataTable;


private:
	UPROPERTY(VisibleAnywhere, Category = MeshTable)
	TMap<FString, class UStaticMesh*> StaticMeshTable;

	UPROPERTY(VisibleAnywhere, Category = MeshTable)
	TMap<FString, class USkeletalMesh*> SkeletalMeshTable;

	UPROPERTY(VisibleAnywhere, Category = ImageTable)
	TMap<FString, class UTexture2D*> ItemImageTable;


};
