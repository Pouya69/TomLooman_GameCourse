// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RogueInteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class URogueInteractionInterface : public UInterface
{
	GENERATED_BODY()
};



class TOMLOOMAN_GAMECOURSE_API IRogueInteractionInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
		void Interact(AActor* CharacterInteracting);
	
	// For when the user cannot interact. E.g. When player already at full health and trying to interact.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void CannotInteract();
	
	// When user interacts and the interactable is on cooldown, this fires.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Cooldown")
		void OnCooldown();
};
