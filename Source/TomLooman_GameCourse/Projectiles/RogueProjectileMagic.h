// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "RogueProjectileBase.h"
#include "RogueProjectileMagic.generated.h"

class URogueActionEffect;
class UCameraShakeBase;

UCLASS(Abstract)
class TOMLOOMAN_GAMECOURSE_API ARogueProjectileMagic : public ARogueProjectileBase
{
	GENERATED_BODY()
public:
	ARogueProjectileMagic();
	
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void PostInitializeComponents() override;

protected:
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
	UPROPERTY(EditDefaultsOnly, Category="Tags")
		FGameplayTag ParryTag;
	
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UCameraShakeBase> ImpactCameraShake;
	
	UPROPERTY(EditDefaultsOnly, Category="Damage")
		TSubclassOf<URogueActionEffect> BurningActionEffectClass;
};
