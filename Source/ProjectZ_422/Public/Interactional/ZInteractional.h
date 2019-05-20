// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "GameFramework/Actor.h"
#include "ZInteractional.generated.h"

UCLASS()
class PROJECTZ_422_API AZInteractional : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZInteractional();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnInteraction(class AZCharacter* NewCharacter);
	// Character의 LineTrace와 충돌했을 때
	void OnFocus();
	// Character의 LineTrace에서 벗어났을 때
	void OnFocusEnd();

};
