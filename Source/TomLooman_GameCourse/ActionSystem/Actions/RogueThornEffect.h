// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueActionEffect.h"
#include "RogueThornEffect.generated.h"

class URogueActionSystemComponent;
/**
 * 
 */
UCLASS(Abstract)
class TOMLOOMAN_GAMECOURSE_API URogueThornEffect : public URogueActionEffect
{
	GENERATED_BODY()
	
public:
	URogueThornEffect();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Thorn", meta = (UIMin = 0, UIMax = 1.0f, ClampMin = 0.0f, ClampMax = 1.0f))
		float DamagePercentageToApplyToAttacker;
	
	virtual void StartAction_Implementation(AActor* Instigator) override;
	
	virtual void StopAction_Implementation(AActor* Instigator) override;
	
	UFUNCTION()
		void OnHealthChanged(AActor* InstigatorActor, URogueActionSystemComponent* OwningComp, const float NewHealth, const float OldHealth);
};
