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
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	FHitResult GetTraceHit(const FVector& TraceStart, const FVector& TraceEnd);
	virtual void Revive();


public:
	void SetIsSprinting(bool NewState);

	void SetCharacterWalkSpeed(float NewSpeed);
	void SetCharacterCrouchWalkSpeed(float NewSpeed);

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

public:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetWalkSpeed(float NewSpeed);
	bool ServerSetWalkSpeed_Validate(float NewSpeed);
	void ServerSetWalkSpeed_Implementation(float NewSpeed);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetCrouchWalkSpeed(float NewSpeed);
	bool ServerSetCrouchWalkSpeed_Validate(float NewSpeed);
	void ServerSetCrouchWalkSpeed_Implementation(float NewSpeed);

	/* Net multicast */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayMontage(const FString& MontageName);
	void MulticastPlayMontage_Implementation(const FString& MontageName);

protected:
	void CheckCharacterRotation(float DeltaTime);
	virtual void OnDead();
	/* 
		게임 상에서 사라질 때 호출할 메소드. 
		상속받은 클래스에서 동작을 정의해줘야함.
	*/
	virtual void OnRemoved();

protected:
	/* Client to server call RPC */
	UFUNCTION(Server, WithValidation, Reliable)
	void ServerSetSprinting(bool bNewState);
	bool ServerSetSprinting_Validate(bool bNewState);
	void ServerSetSprinting_Implementation(bool bNewState);


	/* Server to client call RPC */
	

	/* Net multicast */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnHit();
	void MulticastOnHit_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnDead();
	void MulticastOnDead_Implementation();

	/* Replicated using method */
	UFUNCTION()
	virtual void OnRep_IsSprinting();

	UFUNCTION()
	void OnRep_IsActive();


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State, ReplicatedUsing = OnRep_IsSprinting)
	bool bIsSprinting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Zombie)
	bool bIsPooling;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, ReplicatedUsing = OnRep_IsActive)
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Replicated)
	bool bIsActive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	float DisappearTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	class UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	class USoundBase* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	class USoundBase* DeadSound;

	FTimerHandle InactiveTimer;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter)
	class UZCharacterStatusComponent* StatusComponent;

protected:
	/* AnimInstance는 캐릭터마다 다르므로 파생 클래스의 BeginPlay에서 초기화 해줄 것. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	class UZCharacterAnimInstance* AnimInstance;

};
