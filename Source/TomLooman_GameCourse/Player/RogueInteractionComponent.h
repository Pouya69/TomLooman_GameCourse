// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RogueInteractionComponent.generated.h"


class URogueWorldUserWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOMLOOMAN_GAMECOURSE_API URogueInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	URogueInteractionComponent();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
		TEnumAsByte<ECollisionChannel> CollisionChannel;
	
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
		float InteractionRadius;
	
	UPROPERTY(VisibleAnywhere, Category="Interaction")
		TObjectPtr<AActor> SelectedActor;
	
	// For interaction indicator. TODO: Implement it using InFocus for each actor later. This is for debugging only.
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
		TSubclassOf<URogueWorldUserWidget> DefaultWidgetClass;
	
	UPROPERTY()
		URogueWorldUserWidget* DefaultWidgetInstance;
	
	void FindBestInteractable();

public:
	void Interact();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
