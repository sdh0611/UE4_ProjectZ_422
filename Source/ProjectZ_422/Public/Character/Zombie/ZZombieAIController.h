// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "AIController.h"
#include "ZBaseZombie.h"
#include "ZZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZZombieAIController : public AAIController
{
	GENERATED_BODY()

public:
	AZZombieAIController();

public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
public:
	bool RunAI();
	void StopAI(const FString& Reason);
	void StopAI();

public:
	void SetTargetPawn(class APawn* Target);
	void SetZombieCurrentState(EZombieState NewState);

	const FName& GetHomePosKey() const;
	const FName& GetTargetPosKey() const;
	const FName& GetTargetActorKey() const;
	const FName& GetCurrentStateKey() const;

protected:
	/* ���� ������ �� ���ƿ� ��ǥ�� Ű���� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName HomePosKey;

	/* ���� ����� �ǽð� ��ǥ�� Ű���� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName TargetPosKey;

	/* �ǽð� ���� ��� Ű���� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName TargetActorKey;

	/* Zombie State�� Ű���� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName CurrentStateKey;

	

};
