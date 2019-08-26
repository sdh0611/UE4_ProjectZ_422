// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/Character.h"
#include "ZBaseCharacter.generated.h"

UCLASS()
class PROJECTZ_422_API AZBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, class AActor* DamageCauser) override;

public:
	FHitResult GetTraceHit(const FVector& TraceStart, const FVector& TraceEnd);
	virtual void Revive();


public:
	void SetIsSprinting(bool NewState);

	/* Player Character�� ��� ���� ���¿� ���� �ӵ����� �ٸ��� ������ override����. */
	virtual void SetCurrentSpeed(float NewSpeed);

	virtual void SetActive(bool bActive);

public:
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	bool IsActive() const;

	bool IsSprinting() const;

	float GetWalkSpeed() const;

	float GetSprintSpeed() const;

	float GetCurrentSpeed() const;

	class UZCharacterStatusComponent* const GetStatusComponent() const;

	class UZCharacterAnimInstance* GetAnimInstance() const;

protected:
	void CheckCharacterRotation(float DeltaTime);
	virtual void OnDead();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	bool bIsSprinting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Zombie)
	bool bIsPooling;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	bool bIsActive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	class UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	class USoundBase* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	class USoundBase* DeadSound;

	FRotator Rotate;

	FTimerHandle InactiveTimer;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter)
	class UZCharacterStatusComponent* StatusComponent;

protected:
	/* AnimInstance�� ĳ���͸��� �ٸ��Ƿ� �Ļ� Ŭ������ BeginPlay���� �ʱ�ȭ ���� ��. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	class UZCharacterAnimInstance* AnimInstance;

};
