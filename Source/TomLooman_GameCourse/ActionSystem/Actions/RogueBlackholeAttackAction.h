// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueAction.h"
#include "RogueBlackholeAttackAction.generated.h"

class ARogueProjectileBlackhole;
class UNiagaraSystem;
class ARogueProjectileBase;
/**
 * 
 */
UCLASS(Abstract)
class TOMLOOMAN_GAMECOURSE_API URogueBlackholeAttackAction : public URogueAction
{
	GENERATED_BODY()
	
protected:
		
	UPROPERTY(EditDefaultsOnly, Category="Primary Attack")
		TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category="Blackhole Attack")
		TSubclassOf<ARogueProjectileBlackhole> BlackholeProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackhole Attack")
		TObjectPtr<UNiagaraSystem> BlackholeCastingEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackhole Attack")
		TObjectPtr<USoundBase> BlackholeCastingSound;
	
	UPROPERTY(VisibleAnywhere, Category = "Primary Attack")
		FName MuzzleSocketName;
	
	void AttackTimerElapsed(AActor* Instigator, const TSubclassOf<ARogueProjectileBase> ProjectileClassToSpawn);
	
public:
	URogueBlackholeAttackAction();
	
	virtual void StartAction_Implementation(AActor* Instigator) override;
	
	virtual void StopAction_Implementation(AActor* Instigator) override;
};
