// Fill out your copyright notice in the Description page of Project Settings.


#include "ZAITargetableInterface.h"
#include "..\..\Public\Interface\ZAITargetableInterface.h"

// Add default functionality here for any IZAITargetableInterface functions that are not pure virtual.

bool IZAITargetableInterface::IsTargetable() const
{
	return false;
}

bool IZAITargetableInterface::IsDead() const
{
	return false;
}
