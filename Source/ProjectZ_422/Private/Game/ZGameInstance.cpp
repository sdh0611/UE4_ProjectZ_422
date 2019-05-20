// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGameInstance.h"
#include "Engine/StaticMesh.h"
#include "ConstructorHelpers.h"


UZGameInstance::UZGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable>
		DT_SM(TEXT("DataTable'/Game/GameData/StaticMeshData.StaticMeshData'"));
	if (DT_SM.Succeeded())
	{
		StaticMeshDataTable = DT_SM.Object;
		InitStaticMeshTable();
	}



}

void UZGameInstance::Init()
{
	Super::Init();

}

UStaticMesh * const UZGameInstance::GetStaticMesh(const FString & Name)
{
	if (StaticMeshTable.Contains(Name))
	{
		return StaticMeshTable[Name];
	}

	return nullptr;
}

void UZGameInstance::InitStaticMeshTable()
{
	// Init static mesh table
	TArray<FName> Names = StaticMeshDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		auto Data = StaticMeshDataTable->FindRow<FZStaticMeshData>(Name, TEXT(""));
		ConstructorHelpers::FObjectFinder<UStaticMesh> SM(*Data->StaticMeshPath);
		if (SM.Succeeded())
		{
			StaticMeshTable.Add(Data->Name, SM.Object);
		}
	}



}
