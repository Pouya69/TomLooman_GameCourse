// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RogueProjectileMagic.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraComponent;

UCLASS()
class TOMLOOMAN_GAMECOURSE_API ARogueProjectileMagic : public AActor
{
	GENERATED_BODY()

public:
	ARogueProjectileMagic();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Components")
		TObjectPtr<USphereComponent> SphereComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
		TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
		TObjectPtr<UNiagaraComponent> LoopedNiagaraComponent;
};
