// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/Character.h"
#include "ZBaseCharacter.h"
#include "ZCharacter.generated.h"

UCLASS()
class PROJECTZ_422_API AZCharacter : public AZBaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void PossessedBy(AController* NewController) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, 
		class AController* EventInstigator, class AActor* DamageCauser) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Revive() override;


public:
	FHitResult GetTraceHitFromActorCameraView(float Distance);

	void AttachWeapon(class AZWeapon* Weapon, const FName& SocketName);
	void DetachWeapon(int32 NewWeaponIndex);

	/* Server to client call RPC */



	/* Client to server call RPC */



	/* Net multicast */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayMontage(const FString& MontageName);
	void MulticastPlayMontage_Implementation(const FString& MontageName);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void MulticastAttachWeapon(class AZWeapon* Weapon, FName SocketName);
	bool MulticastAttachWeapon_Validate(class AZWeapon* Weapon, FName SocketName);
	void MulticastAttachWeapon_Implementation(class AZWeapon* Weapon, FName SocketName);

public:
	void SetIsAiming(bool NewState);
	void SetIsSwitchingWeapon(bool NewState);
	void SetCurrentWeapon(class AZWeapon* NewWeapon);
	virtual void SetActive(bool bActive) override;

	bool IsEquipWeapon();
	bool IsAiming();
	bool IsSwitchingWeapon();
	class AZInteractional* GetInteractionalInView();
	class UZCharacterItemStatusComponent* const GetItemStatusComponent() const;
	class UZPlayerStatusComponent* const GetPlayerStatusComponent() const;
	class AZWeapon* const GetCurrentWeapon();
	class UZPlayerAnimInstance* const GetCharacterAnimInstance();

	const FName& GetMainWeaponSocketName() const;
	const FName& GetMainWeaponShotgunSocketName() const;
	const FName& GetSecondaryWeaponSocketName() const;
	const FName& GetThirdWeaponSocketName() const;
	const FName& GetKnifeSocketName() const;
	const FName& GetGrenadeWeaponSocketName() const;
	   
private:
	void CheckCharacterRotation(float DeltaTime);
	virtual void OnDead() override;
	virtual void OnRemoved() override;
	

private:
	/*
		Ű �Է°� ���õ� �޼ҵ��
	*/
	/*
		Character �⺻ �����ӵ�
	*/
	void MoveForward(float NewAxisValue);
	void MoveRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void LookRight(float NewAxisValue);
	void ToggleCrouch();
	void Sprint();
	void SprintRelease();
	virtual void Jump() override;

	/*
		Item ���� ���۵�
	*/
	void Interaction();
	//void ToggleInventory();
	void Attack();
	void AttackEnd();
	void Aim();
	void AimRelease();
	void Reload();
	void DropWeapon();
	void SwitchWeapon(int32 NewWeaponIndex);
	void Slot1();
	void Slot2();
	void Slot3();
	void Slot4();
	void ChangeFireMode();
	//void ToggleInGameMenu();
	//void RemoveWidgetFromTop();

	/*
		������
	*/
	void AddMoney();
	void DamageSelf();
	void Ragdoll();


private:
	/* Client to server call RPC */
	UFUNCTION(Server, WithValidation, Reliable)
	void ServerEquipWeapon(class AZWeapon* NewWeapon);
	bool ServerEquipWeapon_Validate(class AZWeapon* NewWeapon);
	void ServerEquipWeapon_Implementation(class AZWeapon* NewWeapon);

	UFUNCTION(Server, WithValidation, Reliable)
	void ServerSwitchWeapon(int32 NewWeaponIndex);
	bool ServerSwitchWeapon_Validate(int32 NewWeaponIndex);
	void ServerSwitchWeapon_Implementation(int32 NewWeaponIndex);

	/* Server to client call RPC */
	//UFUNCTION(Client, Reliable)


	/* Replicated using method */
	UFUNCTION()
	void OnRep_CurrentWeapon();

	UFUNCTION()
	void OnRep_IsAiming();

protected:
	/* Sockets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MainWeaponSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MainWeaponShotgunSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SecondaryWeaponSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ThirdWeaponSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName KnifeSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName GrenadeWeaponSocketName;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, ReplicatedUsing = OnRep_IsAiming)
	bool bIsAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	bool bIsSwitchingWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	float WalkSpeedCrouched;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	float AimingWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	float AimingWalkSpeedCrouched;

	UPROPERTY()
	class AZInteractional* InteractionActor;

	UPROPERTY()
	class AZPlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere, Category = Character, BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentWeapon)
	class AZWeapon* CurrentWeapon;

	FRotator Rotate;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter)
	class USpringArmComponent* MainCameraSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter)
	class UCameraComponent* MainCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZCharacter)
	class UZCharacterItemStatusComponent* ItemStatusComponent;

};
