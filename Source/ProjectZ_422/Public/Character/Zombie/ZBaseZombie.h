// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZBaseCharacter.h"
#include "ZBaseZombie.generated.h"

DECLARE_DELEGATE(FOnAttackEnd);

UENUM(BlueprintType)
enum class EZombieState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	Attack UMETA(DisplayName = "Attack"),
	Chase UMETA(DisplayName = "Chase"),
	Dead UMETA(DisplayName = "Dead")
};

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZBaseZombie : public AZBaseCharacter
{
	GENERATED_BODY()

public:
	AZBaseZombie();

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
	virtual void ChangeZombieState(EZombieState NewState);

	int32 GetZombieValue() const;
	int32 GetZombieScore() const;
	class UZZombieAnimInstance* const GetZombieAnimInstance() const;
	class UBehaviorTree* const GetZombieBT() const;
	EZombieState GetZombieState() const;
	

protected:
	virtual void AttackCheck();
	virtual void OnDead() override;
	virtual void OnRemoved() override;
	virtual void OnSensingPlayer(APawn* Pawn);


public:
	FOnAttackEnd OnAttackEnd;

protected:
	/* 해당 개체를 잡았을 경우 얻게되는 돈의 값 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Zombie)
	int32 ZombieValue;

	/* 해당 개체를 잡았을 경우 얻게되는 점수의 값 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Zombie)
	int32 ZombieScore;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Zombie)
	float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Zombie)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Zombie)
	float AttackRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Zombie)
	EZombieState ZombieState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintReadOnly)
	class UBehaviorTree* ZombieBT;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, BlueprintReadOnly)
	class USoundBase* DetectSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, BlueprintReadOnly)
	class USoundBase* AttackSound;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Zombie)
	class UPawnSensingComponent* Sense;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Zombie)
	//class UAIPerceptionComponent* AIPerception;

};
