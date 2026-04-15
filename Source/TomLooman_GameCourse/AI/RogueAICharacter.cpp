// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "RogueAIController.h"
#include "ActionSystem/RogueActionComponent.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RogueVisualFeedbackComponent.h"
#include "Projectiles/RogueProjectileBase.h"
#include "UI/Core/RogueWorldUserWidget.h"


ARogueAICharacter::ARogueAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	AIActionSystemComponent = CreateDefaultSubobject<URogueActionSystemComponent>(TEXT("AIActionSystemComp"));
	
	ActionComponent = CreateDefaultSubobject<URogueActionComponent>(TEXT("ActionComp"));
	
	VisualFeedbackComponent = CreateDefaultSubobject<URogueVisualFeedbackComponent>(TEXT("VisualFeedbackComp"));
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	DestroyAfterDeathInSeconds = 10.f;
	
	MeshMuzzleName = "Muzzle_01";
	
	AIControllerClass = ARogueAIController::StaticClass();
	
	MaxShootingSpreadYaw = 3.5f;
	MaxShootingSpreadPitch = 3.5f;
	
	CreditsForDeath = 1;
}

void ARogueAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	// ARogueAIController* MyController = CastChecked<ARogueAIController>(GetController());
	
	VisualFeedbackComponent->InitializeVisualFeedback(GetMesh());
	
	AIActionSystemComponent->OnHealthChanged.AddDynamic(this, &ARogueAICharacter::OnAIHealthChanged);
	if (GetWorld() && GetWorld()->IsGameWorld())
	{
		MyAIController = Cast<ARogueAIController>(GetController());
		MyAIController->OnPlayerSpotted.AddDynamic(this, &ARogueAICharacter::OnPlayerSpotted);
	}
	
}

bool ARogueAICharacter::Heal(const float Amount)
{
	return AIActionSystemComponent->ApplyHealthChange(nullptr, Amount);
}

void ARogueAICharacter::ShootAnimationEvent()
{
	const FVector MuzzleLocation = GetMesh()->GetSocketLocation(MeshMuzzleName);
	FVector Direction = (CurrentShootingActor->GetActorLocation() - MuzzleLocation).GetSafeNormal();
	FRotator MuzzleRotation = Direction.Rotation();
	
	// For Pitch Randomness we will not aim downwards randomly at all because it seems dumb.
	MuzzleRotation.Pitch += FMath::RandRange(0.f, MaxShootingSpreadPitch);
	MuzzleRotation.Yaw += FMath::RandRange(-MaxShootingSpreadYaw, MaxShootingSpreadYaw);
	
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = this;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	 
	AActor* NewProjectileSpawned = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnInfo);
	
	UGameplayStatics::SpawnSoundAtLocation(this, ShootSound, MuzzleLocation);
	UNiagaraFunctionLibrary::SpawnSystemAttached(Shoot_FX, GetMesh(), MeshMuzzleName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
	
	CurrentShootingActor = nullptr;
}

bool ARogueAICharacter::Shoot(AActor* Target)
{
	if (!Target) return false;
	
	// This one makes sure it runs FAILED on the BTTask. So AI won't shoot us even when player is dead.
	if (!URogueActionSystemComponent::GetActionSystemComponent(Target)->IsAlive()) return false;
	
	PlayAnimMontage(ShootMontage);
	CurrentShootingActor = Target;
	
	return true;
}

bool ARogueAICharacter::GetActionSystemComponent_Implementation(URogueActionSystemComponent*& OutActionSystemComponent)
{
	OutActionSystemComponent = AIActionSystemComponent;
	return AIActionSystemComponent != nullptr;
}

int ARogueAICharacter::GetCreditsForDeath() const
{
	return CreditsForDeath;
}

void ARogueAICharacter::OnPlayerSpotted(ARogueAIController* AIControllerSpotter, AActor* SpottedActor)
{
	if (!SpottedWidgetClass) return;
	auto CreatedWidget = CreateWidget<URogueWorldUserWidget>(GetWorld(), SpottedWidgetClass);
	if (!CreatedWidget) return;
	
	CreatedWidget->AttachTo = this;
	CreatedWidget->AddToViewport();
}

void ARogueAICharacter::OnAIHealthChanged(AActor* InstigatorActor, URogueActionSystemComponent* OwningComp,
                                          const float NewHealth, const float OldHealth)
{
	if (FMath::IsNearlyZero(NewHealth))
	{
		// Died?
		GetMovementComponent()->StopMovementImmediately();
		
		AAIController* AIController = Cast<AAIController>(GetController());
		if (ensure(AIController))
		{
			AIController->GetBrainComponent()->StopLogic(TEXT("Death"));
			AIController->SetActorTickEnabled(false);
			
			TimeKilled = GetGameTimeSinceCreation();
			
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
			
			//ARogueGameMode* GameModeRef = CastChecked<ARogueGameMode>(UGameplayStatics::GetGameMode(this));
			//GameModeRef->EnemyDied(this);
			
			// The death animation is done in AnimBP
			
			// PlayAnimMontage(DeathMontage);  // Ragdolling is done as an event of animation.
		}
		
		return;
	}
	
	if (HealthBarWidget == nullptr)
	{
		HealthBarWidget = CreateWidget<URogueWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
		if (HealthBarWidget)
		{
			// We assign stuff before AddToViewport()
			// AddToViewport() calls Construct()
			HealthBarWidget->AttachTo = this;
			HealthBarWidget->AddToViewport();
		}
	}
	
	
	const float DeltaHealth = NewHealth - OldHealth;
	if (FMath::IsNegativeOrNegativeZero(DeltaHealth))
	{
		// AI was damaged.
		
		AActor* DamageCauser = InstigatorActor;
		if (AController* ControllerInstigator = Cast<AController>(InstigatorActor))
		{
			DamageCauser = ControllerInstigator->GetPawn();
		}
		
		if (DamageCauser)
		{
			MyAIController->SetTargetActor(DamageCauser);
		}
		
		if (AIActionSystemComponent->IsHeavyDamage(DeltaHealth))
		{
			// Heavy Damage
			VisualFeedbackComponent->VisualFeedback(ERogueVisualFeedbackType::DAMAGED_HEAVY);
		}
		else
		{
			// Light Damage
			VisualFeedbackComponent->VisualFeedback(ERogueVisualFeedbackType::DAMAGED_LIGHT);
		}
	}
	else
	{
		// AI was healed.
		VisualFeedbackComponent->VisualFeedback(ERogueVisualFeedbackType::HEALED);
	}
}

void ARogueAICharacter::OnDeathAnimationEvent()
{
	// Start Ragdolling and destroy itself after X seconds.
	
	SetLifeSpan(DestroyAfterDeathInSeconds);
}

bool ARogueAICharacter::GetActionComponent_Implementation(URogueActionComponent*& OutActionComponent)
{
	OutActionComponent = ActionComponent;
	return ActionComponent != nullptr;
}
