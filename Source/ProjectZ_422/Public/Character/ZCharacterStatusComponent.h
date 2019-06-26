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

public:
	void AdjustCurrentHP(float Value);
	void AdjustCurrentDopingGage(float Value);
	void AddBuff(class UZBuff* NewBuff);
	void UpdateBuff(float DeltaTime);

public:
	void SetCurrentHP(float NewCurrentHP);
	void SetMaxHP(float NewMaxHP);
	void SetCurrentDopingGage(float NewCurrentDopingGage);
	void SetMaxDopingGage(float NewMaxDopingGage);
	void SetSpeedIncrementWhiledoping(float NewIncrement);

public:
	bool IsDead() const;
	float GetCurrentHP() const;
	float GetMaxHP() const;
	float GetCurrentDopingGage() const;
	float GetMaxDopingGage() const;
	float GetSpeedIncrementWhileDoping() const;

	UFUNCTION(BlueprintCallable, Category = Status)
	float GetHPRatio() const;

	UFUNCTION(BlueprintCallable, Category = Status)
	float GetTargetHPRatio() const;

	UFUNCTION(BlueprintCallable, Category = Status)
	float GetDopingGageRatio() const;

	UFUNCTION(BlueprintCallable, Category = Status)
	float GetTargetDopingGageRatio() const;


private:
	void DecreaseDopingGage();
	void InterpGage(float DeltaTime);
	void IncreaseHPWhileDoping();

public:
	FOnStatusChanged OnStatusChanged;

	UPROPERTY(EditAnywhere, Category = Stat)
	bool bCanInterpGageValues;

private:
	UPROPERTY()
	class AZCharacter* OwnerCharacter;

private:
	/*
		HP
	*/
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Stat, Transient, Meta = (AllowPrivateAccess = true))
	float CurrentHP;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Stat, Transient, Meta = (AllowPrivateAccess = true))
	float MaxHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Transient, Meta = (AllowPrivateAccess = true))
	float TargetHP;

	/*
		Doping
	*/
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Stat, Transient, Meta = (AllowPrivateAccess = true))
	float CurrentDopingGage;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Stat, Transient, Meta = (AllowPrivateAccess = true))
	float MaxDopingGage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Transient, Meta = (AllowPrivateAccess = true))
	float TargetDopingGage;

	UPROPERTY(EditAnywhere, Category = Stat)
	float DecreaseDopingDelay;

	UPROPERTY(EditAnywhere, Category = Stat)
	float DecreaseDopingGageValue;

	UPROPERTY(EditAnywhere, Category = Stat)
	float SpeedIncrementWhileDoping;

	UPROPERTY()
	FTimerHandle DopingTimer;

	UPROPERTY()
	FTimerHandle IncreaseHPTimer;

	/*
		Buff list
	*/
	UPROPERTY(VisibleAnywhere, Category = Stat)
	TArray<class UZBuff*> BuffList;

};
