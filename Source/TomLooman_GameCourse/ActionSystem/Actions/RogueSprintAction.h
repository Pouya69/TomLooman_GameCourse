// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueAction.h"
#include "RogueSprintAction.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class TOMLOOMAN_GAMECOURSE_API URogueSprintAction : public URogueAction
{
	GENERATED_BODY()
	
public:
	URogueSprintAction();
	
	UPROPERTY(EditDefaultsOnly, Category="Sprint")
		float SpeedAddition = 80.0f;
	
	virtual void StartAction_Implementation(AActor* Instigator) override;
	
	virtual void StopAction_Implementation(AActor* Instigator) override;
};
