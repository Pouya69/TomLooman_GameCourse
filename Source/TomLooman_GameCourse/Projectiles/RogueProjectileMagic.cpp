// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueProjectileMagic.h"

#include "ActionSystem/RogueActionSystemInterface.h"
#include "Core/RogueGameplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ActionSystem/RogueActionComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ARogueProjectileMagic::ARogueProjectileMagic()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARogueProjectileMagic::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// UE_LOG(LogTemp, Log, TEXT("%s"), *ProjectileMovementComponent->Velocity.ToCompactString());
}

void ARogueProjectileMagic::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

void ARogueProjectileMagic::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == GetInstigator()) return;
	
	URogueActionComponent* ActionComponent = URogueActionComponent::GetActionComponent(OtherActor);
	if (ActionComponent && ActionComponent->ActiveGameplayTags.HasTag(ParryTag))
	{
		FTimerDelegate TimerDelegate;
						
		// Reflect the projectile upon Parry.
		SphereComponent->IgnoreActorWhenMoving(GetInstigator(), false);
		GetInstigator()->MoveIgnoreActorRemove(this);
				
		APawn* Pawn = Cast<APawn>(OtherActor);
		SetInstigator(Pawn);
		SphereComponent->IgnoreActorWhenMoving(Pawn, true);
		Pawn->MoveIgnoreActorAdd(this);
			
		const FVector Direction = -ProjectileMovementComponent->Velocity.GetSafeNormal();
			
		FTransform NewTransform = GetActorTransform();
		NewTransform.SetTranslation(NewTransform.GetLocation() + Direction * 5.0f);
		NewTransform.SetRotation(Pawn->GetControlRotation().Quaternion());
		SetActorTransform(NewTransform, false, nullptr, ETeleportType::TeleportPhysics);
			
		// SphereComponent->SetSimulatePhysics(true);
		SphereComponent->WakeRigidBody();
			
		ProjectileMovementComponent->bSimulationEnabled = true;
		ProjectileMovementComponent->Velocity = ProjectileMovementComponent->InitialSpeed * Direction;
		// UE_LOG(LogTemp, Log, TEXT("%s"), *ProjectileMovementComponent->Velocity.ToCompactString());
		ProjectileMovementComponent->UpdateComponentVelocity();

		return;
	}
	
	URogueGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, 10.f, Hit);
	if (ImpactCameraShake)
		UGameplayStatics::PlayWorldCameraShake(this, ImpactCameraShake, Hit.ImpactPoint, 50.f, 5000.f);
	
	// We moved Super:: to here because of Destroy()
	Super::OnActorHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}
