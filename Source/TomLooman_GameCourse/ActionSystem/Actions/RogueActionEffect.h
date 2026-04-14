// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueAction.h"
#include "RogueActionEffect.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class TOMLOOMAN_GAMECOURSE_API URogueActionEffect : public URogueAction
{
	GENERATED_BODY()
	
public:
	URogueActionEffect();
	
protected:
	// Total duration of the effect
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
		float Duration;
	
	// Time between each 'Tick' (more like a heartbeat)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
		float Period;
	
	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
		void ExecutePeriodicEffect(AActor* Instigator);
	
	virtual void StartAction_Implementation(AActor* Instigator) override;
	
	virtual void StopAction_Implementation(AActor* Instigator) override;
};
