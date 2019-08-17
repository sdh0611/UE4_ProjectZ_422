// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Zombie/ZBaseZombie.h"
#include "ZTanker.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZTanker : public AZBaseZombie
{
	GENERATED_BODY()
	
public:
	AZTanker();

public:
	virtual void BeginPlay() override;
	virtual void ChangeZombieState(EZombieState NewState) override;

protected:
	virtual void AttackCheck();
	virtual void OnDead() override;
	virtual void OnSensingPlayer(APawn* Pawn) override;

protected:
	/* SphereComponent의 OnComponentBeginOverlap 바인딩용 메소드 */
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void ToggleRush(bool bInRush);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsRushing;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ImpulseStrength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RushDamage;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* ImpulseSphere;


};
