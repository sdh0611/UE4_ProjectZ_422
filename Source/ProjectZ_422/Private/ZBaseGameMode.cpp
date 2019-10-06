// Fill out your copyright notice in the Description page of Project Settings.


#include "ZBaseGameMode.h"
#include "ZBasePlayerController.h"

void AZBaseGameMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);

	/* DB�� Logout ���·� ����. */
	auto MyPC = Cast<AZBasePlayerController>(Exiting);
	if (MyPC)
	{
		MyPC->ClientRequestLogout();
	}

}
