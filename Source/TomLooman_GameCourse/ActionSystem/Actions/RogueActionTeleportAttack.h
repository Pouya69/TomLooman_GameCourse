// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueAction.h"
#include "RogueActionTeleportAttack.generated.h"

class ARogueProjectileBase;
class UNiagaraSystem;
class ARogueProjectileTeleporter;
/**
 * 
 */
UCLASS(Abstract)
class TOMLOOMAN_GAMECOURSE_API URogueActionTeleportAttack : public URogueAction
{
	GENERATED_BODY()
	
public:
	URogueActionTeleportAttack();
	
	virtual void StartAction_Implementation(AActor* Instigator) override;
	
	virtual void StopAction_Implementation(AActor* Instigator) override;
	
protected:
	
	
	UPROPERTY(EditDefaultsOnly, Category="Primary Attack")
		TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category="Secondary Attack")
		TSubclassOf<ARogueProjectileTeleporter> TeleporterProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Secondary Attack")
		TObjectPtr<UNiagaraSystem> TeleporterCastingEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Secondary Attack")
		TObjectPtr<USoundBase> TeleporterCastingSound;
	
	UPROPERTY(VisibleAnywhere, Category = "Secondary Attack")
		FName MuzzleSocketName;
	
	void AttackTimerElapsed(AActor* Instigator, const TSubclassOf<ARogueProjectileBase> ProjectileClassToSpawn);
};
