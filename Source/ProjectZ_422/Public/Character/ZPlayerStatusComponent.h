// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZCharacterStatusComponent.h"
#include "ZPlayerStatusComponent.generated.h"

/* Player Character의 상태를 관리하는 컴포넌트. */

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZPlayerStatusComponent : public UZCharacterStatusComponent
{
	GENERATED_BODY()
	
public:
	UZPlayerStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void AdjustCurrentDopingGage(float Value);

public:
	void SetCurrentDopingGage(float NewCurrentDopingGage);
	void SetMaxDopingGage(float NewMaxDopingGage);
	void SetSpeedIncrementWhileDoping(float NewIncrement);

public:
	float GetCurrentDopingGage() const;
	float GetMaxDopingGage() const;
	float GetSpeedIncrementWhileDoping() const;

	UFUNCTION(BlueprintCallable, Category = Status)
	float GetDopingGageRatio() const;

private:
	void DecreaseDopingGage();
	void IncreaseHPWhileDoping();

private:
	UFUNCTION()
	void OnRep_CurrentDopingGage();

protected:
	/*
		Doping
	*/
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Stat, Transient, ReplicatedUsing = OnRep_CurrentDopingGage)
	float CurrentDopingGage;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Stat, Transient)
	float MaxDopingGage;

	UPROPERTY(EditAnywhere, Category = Stat)
	float DecreaseDopingDelay;

	UPROPERTY(EditAnywhere, Category = Stat)
	float DecreaseDopingGageValue;

	UPROPERTY(EditAnywhere, Category = Stat)
	float SpeedIncrementWhileDoping;

	UPROPERTY()
	FTimerHandle DopingTimer;

};
