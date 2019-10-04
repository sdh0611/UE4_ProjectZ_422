// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
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
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, class AActor* DamageCauser) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void SetActive(bool bActive) override;

public:
	void ToggleRush(bool bInRush);

protected:
	virtual void AttackCheck();
	virtual void OnDead() override;
	virtual void OnSensingPlayer(APawn* Pawn) override;

	/* SphereComponent�� OnComponentBeginOverlap ���ε��� �޼ҵ� */
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

public:
	//bool IsRushing() const;
	bool IsRushCooldown() const;

protected:
	FTimerHandle RushTimer;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	//bool bIsRushing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsRushCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ImpulseStrength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RushSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RushDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RushDelay;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//class UParticleSystem* RushExplosionParticle;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* ImpulseSphere;


};
