// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "RogueVisualFeedbackComponent.h"
#include "ActionSystem/RogueActionComponent.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ARoguePlayerCharacter::ARoguePlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActionSystemComponent = CreateDefaultSubobject<URogueActionSystemComponent>(TEXT("RogueActionSystemComp"));
	
	// bUseControllerRotationYaw = false;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Comp"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Comp"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	PlayerVisualFeedbackComponent = CreateDefaultSubobject<URogueVisualFeedbackComponent>(TEXT("VisualFeedbackComp"));
	
	ActionComponent = CreateDefaultSubobject<URogueActionComponent>(TEXT("Action Component"));
}

void ARoguePlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ActionSystemComponent->OnHealthChanged.AddDynamic(this, &ARoguePlayerCharacter::OnPlayerHealthChanged);
	
	PlayerVisualFeedbackComponent->InitializeVisualFeedback(GetMesh());
	
}

// Called to bind functionality to input
void ARoguePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInput->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::Move);
		EnhancedInput->BindAction(Input_Look, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::Look);
		EnhancedInput->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::PrimaryAttack);
		EnhancedInput->BindAction(Input_SecondaryAttack, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::SecondaryAttack);
		EnhancedInput->BindAction(Input_Jump, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::Jump);
		EnhancedInput->BindAction(Input_BlackholeAttack, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::BlackholeAttack);
		EnhancedInput->BindAction(Input_Sprint, ETriggerEvent::Started, this, &ARoguePlayerCharacter::StartSprint);
		EnhancedInput->BindAction(Input_Sprint, ETriggerEvent::Completed, this, &ARoguePlayerCharacter::StopSprint);
		EnhancedInput->BindAction(Input_Parry, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::Parry);
	}

}

float ARoguePlayerCharacter::GetHealth() const
{
	return ActionSystemComponent->GetHealth();
}

bool ARoguePlayerCharacter::GetActionSystemComponent_Implementation(URogueActionSystemComponent*& OutActionSystemComponent)
{
	OutActionSystemComponent = ActionSystemComponent;
	return ActionSystemComponent != nullptr;
}

void ARoguePlayerCharacter::HealSelf(const float Amount)
{
	ActionSystemComponent->ApplyHealthChange(nullptr, Amount);
}

FVector ARoguePlayerCharacter::GetPawnViewLocation() const
{
	return CameraComponent->GetComponentLocation();
}

void ARoguePlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D InputValue = Value.Get<FVector2D>();
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.f;

	AddMovementInput(ControlRot.Vector(), InputValue.X);  // Forward and Back
	// Same as how you used KismetMathLibrary to transform the rotation.
	// But easier.
	AddMovementInput(ControlRot.RotateVector(FVector::RightVector), InputValue.Y);  // Right and Left
}

void ARoguePlayerCharacter::Look(const FInputActionInstance& Value)
{
	FVector2D InputValue = Value.GetValue().Get<FVector2D>();
	AddControllerYawInput(InputValue.X);
	AddControllerPitchInput(InputValue.Y);
}

void ARoguePlayerCharacter::PrimaryAttack()
{
	ActionComponent->StartActionByName(this, "Projectile Attack");	
}

void ARoguePlayerCharacter::SecondaryAttack()
{
	ActionComponent->StartActionByName(this, "Teleport Attack");
}

void ARoguePlayerCharacter::BlackholeAttack()
{
	ActionComponent->StartActionByName(this, "Blackhole Attack");
}

void ARoguePlayerCharacter::Jump()
{
	// Jump off the wall if in air or jump normally
	
	if (GetCharacterMovement()->IsFalling())
	{
		// Check for wall
		FHitResult HitResult;
		const FVector Start = GetActorLocation();
		const FCollisionShape CollisionShape = FCollisionShape::MakeSphere(50.f);
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);
		const bool bIsNextToWall = GetWorld()->SweepSingleByChannel(HitResult, Start, Start, FQuat::Identity, ECC_WorldStatic, CollisionShape, CollisionParams);
		if (!bIsNextToWall)
			return;
		
		const FVector LaunchVelocity = (HitResult.ImpactNormal * 800.f) + FVector(0, 0, 500.f);
		LaunchCharacter(LaunchVelocity, true, true);
		return;
	}
	
	Super::Jump();
}

void ARoguePlayerCharacter::StartSprint()
{
	ActionComponent->StartActionByName(this, "Sprint");
}

void ARoguePlayerCharacter::StopSprint()
{
	ActionComponent->StopActionByName(this, "Sprint");
}

void ARoguePlayerCharacter::Parry()
{
	ActionComponent->StartActionByName(this, "Parry");
}

void ARoguePlayerCharacter::OnPlayerHealthChanged(AActor* InstigatorActor, URogueActionSystemComponent* OwningComp, const float NewHealth, const float OldHealth)
{
	if (FMath::IsNearlyZero(NewHealth))
	{
		// Died?
		// Accepts null. But only works for this character not controller.
		DisableInput(nullptr);
		GetCharacterMovement()->StopMovementImmediately();
		
		PlayAnimMontage(DeathMontage);
		return;
	}
	
	const float DeltaHealth = NewHealth - OldHealth;
	if (FMath::IsNegativeOrNegativeZero(DeltaHealth))
	{
		// Player was damaged.
		if (ActionSystemComponent->IsHeavyDamage(-DeltaHealth))
		{
			// Heavy Damage
			PlayerVisualFeedbackComponent->VisualFeedback(ERogueVisualFeedbackType::DAMAGED_HEAVY);
		}
		else
		{
			// Light Damage
			PlayerVisualFeedbackComponent->VisualFeedback(ERogueVisualFeedbackType::DAMAGED_LIGHT);
		}
		
		OwningComp->ApplyRageChange(-DeltaHealth);
	}
	else
	{
		// Player was healed.
		PlayerVisualFeedbackComponent->VisualFeedback(ERogueVisualFeedbackType::HEALED);
	}
}

bool ARoguePlayerCharacter::GetActionComponent_Implementation(URogueActionComponent*& OutActionComponent)
{
	OutActionComponent = ActionComponent;
	return ActionComponent != nullptr;
}
