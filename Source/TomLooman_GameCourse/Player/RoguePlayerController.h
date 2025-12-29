// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RoguePlayerController.generated.h"

class UInputAction;
class URogueInteractionComponent;
/**
 * 
 */
UCLASS()
class TOMLOOMAN_GAMECOURSE_API ARoguePlayerController : public APlayerController
{
	GENERATED_BODY()
	
	// We add interactionComp in PlayerController for better code division AND multiplayer.
	// We don't want other players to know about our Character having Interactions etc. same as us.
	// That is why we make these components inside of PlayerController because there will be only 1 PlayerController.
	
public:
	ARoguePlayerController();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Component")
		TObjectPtr<URogueInteractionComponent> InteractionComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
		TObjectPtr<UInputAction> Input_Interact;
	
	void StartInteract();
	
	virtual void SetupInputComponent() override;
};
