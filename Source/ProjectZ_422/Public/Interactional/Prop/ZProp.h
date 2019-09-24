// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ZAITargetableInterface.h"
#include "GenericTeamAgentInterface.h"
#include "ZProp.generated.h"

UCLASS()
class PROJECTZ_422_API AZProp : public AActor, public IZAITargetableInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, class AActor* DamageCauser) override;

	virtual bool IsTargetable() const override;
	virtual bool IsDead() const override;

	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	FGenericTeamId TeamId;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentHP = 100.f;

protected:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BodyCollision;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* BodyMesh;


};
