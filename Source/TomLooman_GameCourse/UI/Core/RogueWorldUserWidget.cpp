// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

void URogueWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	// Is Valid checks for null AND checks if it is not being Destroyed
	if (!ParentSizeBox) return;
	
	if (!IsValid(AttachTo))
	{
		RemoveFromParent();
		
		UE_LOG(LogTemp, Warning, TEXT("AttachTo actor no longer valid for MinionHealthbarWidget. Removing the widget."));
		return;
	}
	
	FVector2D ScreenPosition;
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachTo->GetActorLocation() + WorldOffset, ScreenPosition))
	{
		const float Scale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
		
		ScreenPosition /= Scale;
		ParentSizeBox->SetRenderTranslation(ScreenPosition);
	}
}
