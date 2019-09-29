// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "ZItem.h"
#include "ZWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnWeaponFired);

UENUM(BlueprintType)
enum class EWeaponCategory : uint8
{
	Gun,
	Pistol,
	Knife,
	Grenade,
	Invalid
};

/*
	WeaponCategory
*/

static FString GetWeaponCategoryEnumAsString(EWeaponCategory WeaponCategory)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponCategory"), true);
	if (nullptr == EnumPtr)
	{
		return FString(TEXT("Invalid"));
	}

	FString Concated = EnumPtr->GetNameStringByValue((int64)WeaponCategory);
	Concated.RemoveFromStart(TEXT("EWeaponCategory::"));

	return Concated;
}

static EWeaponCategory GetWeaponCategoryFromString(const FString& WeaponCategoryName)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponCategory"), true);
	if (nullptr == EnumPtr)
	{
		return EWeaponCategory::Invalid;
	}

	if (WeaponCategoryName == TEXT("Gun"))
	{
		return EWeaponCategory::Gun;
	}
	else if (WeaponCategoryName == TEXT("Knife"))
	{
		return EWeaponCategory::Knife;
	}
	else if (WeaponCategoryName == TEXT("Grenade"))
	{
		return EWeaponCategory::Grenade;
	}

	return EWeaponCategory::Invalid;
};

USTRUCT(BlueprintType)
struct PROJECTZ_422_API FZWeaponInfo : public FZItemInfo
{
	GENERATED_BODY()
		
public:
	virtual ~FZWeaponInfo() { };

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWeaponCategory WeaponCategory;

	static const int32 TypeID = 1;

	virtual bool IsOfType(int32 NewID) const override { return (NewID == FZWeaponInfo::TypeID) || FZItemInfo::IsOfType(NewID); }
};

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZWeapon : public AZItem
{
	GENERATED_BODY()
	
public:
	AZWeapon();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRemoved() override;
	virtual void OnDropped(int32 Quantity = 1) override;
	virtual void InitItemData(const struct FZItemData* const NewItemData) override;
	virtual void ApplyItemInfo(FZItemInfo NewItemInfo) override;
	virtual void Fire();
	virtual void FireEnd();

public:
	void SetWeaponInventoryIndex(int32 NewIndex);
	void SetIsEquipped(bool NewState);
	void SetWeaponCategory(EWeaponCategory NewWeaponCategory);

public:
	int32 GetWeaponInventoryIndex() const;
	bool IsEquipped() const;
	EWeaponCategory GetWeaponCategory() const;
	virtual class UAnimMontage* const GetFireAnimMontage() const;
	virtual FZItemInfo CreateItemInfo();

protected:
	FHitResult WeaponTrace(float Distance, bool bDrawDebugLine = false);
	virtual void InitItemInfo(FZItemInfo& ItemInfo) override;

protected:
	/*From server to client RPC*/
	UFUNCTION(Client, Reliable, WithValidation)
	void ClientSetSkeletalMesh(class USkeletalMesh* NewMesh);
	bool ClientSetSkeletalMesh_Validate(class USkeletalMesh* NewMesh);
	void ClientSetSkeletalMesh_Implementation(class USkeletalMesh* NewMesh);

public:
	FOnWeaponFired OnWeaponFired;

	/*
		���� Player�� ��� �ִ� ���������� ����
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon, Replicated)
	bool bIsEquipped;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, Replicated)
	int32 WeaponInventoryIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = Weapon, BlueprintReadOnly)
	EWeaponCategory WeaponCategory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	class USoundBase* FireSound;

	UPROPERTY(EditAnywhere)
	bool bToggleDebug;

};
