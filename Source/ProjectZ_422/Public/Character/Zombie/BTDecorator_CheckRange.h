// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "ZBaseZombie.h"
#include "BTDecorator_CheckRange.generated.h"

UENUM(BlueprintType)
enum class ETaskCondition : uint8
{
	Less = 0 UMETA(Display = "<="),
	Greater = 1 UMETA(Display = ">")
};

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UBTDecorator_CheckRange : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTDecorator_CheckRange();

public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETaskCondition Condition = ETaskCondition::Less;

};
