// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Engine/TargetPoint.h"
#include "ZEnemySpawner.generated.h"

UCLASS()
class PROJECTZ_422_API AZEnemySpawner : public ATargetPoint
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void SetActive(bool bActive);

private:
	void SpawnEnemy();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Spawner)
	bool bIsPooling = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spawner)
	bool bIsActive;

	UPROPERTY(EditAnywhere, Transient, Category = Spawner)
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, Transient, Category = Spawner)
	FRotator SpawnRotation;

	UPROPERTY(EditAnywhere, Category = Spawner)
	float SpawnDelay;

	UPROPERTY(EditAnywhere, Category = Spawner)
	int32 SpawnMaximum;

	UPROPERTY(VisibleAnywhere, Category = Spawner)
	int32 CurrentAliveEnemies;

	UPROPERTY(EditAnywhere, Category = Spawner)
	int32 EnemyPoolSize;

	UPROPERTY(EditAnywhere, Category = Spawner)
	TSubclassOf<class AZBaseZombie> SpawnEnemyClass;

	FTimerHandle SpawnTimer;

private:
	TArray<class AZBaseZombie*> EnemyPool;

private:
	//UPROPERTY(VisibleAnywhere)
	//class USceneComponent* SceneComponent;

};
