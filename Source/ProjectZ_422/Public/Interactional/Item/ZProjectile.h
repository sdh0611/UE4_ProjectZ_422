// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/Actor.h"
#include "ZProjectile.generated.h"

UCLASS()
class PROJECTZ_422_API AZProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void SetDamage(float NewDamage);
	virtual void FireInDirection(const FVector& Direction);

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, 
		FVector NormalImpulse, const FHitResult& Hit);

protected:
	TSubclassOf<class UDamageType> DamageType;

protected:
	//UPROPERTY(VisibleAnywhere, Category = Projectile)
	//class USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere, Category = Projectile)
	class UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, Category = Projectile)
	class UProjectileMovementComponent* Movement;

	UPROPERTY(VisibleAnywhere, Category = Projectile)
	class URadialForceComponent* RadialForce;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	float LifeSpan;

};
