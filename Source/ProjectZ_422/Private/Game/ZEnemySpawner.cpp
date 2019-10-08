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

	/* 비활성화 상태로 생성 */
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
	
	/* Enemy Pool 채워놓기. */
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Owner = this;

	SpawnLocation = GetActorLocation();
	SpawnRotation = GetActorRotation();

	if (bIsPooling)
	{
		for (int i = 0; i < EnemyPoolSize; ++i)
		{
			/* 각각의 Enemy는 Spawn후 비활성화상태로 */

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
		/* Spawn Timer 등록 */
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
		/* SpawnMaximum에 맞춰서 활성화 */
		for (const auto& Enemy : EnemyPool)
		{
			if (!Enemy->IsActive())
			{
				/* 활성화 코드 */
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

