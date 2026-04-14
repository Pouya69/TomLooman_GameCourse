// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/RogueInteractionInterface.h"
#include "GameFramework/Actor.h"
#include "RoguePowerupBase.generated.h"


class USoundBase;
class ACharacter;

UENUM(BlueprintType)
enum class EPowerUpUsageType : uint8
{
	ON_COLLISION = 0,
	INTERACT,
};

UCLASS(Abstract)
class TOMLOOMAN_GAMECOURSE_API ARoguePowerupBase : public AActor, public IRogueInteractionInterface
{
	GENERATED_BODY()

public:
	ARoguePowerupBase();
	
	UFUNCTION(BlueprintCallable, Category="Power Up")
		virtual void PowerUp(ACharacter* CharacterInteracting);
	
	UFUNCTION(BlueprintCallable, Category="Cooldown")
		bool IsOnCooldown() const;
	
	// Starts cooldown timer IF CooldownTime > 0 by default.
	UFUNCTION(BlueprintCallable, Category="Cooldown")
		virtual void StartCooldown();
	
	virtual void OnCooldown_Implementation() override;
	
	virtual void CannotInteract_Implementation() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Power Up")
		EPowerUpUsageType PowerUpUsageType = EPowerUpUsageType::ON_COLLISION;
	
	virtual void PostInitializeComponents() override;
	
	virtual void Interact_Implementation(AActor* CharacterInteracting) override;
	
	UPROPERTY(BlueprintReadOnly, Category="Cooldown")
		FTimerHandle CooldownTimer;
	
	UPROPERTY(EditDefaultsOnly, Category="Power Up")
		TObjectPtr<USoundBase> PowerUpSound;
		
	// If true, MID will be created and assigned.
	UPROPERTY(EditDefaultsOnly, Category="Materials")
		bool bIsDynamic;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Materials")
		TObjectPtr<UMaterialInstanceDynamic> Mesh_MID;
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
		TObjectPtr<UStaticMeshComponent> MeshComponent;
	
	// This could vary based on the Power Up. The added stamina, the added health, etc.
	UPROPERTY(EditAnywhere, Category="Power Up")
		float Amount;
	
	// If < 0, no cooldown.
	UPROPERTY(EditAnywhere, Category="Cooldown")
		float CooldownTime;
	
	UPROPERTY(EditDefaultsOnly, Category="Cooldown")
		TObjectPtr<USoundBase> OnCooldownSound;
	
	UPROPERTY(EditDefaultsOnly, Category="Power Up")
		TObjectPtr<USoundBase> CannotPowerUpSound;
	
};
