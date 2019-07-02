// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "BehaviorTree/BTService.h"
#include "BTService_FindPlayer.generated.h"

/**
 *
 */
UCLASS()
class PROJECTZ_422_API UBTService_FindPlayer : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_FindPlayer();


protected:
	virtual void TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds) override;


private:
	class AZCharacter* SweepTrace(class APawn* Zombie);


private:
	//UPROPERTY()
	//class AZZombieAIController* ZombieController;

	//UPROPERTY()
	//class AZBaseCharacter* TargetCharacter;

};
