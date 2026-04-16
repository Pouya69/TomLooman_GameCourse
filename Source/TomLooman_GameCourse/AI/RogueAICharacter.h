// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/RogueActionSystemInterface.h"
#include "GameFramework/Character.h"
#include "RogueAICharacter.generated.h"

class URogueWorldUserWidget;
class ARogueProjectileBase;
class ARogueAIController;
class UAIPerceptionComponent;
class URogueVisualFeedbackComponent;
class URogueActionSystemComponent;
class USoundBase;
class UNiagaraSystem;
class URogueActionComponent;

UCLASS()
class TOMLOOMAN_GAMECOURSE_API ARogueAICharacter : public ACharacter, public IRogueActionSystemInterface
{
	GENERATED_BODY()

public:
	ARogueAICharacter();
	
	//UFUNCTION(BlueprintCallable, Category="Health")
		//float GetHealth() const;
	
	//UFUNCTION(BlueprintCallable, Category="Health")
		//bool IsAlive() const;
	
	UFUNCTION(BlueprintCallable, Category="Health")
		bool Heal(const float Amount);
	
	// Gets set to GetGameTimeSinceCreation() when character dies.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Death")
		float TimeKilled;
	
	UFUNCTION(BlueprintCallable, Category = "Shoot")
		bool Shoot(AActor* Target);
	
	virtual bool GetActionSystemComponent_Implementation(URogueActionSystemComponent*& OutActionSystemComponent) override;
	
	UFUNCTION(BlueprintCallable, Category = "Death")
		int GetCreditsForDeath() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<URogueWorldUserWidget> SpottedWidgetClass;
	
	UFUNCTION()
		void OnPlayerSpotted(ARogueAIController* AIControllerSpotter, AActor* SpottedActor);
	
	virtual void PostInitializeComponents() override;
	
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly)
		TObjectPtr<ARogueAIController> MyAIController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		TObjectPtr<URogueActionSystemComponent> AIActionSystemComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		TObjectPtr<URogueActionComponent> ActionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		TObjectPtr<URogueVisualFeedbackComponent> VisualFeedbackComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Death")
		UAnimMontage* DeathMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
		UAnimMontage* ShootMontage;
	
	UFUNCTION(BlueprintCallable, Category = "Shoot")
		void ShootAnimationEvent();
	
	UPROPERTY(EditDefaultsOnly, Category="Shoot")
		FName MeshMuzzleName;
	
	UPROPERTY(EditDefaultsOnly, Category="Shoot")
		TSubclassOf<ARogueProjectileBase> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Shoot")
		TObjectPtr<USoundBase> ShootSound;
	
	UPROPERTY(EditDefaultsOnly, Category="Shoot")
		TObjectPtr<UNiagaraSystem> Shoot_FX;
	
	UPROPERTY(EditDefaultsOnly, Category="Shoot | Accuracy")
		float MaxShootingSpreadPitch;
	
	UPROPERTY(EditDefaultsOnly, Category="Shoot | Accuracy")
		float MaxShootingSpreadYaw;
	
	UPROPERTY(BlueprintReadOnly, Category="Shoot")
		AActor* CurrentShootingActor;
	
	UFUNCTION()
		void OnAIHealthChanged(AActor* InstigatorActor, URogueActionSystemComponent* OwningComp, const float NewHealth, const float OldHealth);
	
	UFUNCTION(BlueprintCallable, Category="Death")
		void OnDeathAnimationEvent();
	
	UPROPERTY(EditDefaultsOnly, Category="Health")
		TSubclassOf<URogueWorldUserWidget> HealthBarWidgetClass;
	
	UPROPERTY(BlueprintReadWrite, Category="Health")
		URogueWorldUserWidget* HealthBarWidget;
	
	// How long before Destroy() gets called for this.
	UPROPERTY(EditDefaultsOnly, Category = "Death")
		float DestroyAfterDeathInSeconds;
	
	UPROPERTY(EditDefaultsOnly, Category = "Death")
		int CreditsForDeath;
	
	virtual bool GetActionComponent_Implementation(URogueActionComponent*& OutActionComponent) override;
};
