// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Components/ActorComponent.h"
#include "ZCharacterStatusComponent.generated.h"

/*
	Character의 능력치 및 상태 등을 관리하기 위한 컴포넌트.
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTZ_422_API UZCharacterStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UZCharacterStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void AdjustCurrentHP(float Value);

public:
	void SetCurrentHP(float NewCurrentHP);
	void SetMaxHP(float NewMaxHP);

public:
	bool IsDead() const;
	float GetCurrentHP() const;
	float GetMaxHP() const;

		
private:
	UPROPERTY(EditInstanceOnly, Category = Stat, Transient)
	float CurrentHP;

	UPROPERTY(EditAnywhere, Category = Stat)
	float MaxHP;

};
