// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueProjectileTeleporter.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


ARogueProjectileTeleporter::ARogueProjectileTeleporter()
{
	// 0.2 seconds 
	InitialLifeSpan = 0.4f;
	// PrimaryActorTick.bCanEverTick = true;
}

void ARogueProjectileTeleporter::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TeleportTimerHandle, this, &ARogueProjectileTeleporter::StartTeleport, 0.2f);
}

void ARogueProjectileTeleporter::StartTeleport()
{
	GetWorldTimerManager().ClearTimer(TeleportTimerHandle);
	ProjectileMovementComponent->StopMovementImmediately();
	LoopedAudioComponent->Stop();
	LoopedNiagaraComponent->Deactivate();
	SetActorEnableCollision(false);  // Disable Collision for projectile completely
	
	const FVector EffectLocation = GetActorLocation();
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, TeleportPreparationEffect, EffectLocation);
	UGameplayStatics::PlaySoundAtLocation(this, TeleportPreparationSound, EffectLocation);
	// The OnDestroyed based on InitialLifeSpan will take care of things.
}

void ARogueProjectileTeleporter::OnProjectile_Destroyed_Implementation(AActor* DestroyedActor)
{
	Super::OnProjectile_Destroyed_Implementation(DestroyedActor);
	if (!GetInstigator()) return;
	// Teleport to this location.
	const FRotator TeleportRotation = FRotator(0.f, GetActorRotation().Yaw, 0.f);
	FVector TeleportLocation = GetActorLocation();
	const bool SuccessfullyTeleported = GetInstigator()->TeleportTo(TeleportLocation, TeleportRotation);
	if (!SuccessfullyTeleported)
	{
		TeleportLocation -= GetActorRotation().Vector() * TeleportAdjustment;
		GetInstigator()->TeleportTo(TeleportLocation, TeleportRotation);
	}
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionImpactSound, TeleportLocation);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect,  TeleportLocation);
}

void ARogueProjectileTeleporter::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                                           UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetWorldTimerManager().IsTimerActive(TeleportTimerHandle)) return;  // Already teleporting ignore.
	GetWorldTimerManager().ClearTimer(TeleportTimerHandle);
	// Super::OnActorHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
	OnProjectileHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
	StartTeleport();
}

