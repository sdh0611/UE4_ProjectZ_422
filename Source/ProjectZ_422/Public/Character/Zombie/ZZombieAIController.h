// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "AIController.h"
#include "ZZombie.h"
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
	/* 추적 실패할 시 돌아올 좌표값 키네임 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName HomePosKey;

	/* 추적 대상의 실시간 좌표값 키네임 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName TargetPosKey;

	/* 실시간 추적 대상 키네임 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName TargetActorKey;

	/* Zombie State값 키네임 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName CurrentStateKey;

	

};
