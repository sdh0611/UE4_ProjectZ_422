// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "ZBaseCharacter.h"
#include "ZZombie.generated.h"

DECLARE_DELEGATE(FOnAttackEnd);

UENUM(BlueprintType)
enum class EZombieState : uint8
{
	Idle = 1,
	Attack,
	Chase,
	Dead
};

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZZombie : public AZBaseCharacter
{
	GENERATED_BODY()
	
public:
	AZZombie();

public:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, class AActor* DamageCauser) override;

public:
	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

public:
	void Attack();
	void AttackEnd();
	virtual void Revive() override;

public:
	virtual void SetActive(bool bActive) override;

public:
	void SetZombieState(EZombieState NewState);

	class UZZombieAnimInstance* const GetZombieAnimInstance() const;
	class UBehaviorTree* const GetZombieBT() const;
	EZombieState GetZombieState() const;


private:
	void AttackCheck();
	virtual void OnDead() override;

public:
	FOnAttackEnd OnAttackEnd;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Zombie)
	FName RightHandSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Zombie)
	FName LeftHandSocket;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Zombie)
	float AttackDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Zombie)
	bool bIsAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Zombie)
	EZombieState ZombieState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintReadOnly)
	class UBehaviorTree* ZombieBT;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Zombie)
	class UPawnSensingComponent* Sense;

	//UPROPERTY(EditDefaultsOnly, Category = Zombie)
	//class USphereComponent* AttackCollision;

};
