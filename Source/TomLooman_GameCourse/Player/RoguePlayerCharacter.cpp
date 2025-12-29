// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TomLooman_GameCourse/Projectiles/RogueProjectileMagic.h"
#include "TomLooman_GameCourse/Projectiles/RogueProjectileBlackhole.h"
#include "TomLooman_GameCourse/Projectiles/RogueProjectileTeleporter.h"

// Sets default values
ARoguePlayerCharacter::ARoguePlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// bUseControllerRotationYaw = false;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Comp"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Comp"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	MuzzleSocketName = FName("Muzzle_01");
}

// Called when the game starts or when spawned
void ARoguePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
	}

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
	PlayAnimMontage(AttackMontage);
	UNiagaraFunctionLibrary::SpawnSystemAttached(MagicCastingEffect, GetMesh(), MuzzleSocketName,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
	UGameplayStatics::PlaySound2D(this, MagicCastingSound);
	
	FTimerHandle AttackTimerHandle;
	const float AttackTimer = 0.2f;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]()
	{
		AttackTimerElapsed(MagicProjectileClass, GetControlRotation());
	});
	GetWorldTimerManager().SetTimer(AttackTimerHandle, TimerDelegate, AttackTimer, false);	
	
}

void ARoguePlayerCharacter::SecondaryAttack()
{
	PlayAnimMontage(AttackMontage);
	if (TeleporterCastingEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(TeleporterCastingEffect, GetMesh(), MuzzleSocketName,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
	}
	if (TeleporterCastingSound)
		UGameplayStatics::PlaySound2D(this, TeleporterCastingSound);
	
	FTimerHandle AttackTimerHandle;
	const float AttackTimer = 0.2f;
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]()
	{
		AttackTimerElapsed(TeleporterProjectileClass, GetControlRotation());
	});
	GetWorldTimerManager().SetTimer(AttackTimerHandle, TimerDelegate, AttackTimer, false);
}

void ARoguePlayerCharacter::BlackholeAttack()
{
	PlayAnimMontage(AttackMontage);
	if (BlackholeCastingEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(BlackholeCastingEffect, GetMesh(), MuzzleSocketName,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
	}
	if (BlackholeCastingSound)
		UGameplayStatics::PlaySound2D(this, BlackholeCastingSound);
	
	FTimerHandle AttackTimerHandle;
	const float AttackTimer = 0.3f;
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]()
	{
		AttackTimerElapsed(BlackholeProjectileClass, GetControlRotation());
	});
	GetWorldTimerManager().SetTimer(AttackTimerHandle, TimerDelegate, AttackTimer, false);
}

void ARoguePlayerCharacter::AttackTimerElapsed(const TSubclassOf<ARogueProjectileBase> ProjectileClassToSpawn, const FRotator& SpawnRotation)
{
	const FVector SpawnLocation = GetMesh()->GetSocketLocation(MuzzleSocketName);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* SpawnedProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClassToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
	MoveIgnoreActorAdd(SpawnedProjectile);
}

void ARoguePlayerCharacter::Jump()
{
	// For now, we just do the normal jump.
	Super::Jump();
}

// Called every frame
void ARoguePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
