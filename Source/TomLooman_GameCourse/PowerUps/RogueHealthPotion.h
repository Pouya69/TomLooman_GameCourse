// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoguePowerupBase.h"
#include "RogueHealthPotion.generated.h"

UCLASS()
class TOMLOOMAN_GAMECOURSE_API ARogueHealthPotion : public ARoguePowerupBase
{
	GENERATED_BODY()

public:
	ARogueHealthPotion();
	
	virtual void StartCooldown() override;
	
	virtual void PowerUp(ACharacter* CharacterInteracting) override;
	
	virtual void OnCooldown_Implementation() override;
	
	virtual void CannotInteract_Implementation() override;
	
protected:
	virtual void Interact_Implementation(AActor* CharacterInteracting) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Credits")
		int CreditsNeededToUse;
};
