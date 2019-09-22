// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectZ_422.h"
#include "ZUserWidget.h"
#include "ZInputNumberWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_422_API UZInputNumberWidget : public UZUserWidget
{
	GENERATED_BODY()


public:
	virtual void NativeConstruct() override;
	virtual void OnDrawScreen() override;
	virtual void OnRemoveScreen() override;

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

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

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
