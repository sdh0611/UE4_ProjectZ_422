// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "Blueprint/UserWidget.h"
#include "ZInputNumberWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZInputNumberWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void NativeConstruct() override;
	
public:
	void BindWidget(class UUserWidget* NewWidget);

	int32 GetInputNumber() const;	
	

protected:
	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
	UFUNCTION()
	void OnOKButtonClicked();

	UFUNCTION()
	void OnCancelButtonClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UEditableTextBox* InputText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UButton* OKButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UButton* CancelButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UUserWidget* WidgetBinded;

};
