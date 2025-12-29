// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueProjectileBase.h"
#include "RogueProjectileBlackhole.generated.h"

class URadialForceComponent;
class UNiagaraSystem;

UCLASS(Abstract)
class TOMLOOMAN_GAMECOURSE_API ARogueProjectileBlackhole : public ARogueProjectileBase
{
	GENERATED_BODY()

public:
	ARogueProjectileBlackhole();
	
	// virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		TObjectPtr<URadialForceComponent> BlackholeForceComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackhole")
		TObjectPtr<UNiagaraSystem> ObjectCollisionDestroyEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackhole")
		TObjectPtr<USoundBase> ObjectCollisionDestroySound;
	
	virtual void PostInitializeComponents() override;
	
	UFUNCTION()
		void OnBlackhole_OverlapObject(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	virtual void OnProjectile_Destroyed_Implementation(AActor* DestroyedActor) override;
	
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
