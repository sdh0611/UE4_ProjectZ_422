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

	/* Player Character의 경우 여러 상태에 대한 속도값이 다르기 때문에 override해줌. */
	virtual void SetCurrentSpeed(float NewSpeed);


public:
	bool IsDead() const;

	bool IsSprinting() const;

	/* Player Character의 경우 여러 상태에 대한 속도값이 다르기 때문에 override해줌. */
	virtual float GetCurrentSpeed() const;

	class UZCharacterStatusComponent* const GetStatusComponent() const;

	class UAnimInstance* GetAnimInstance() const;

protected:
	void CheckCharacterRotation(float DeltaTime);

protected:
	UPROPERTY(VisibleAnywhere, Category = State)
	bool bIsSprinting;

	UPROPERTY(EditAnywhere, Category = Stat)
	float WalkSpeed;

	UPROPERTY(EditAnywhere, Category = Stat)
	float SprintSpeed;

	FRotator Rotate;

protected:
	UPROPERTY(VisibleAnywhere, Category = ZCharacter)
	class UZCharacterStatusComponent* StatusComponent;

private:
	/* AnimInstance는 캐릭터마다 다르므로 파생 클래스의 BeginPlay에서 초기화 해줄 것. */
	UPROPERTY(VisibleAnywhere, Category = Character)
	class UAnimInstance* AnimInstance;

};
