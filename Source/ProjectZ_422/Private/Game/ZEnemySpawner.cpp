// Fill out your copyright notice in the Description page of Project Settings.


#include "ZEnemySpawner.h"
#include "ZBaseZombie.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "ZGameMode.h"
#include "ZGameState.h"

// Sets default values
AZEnemySpawner::AZEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* ��Ȱ��ȭ ���·� ���� */
	bIsActive = false;
	SpawnDelay = 3.f;
	SpawnMaximum = 10;
	CurrentAliveEnemies = 0;
	EnemyPoolSize = 10;
	
	//SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	//RootComponent = SceneComponent;

}

// Called when the game starts or when spawned
void AZEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	/* Enemy Pool ä������. */
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Owner = this;

	SpawnLocation = GetActorLocation();
	SpawnRotation = GetActorRotation();

	if (bIsPooling)
	{
		for (int i = 0; i < EnemyPoolSize; ++i)
		{
			/* ������ Enemy�� Spawn�� ��Ȱ��ȭ���·� */

			//auto Enemy = GetWorld()->SpawnActor<AZZombie>(AZZombie::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
			auto Enemy = GetWorld()->SpawnActor<AZBaseZombie>(SpawnEnemyClass, GetActorTransform(), SpawnParams);
			if (nullptr != Enemy)
			{
				Enemy->SetActive(false);
				Enemy->bIsPooling = true;
				EnemyPool.Add(Enemy);
			}

		}
	}

}

// Called every frame
void AZEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}

void AZEnemySpawner::SetActive(bool bActive)
{
	if (bActive)
	{
		/* Spawn Timer ��� */
		GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &AZEnemySpawner::SpawnEnemy, SpawnDelay, true, SpawnDelay);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
	}
}

void AZEnemySpawner::SpawnEnemy()
{
	if (bIsPooling)
	{
		/* SpawnMaximum�� ���缭 Ȱ��ȭ */
		for (const auto& Enemy : EnemyPool)
		{
			if (!Enemy->IsActive())
			{
				/* Ȱ��ȭ �ڵ� */
				Enemy->SetActorLocationAndRotation(SpawnLocation, SpawnRotation);
				Enemy->ClientSetLocationAndRotation(SpawnLocation, SpawnRotation);

				if (Enemy->IsDead())
				{
					Enemy->Revive();
				}
				else
				{
					Enemy->SetActive(true);
				}

				auto MyGameMode = GetWorld()->GetAuthGameMode<AZGameMode>();
				if (MyGameMode)
				{
					MyGameMode->AdjustZombieNum(1);
				}

				break;
			}


		}
	}
	else
	{
		if (CurrentAliveEnemies < SpawnMaximum)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			SpawnParams.Owner = this;

			auto Enemy = GetWorld()->SpawnActor<AZBaseZombie>(SpawnEnemyClass, GetActorTransform(), SpawnParams);
			if (nullptr == Enemy)
			{
				ZLOG(Error, TEXT("Failed to spawn enemy"));
			}

			Enemy->OnEnemyDead.BindLambda([&]() {
				--CurrentAliveEnemies;
			});

			++CurrentAliveEnemies;

			auto MyGameMode = GetWorld()->GetAuthGameMode<AZGameMode>();
			if (MyGameMode)
			{
				MyGameMode->AdjustZombieNum(1);
			}
		}
	}
}

