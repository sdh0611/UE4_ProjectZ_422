// Fill out your copyright notice in the Description page of Project Settings.


#include "ZInGameMenuWidget.h"
#include "ZBasePlayerController.h"
#include "ZClientGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UZInGameMenuWidget::OnLeaveGame()
{
	auto PC = Cast<AZBasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && PC->IsLocalPlayerController())
	{
		auto MyGameInstance = GetGameInstance<UZClientGameInstance>();
		if (MyGameInstance)
		{
			PC->ServerRemovePlayerSession(MyGameInstance->GetPlayerSessionID());
		}
	}


}
