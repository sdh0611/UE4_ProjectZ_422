// Fill out your copyright notice in the Description page of Project Settings.


#include "ZPlayerState.h"

AZPlayerState::AZPlayerState()
{
	Kills = 0;


}

void AZPlayerState::AddKill()
{
	++Kills;
}
