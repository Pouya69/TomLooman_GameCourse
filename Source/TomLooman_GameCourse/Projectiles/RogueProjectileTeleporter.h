// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueProjectileBase.h"
#include "RogueProjectileTeleporter.generated.h"

class UNiagaraSystem;
class USoundBase;

UCLASS(Abstract)
class TOMLOOMAN_GAMECOURSE_API ARogueProjectileTeleporter : public ARogueProjectileBase
{
	GENERATED_BODY()

public:
	ARogueProjectileTeleporter();
	
protected:
	virtual void BeginPlay() override;
	
	// Just in case we cannot teleport on first time (Eg. Landscape collision)
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
		float TeleportAdjustment = 80.f;
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
		TObjectPtr<UNiagaraSystem> TeleportPreparationEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
		TObjectPtr<USoundBase> TeleportPreparationSound;
	
	FTimerHandle TeleportTimerHandle;
	void StartTeleport();
	
	virtual void OnProjectile_Destroyed_Implementation(AActor* DestroyedActor) override;
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
