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

public:
	void SetIsSprinting(bool NewState);

	/* Player Character�� ��� ���� ���¿� ���� �ӵ����� �ٸ��� ������ override����. */
	virtual void SetCurrentSpeed(float NewSpeed);


public:
	bool IsSprinting();

	/* Player Character�� ��� ���� ���¿� ���� �ӵ����� �ٸ��� ������ override����. */
	virtual float GetCurrentSpeed() const;

	class UZCharacterStatusComponent* const GetStatusComponent() const;

	class UAnimInstance* const GetAnimInstance();

protected:
	void CheckCharacterRotation(float DeltaTime);

protected:
	UPROPERTY(VisibleAnywhere, Category = State)
	bool bIsSprinting;

	UPROPERTY(EditAnywhere, Category = Stat)
	float WalkSpeed;

	UPROPERTY(EditAnywhere, Category = Stat)
	float SprintSpeed;

	/* AnimInstance�� ĳ���͸��� �ٸ��Ƿ� �Ļ� Ŭ������ BeginPlay���� �ʱ�ȭ ���� ��. */
	UPROPERTY(VisibleAnywhere, Category = Character)
	class UAnimInstance* AnimInstance;

	FRotator Rotate;

protected:
	UPROPERTY(VisibleAnywhere, Category = ZCharacter)
	class UZCharacterStatusComponent* StatusComponent;
};