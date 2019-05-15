// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/Character.h"
#include "ZCharacter.generated.h"

UCLASS()
class PROJECTZ_422_API AZCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void SetIsSprinting(bool NewState);

public:
	bool IsSprinting();
	class AZInteractional* GetInteractionalInView();
	class UZCharacterItemStatusComponent* const GetItemStatusComponent() const;

private:
	void MoveForward(float NewAxisValue);
	void MoveRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void LookRight(float NewAxisValue);
	void ToggleCrouch();
	void Sprint();
	void SprintRelease();
	void Interaction();
	virtual void Jump() override;


private:
	UPROPERTY(VisibleAnywhere, Category = State)
	bool bIsSprinting;
	
	UPROPERTY(EditAnywhere, Category = Stat)
	float WalkSpeed;

	UPROPERTY(EditAnywhere, Category = Stat)
	float SprintSpeed;

	UPROPERTY()
	class AZInteractional* InteractionActor;

private:
	UPROPERTY(VisibleAnywhere, Category = ZCharacter)
	class USpringArmComponent* MainCameraSpringArm;

	UPROPERTY(VisibleAnywhere, Category = ZCharacter)
	class UCameraComponent* MainCamera;

	UPROPERTY(VisibleAnywhere, Category = ZCharacter)
	class UZCharacterItemStatusComponent* ItemStatusComponent;

};
