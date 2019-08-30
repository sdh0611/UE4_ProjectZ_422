// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ZNumberWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UZNumberWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTZ_422_API IZNumberWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnReceiveNumberInput(int32 NewNumber);


};
