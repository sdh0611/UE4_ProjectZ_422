// Fill out your copyright notice in the Description page of Project Settings.


#include "ZPlayerController.h"
#include "ZHUD.h"


AZPlayerController::AZPlayerController()
{

}

AZHUD * const AZPlayerController::GetZHUD() const
{
	return Cast<AZHUD>(GetHUD());
}
