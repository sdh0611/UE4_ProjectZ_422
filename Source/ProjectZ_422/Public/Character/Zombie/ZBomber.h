// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Character/Zombie/ZBaseZombie.h"
#include "ZBomber.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZBomber : public AZBaseZombie
{
	GENERATED_BODY()
	
public:
	AZBomber();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, class AActor* DamageCauser) override;

protected:
	/* SphereComponent�� OnComponentBeginOverlap ���ε��� �޼ҵ� */
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void Explosion();
	
	virtual void ChangeZombieState(EZombieState NewState) override;

	virtual void OnSensingPlayer(APawn* Pawn) override;

	virtual void OnDead() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastExplosion();
	void MulticastExplosion_Implementation();

protected:
	FTimerHandle ExplosionTimer;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsTriggered;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ExplosionDelay;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ElapsedTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundBase* ExplosionSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UParticleSystem* ExplosionParticle;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class URadialForceComponent* RadialForce;

};