// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"

// Sets default values
ARogueCharacter::ARogueCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// bUseControllerRotationYaw = false;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Comp"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Comp"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ARogueCharacter::Move(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.f;
	FVector MovementDirection = FVector(InputValue, 0.f);

	AddMovementInput(ControlRot.Vector(), InputValue.X);  // Forward and Back
	// Same as how you used KismetMathLibrary to transform the rotation.
	// But easier.
	AddMovementInput(ControlRot.RotateVector(FVector::RightVector), InputValue.Y);  // Right and Left
}

void ARogueCharacter::Look(const FInputActionInstance& Value)
{
	FVector2D InputValue = Value.GetValue().Get<FVector2D>();
	AddControllerYawInput(InputValue.X);
	AddControllerPitchInput(InputValue.Y);
}

// Called when the game starts or when spawned
void ARogueCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARogueCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARogueCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInput->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ARogueCharacter::Move);
		EnhancedInput->BindAction(Input_Look, ETriggerEvent::Triggered, this, &ARogueCharacter::Look);
	}

}
