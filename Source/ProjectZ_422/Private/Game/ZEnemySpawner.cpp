// Fill out your copyright notice in the Description page of Project Settings.


#include "ZEnemySpawner.h"
#include "ZZombie.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
AZEnemySpawner::AZEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnDelay = 3.f;
	SpawnMaximum = 10;
	CurrentAliveEnemies = 0;
	EnemyPoolSize = 10;

}

// Called when the game starts or when spawned
void AZEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	/* Enemy Pool ä������. */
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	SpawnLocation = GetActorLocation();
	SpawnRotation = GetActorRotation();

	for (int i = 0; i < EnemyPoolSize; ++i)
	{
		/* ������ Enemy�� Spawn�� ��Ȱ��ȭ���·� */
		auto Enemy = GetWorld()->SpawnActor<AZZombie>(AZZombie::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
		if (nullptr == Enemy)
		{
			EnemyPool.Add(Enemy);
		}

	}

	/* Spawn Timer ��� */
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &AZEnemySpawner::SpawnEnemy, SpawnDelay, true, SpawnDelay);

}

// Called every frame
void AZEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AZEnemySpawner::SpawnEnemy()
{
	/* SpawnMaximum�� ���缭 Ȱ��ȭ */
	for (const auto& Enemy : EnemyPool)
	{
		if (!Enemy->IsDead())
		{
			/* Ȱ��ȭ �ڵ� */
			Enemy->SetActorLocation(SpawnLocation);
			Enemy->SetActorRotation(SpawnRotation);

			/* 
				NOTE(7.24):
					Enemy��ü�� �ٽ� Ȱ��ȭ ��Ű�� �ڵ� ������ �� 
			*/
			Enemy->SetActive(true);

		}

	}


}

