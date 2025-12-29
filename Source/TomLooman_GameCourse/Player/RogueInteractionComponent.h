// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RogueInteractionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOMLOOMAN_GAMECOURSE_API URogueInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	URogueInteractionComponent();

protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
		float InteractionRadius;
	
	UPROPERTY(VisibleAnywhere, Category="Interaction")
		TObjectPtr<AActor> SelectedActor;

public:
	void Interact();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
