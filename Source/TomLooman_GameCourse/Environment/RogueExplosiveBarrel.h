// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/RogueActionSystemInterface.h"
#include "GameFramework/Actor.h"
#include "RogueExplosiveBarrel.generated.h"

class URogueActionSystemComponent;
class UNiagaraSystem;
class URadialForceComponent;
class UAudioComponent;

UCLASS()
class TOMLOOMAN_GAMECOURSE_API ARogueExplosiveBarrel : public AActor, public IRogueActionSystemInterface
{
	GENERATED_BODY()

public:
	ARogueExplosiveBarrel();
	
	virtual bool GetActionSystemComponent_Implementation(URogueActionSystemComponent*& OutActionSystemComponent) override;

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		TObjectPtr<URogueActionSystemComponent> ActionSystemComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		TObjectPtr<UStaticMeshComponent> BarrelMeshComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		TObjectPtr<URadialForceComponent> ExplosionRadialForceComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		TObjectPtr<UAudioComponent> FireTrailSoundComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		TObjectPtr<UNiagaraSystem> ExplosionEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		TObjectPtr<UNiagaraSystem> FireTrailEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		float LaunchExplosionTimer = 2.f;
	
	// After LaunchExplosionTimer, how long we wait for Destroy() and actual explosion.
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		float ExplosionTimer = 4.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		TObjectPtr<USoundBase> ExplosionSound;
	
	UFUNCTION(BlueprintCallable, Category = "Explosion")
	void Explode();
	
	// After launched into air.
	UFUNCTION()
		void Exploded(AActor* DestroyedActor);
	
	void DestroyBarrel();
	
	void StartExplosion(const float ActualDamage);
	
	FTimerHandle ExplosionTimerHandle;
	FTimerHandle DestroyTimerHandle;
	
	virtual void PostInitializeComponents() override;
	
	UFUNCTION()
		void OnHealthChanged(AActor* InstigatorActor, URogueActionSystemComponent* OwningComp, const float NewHealth, const float OldHealth);
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
