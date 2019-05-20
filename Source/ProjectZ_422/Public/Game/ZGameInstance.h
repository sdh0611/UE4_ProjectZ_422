// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Engine/GameInstance.h"
#include "ZGameInstance.generated.h"

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

public:
	class UStaticMesh* const GetStaticMesh(const FString& Name);

private:
	void InitStaticMeshTable();

private:
	UPROPERTY(VisibleAnywhere, Category = DataTable)
	class UDataTable* StaticMeshDataTable;
	
private:
	UPROPERTY(VisibleAnywhere, Category = MeshTable)
	TMap<FString, class UStaticMesh*> StaticMeshTable;

};
