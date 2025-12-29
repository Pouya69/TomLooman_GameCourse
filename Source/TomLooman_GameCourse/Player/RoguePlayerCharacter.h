// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RoguePlayerCharacter.generated.h"


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


UCLASS()
class TOMLOOMAN_GAMECOURSE_API ARoguePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARoguePlayerCharacter();

protected:
		
	UPROPERTY(EditDefaultsOnly, Category = "Primary Attack")
		TSubclassOf<ARogueProjectileMagic> MagicProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Primary Attack")
		TObjectPtr<UNiagaraSystem> MagicCastingEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Primary Attack")
		TObjectPtr<USoundBase> MagicCastingSound;
	
	UPROPERTY(VisibleAnywhere, Category = "Attack")
		FName MuzzleSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category="PrimaryAttack")
		TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category="Secondary Attack")
		TSubclassOf<ARogueProjectileTeleporter> TeleporterProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Secondary Attack")
		TObjectPtr<UNiagaraSystem> TeleporterCastingEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Secondary Attack")
		TObjectPtr<USoundBase> TeleporterCastingSound;
	
	UPROPERTY(EditDefaultsOnly, Category="Blackhole Attack")
		TSubclassOf<ARogueProjectileBlackhole> BlackholeProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackhole Attack")
		TObjectPtr<UNiagaraSystem> BlackholeCastingEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackhole Attack")
		TObjectPtr<USoundBase> BlackholeCastingSound;
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
		TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		TObjectPtr<USpringArmComponent> SpringArmComponent;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionInstance& Value);
	void PrimaryAttack();
	void SecondaryAttack();
	void BlackholeAttack();
	void AttackTimerElapsed(const TSubclassOf<ARogueProjectileBase> ProjectileClassToSpawn, const FRotator& SpawnRotation = FRotator::ZeroRotator);
	virtual void Jump() override;

	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
