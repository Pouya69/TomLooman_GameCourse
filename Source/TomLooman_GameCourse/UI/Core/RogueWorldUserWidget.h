// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RogueWorldUserWidget.generated.h"

class USizeBox;
/**
 * 
 */
UCLASS(Abstract)
class TOMLOOMAN_GAMECOURSE_API URogueWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category="UI")
		TObjectPtr<AActor> AttachTo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
		FVector WorldOffset;
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	// BindWidget will bind it in BP automatically one we make a USizeBox type named ParentSizeBox
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		TObjectPtr<USizeBox> ParentSizeBox;
	
};
