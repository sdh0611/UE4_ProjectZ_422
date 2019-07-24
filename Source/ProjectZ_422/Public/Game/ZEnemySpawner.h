// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/Actor.h"
#include "ZEnemySpawner.generated.h"

UCLASS()
class PROJECTZ_422_API AZEnemySpawner : public AActor
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

private:
	void SpawnEnemy();


private:
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

	FTimerHandle SpawnTimer;

private:
	TArray<class AZZombie*> EnemyPool;

private:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* SceneComponent;

};
