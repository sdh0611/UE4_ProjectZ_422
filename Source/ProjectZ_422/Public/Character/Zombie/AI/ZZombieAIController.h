// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "AIController.h"
#include "ZBaseZombie.h"
#include "Perception/AIPerceptionTypes.h"
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
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	bool RunAI();
	void StopAI(const FString& Reason);
	void StopAI();

public:
	void SetTargetActor(class AActor* Target);
	void SetZombieCurrentState(EZombieState NewState);

	UObject* const GetTargetPawn() const;
	const FName& GetHomePosKey() const;
	const FName& GetTargetPosKey() const;
	const FName& GetTargetActorKey() const;
	const FName& GetCurrentStateKey() const;

protected:
	UFUNCTION()
	void OnPerceptionUpdate(const TArray<AActor*>& UpdatedActors);

	UFUNCTION()
	void OnTargetPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus);

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

	UPROPERTY(VisibleDefaultsOnly)
	class UAISenseConfig_Sight* SightConfig;

protected:
	UPROPERTY(VisibleAnywhere)
	class UAIPerceptionComponent* AISense;
	


};
