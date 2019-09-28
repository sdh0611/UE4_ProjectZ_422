// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Components/ActorComponent.h"
#include "ZCharacterStatusComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnStatusChanged);

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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void AdjustCurrentHP(float Value);

public:
	void SetCurrentHP(float NewCurrentHP);
	void SetMaxHP(float NewMaxHP);

public:
	bool IsDead() const;
	float GetCurrentHP() const;
	float GetMaxHP() const;

	UFUNCTION(BlueprintCallable, Category = Status)
	float GetHPRatio() const;

protected:
	/* Replicated using */
	UFUNCTION()
	void OnRep_CurrentHP();

public:
	FOnStatusChanged OnStatusChanged;

	UPROPERTY(EditAnywhere, Category = Stat)
	bool bCanInterpGageValues;

protected:
	FTimerHandle IncreaseHPTimer;

	UPROPERTY(VisibleAnywhere)
	class AZBaseCharacter* OwnerCharacter;

protected:
	/*
		HP
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Transient, ReplicatedUsing = OnRep_CurrentHP)
	float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Transient, Replicated)
	float MaxHP;


};
