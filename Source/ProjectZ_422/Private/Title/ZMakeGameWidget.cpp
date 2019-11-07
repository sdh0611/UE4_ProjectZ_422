// Fill out your copyright notice in the Description page of Project Settings.


#include "ZMakeGameWidget.h"
#include "ZGameInstance.h"
#include "ZTitlePlayerController.h"
#include "ZPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Engine/Engine.h"

void UZMakeGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SessionName = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ETB_SessionName")));
	check(SessionName);

	ConnectNumber = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ETB_ConnectNumber")));
	check(ConnectNumber);

	OK = Cast<UButton>(GetWidgetFromName(TEXT("BTN_OK")));
	check(OK);

	OK->OnClicked.AddDynamic(this, &UZMakeGameWidget::OnCreateGameButtonClick);

}

void UZMakeGameWidget::OnCreateGameButtonClick()
{
	if (SessionName->GetText().IsEmpty())
	{
		/* ErrorText */

		return;
	}

	if (!ConnectNumber->GetText().IsNumeric())
	{
		/* ErrorText */

		return;
	}

	FString Num = ConnectNumber->GetText().ToString();
	if (Num.Contains(TEXT(".")))
	{
		/* ErrorText */

		return;
	}

	if (Num.Contains(TEXT("-")))
	{
		/* ErrorText */

		return;
	}
	
	FString Name = SessionName->GetText().ToString();


	auto MyGameInstance = GetGameInstance<UZGameInstance>();
	if (MyGameInstance)
	{
		//auto PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		//if (PC)
		//{
		//	auto PS = PC->GetPlayerState<APlayerState>();
		//	if (PS)
		//	{
		//		if (!MyGameInstance->HostSession(PS->UniqueId.GetUniqueNetId(), *Name, true, true, FCString::Atoi(*Num)))
		//		{
		//			/* ErrorText */
		//			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Failed to host session!"));
		//		}

		//	}
		//}
	}


}
