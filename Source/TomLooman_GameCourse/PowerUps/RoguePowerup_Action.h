// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoguePowerupBase.h"
#include "RoguePowerup_Action.generated.h"

class URogueAction;

UCLASS()
class TOMLOOMAN_GAMECOURSE_API ARoguePowerup_Action : public ARoguePowerupBase
{
	GENERATED_BODY()

public:
	ARoguePowerup_Action();

protected:
	virtual void PostInitializeComponents() override;
	
	UFUNCTION()
		void OnPlayerOverlapWithThis(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	virtual void PowerUp(ACharacter* CharacterInteracting) override;
	
	UPROPERTY(EditDefaultsOnly, Category="Action")
		TSubclassOf<URogueAction> ActionToGrant;
};
