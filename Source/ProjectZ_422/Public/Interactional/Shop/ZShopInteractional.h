// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactional/ZInteractional.h"
#include "ZShopInteractional.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API AZShopInteractional : public AZInteractional
{
	GENERATED_BODY()

public:
	AZShopInteractional();

public:
	virtual void OnInteraction(class AZCharacter* NewCharacter);
	virtual void OnFocus() override;
	virtual void OnFocusEnd() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Shop)
	class UBoxComponent* BodyCollision;

	UPROPERTY(VisibleAnywhere, Category = Shop)
	class USkeletalMeshComponent* BodyMesh;

};
