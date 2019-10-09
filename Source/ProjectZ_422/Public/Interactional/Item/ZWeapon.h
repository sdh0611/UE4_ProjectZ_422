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
	virtual void OnPicked() override;
	virtual void WhenDropped();

	virtual void InitItemData(const struct FZItemData* const NewItemData) override;

	virtual void ClearDelegates() override;
	virtual class UAnimMontage* const GetFireAnimMontage() const;
	virtual void RepItemOwner() override;

public:
	void StartFire();
	void StopFire();

public:
	void SetWeaponInventoryIndex(int32 NewIndex);
	void SetIsEquipped(bool NewState);
	void SetWeaponCategory(EWeaponCategory NewWeaponCategory);

	int32 GetWeaponInventoryIndex() const;
	bool IsEquipped() const;
	EWeaponCategory GetWeaponCategory() const;


public:
	void PlayEquipSound();


protected:
	FHitResult WeaponTrace(float Distance, bool bDrawDebugLine = false);

	/* �ݵ�� �������� ����Ǿ� �ϴ� ����. */
	virtual void Fire();
	virtual void FireEnd();

protected:
	/* From client to server RPC */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire(bool bFire);
	bool ServerFire_Validate(bool bFire);
	void ServerFire_Implementation(bool bFire);

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, Replicated)
	FName AttachSocketName;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	class USkeletalMeshComponent* WeaponMesh;

protected:
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	class USoundBase* EquipSound;

	UPROPERTY(EditAnywhere)
	bool bToggleDebug;

};
