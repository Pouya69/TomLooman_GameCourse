// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueAction.h"
#include "RogueProjectileAttackAction.generated.h"

class ARogueProjectileBase;
class UNiagaraSystem;
class ARogueProjectileMagic;
/**
 * 
 */
UCLASS(Abstract)
class TOMLOOMAN_GAMECOURSE_API URogueProjectileAttackAction : public URogueAction
{
	GENERATED_BODY()
	
protected:
		
	UPROPERTY(EditDefaultsOnly, Category="Primary Attack")
		TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Primary Attack")
		TSubclassOf<ARogueProjectileMagic> MagicProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Primary Attack")
		TObjectPtr<UNiagaraSystem> MagicCastingEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Primary Attack")
		TObjectPtr<USoundBase> MagicCastingSound;
	
	UPROPERTY(VisibleAnywhere, Category = "Primary Attack")
		FName MuzzleSocketName;
	
	void AttackTimerElapsed(AActor* Instigator, const TSubclassOf<ARogueProjectileBase> ProjectileClassToSpawn);
	
public:
	URogueProjectileAttackAction();
	
	virtual void StartAction_Implementation(AActor* Instigator) override;
	
	virtual void StopAction_Implementation(AActor* Instigator) override;
};
