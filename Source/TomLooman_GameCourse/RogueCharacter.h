// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RogueCharacter.generated.h"


struct FInputActionInstance;
struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;


UCLASS()
class TOMLOOMAN_GAMECOURSE_API ARogueCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARogueCharacter();

protected:
	// Called when the game starts or when spawned
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		TObjectPtr<UInputAction> Input_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
		TObjectPtr<UInputAction> Input_Look;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
		TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		TObjectPtr<USpringArmComponent> SpringArmComponent;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionInstance& Value);

	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
