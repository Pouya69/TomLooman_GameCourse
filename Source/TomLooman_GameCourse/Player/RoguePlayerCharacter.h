// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/RogueActionSystemInterface.h"
#include "GameFramework/Character.h"
#include "RoguePlayerCharacter.generated.h"


class URogueActionComponent;
class URogueVisualFeedbackComponent;
class ARogueProjectileBlackhole;
class ARogueProjectileBase;
class ARogueProjectileTeleporter;
class UNiagaraSystem;
struct FInputActionInstance;
struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class ARogueProjectileMagic;
class UAnimMontage;
class URogueActionSystemComponent;
class UUserWidget;


UCLASS()
class TOMLOOMAN_GAMECOURSE_API ARoguePlayerCharacter : public ACharacter, public IRogueActionSystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARoguePlayerCharacter();

protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Death")
		TObjectPtr<UAnimMontage> DeathMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		TObjectPtr<UInputAction> Input_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
		TObjectPtr<UInputAction> Input_Look;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		TObjectPtr<UInputAction> Input_PrimaryAttack;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		TObjectPtr<UInputAction> Input_SecondaryAttack;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		TObjectPtr<UInputAction> Input_BlackholeAttack;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		TObjectPtr<UInputAction> Input_Jump;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		TObjectPtr<UInputAction> Input_Sprint;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		TObjectPtr<UInputAction> Input_Parry;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
		TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		TObjectPtr<URogueActionSystemComponent> ActionSystemComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		TObjectPtr<URogueVisualFeedbackComponent> PlayerVisualFeedbackComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		TObjectPtr<URogueActionComponent> ActionComponent;
	
	virtual FVector GetPawnViewLocation() const override;
	
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionInstance& Value);
	void PrimaryAttack();
	void SecondaryAttack();
	void BlackholeAttack();
	virtual void Jump() override;
	void StartSprint();
	void StopSprint();
	void Parry();
	
	UFUNCTION()
		void OnPlayerHealthChanged(AActor* InstigatorActor, URogueActionSystemComponent* OwningComp, const float NewHealth, const float OldHealth);
	
	virtual bool GetActionComponent_Implementation(URogueActionComponent*& OutActionComponent) override;
	
public:	
		
	virtual void PostInitializeComponents() override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable, Category="Health")
		float GetHealth() const;
	
	virtual bool GetActionSystemComponent_Implementation(URogueActionSystemComponent*& OutActionSystemComponent) override;
	
	// Debug function.
	UFUNCTION(Exec)
		void HealSelf(const float Amount = 100.0f);
};
