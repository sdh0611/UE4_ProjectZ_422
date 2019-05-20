// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "ZType.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZType : public UObject
{
	GENERATED_BODY()
	
};

USTRUCT(BlueprintType)
struct FZStaticMeshData : public FTableRowBase
{

	GENERATED_BODY()

public:
	FZStaticMeshData()
		:Name(TEXT("")), StaticMeshPath(TEXT(""))
	{

	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponData)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponData)
	FString StaticMeshPath;

};