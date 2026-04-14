// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueActionTeleportAttack.h"

#include "NiagaraFunctionLibrary.h"
#include "RogueGameTypes.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/RogueProjectileTeleporter.h"

URogueActionTeleportAttack::URogueActionTeleportAttack()
{
	ActionName = "Teleport Attack";
	MuzzleSocketName = "Muzzle_01";
}

void URogueActionTeleportAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	
	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (!Character) return;
	
	if (TeleporterCastingEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(TeleporterCastingEffect, Character->GetMesh(), MuzzleSocketName,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
	}
	if (TeleporterCastingSound)
		UGameplayStatics::PlaySound2D(this, TeleporterCastingSound);
	
	FTimerHandle AttackTimerHandle;
	const float AttackTimer = 0.2f;
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&, Instigator]()
	{
		AttackTimerElapsed(Instigator, TeleporterProjectileClass);
	});
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, TimerDelegate, AttackTimer, false);
}

void URogueActionTeleportAttack::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
}

void URogueActionTeleportAttack::AttackTimerElapsed(AActor* Instigator,
	const TSubclassOf<ARogueProjectileBase> ProjectileClassToSpawn)
{
	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (!Character) return;
	
	Character->PlayAnimMontage(AttackMontage);
	
	FHitResult HitResult;
	const float SphereRadius = 20.f;
	const FVector CameraForward = Character->GetControlRotation().Vector();
	// We move the start a bit forward to avoid collision with objects close to camera.
	const FVector Start = Character->GetPawnViewLocation() + (CameraForward * 40.0f);
	const FVector End = Start + (CameraForward * 1000000.f);
	// We do sweep for some aim assist ;D
	bool bHasHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, COLLISION_PROJECTILE);
	if (!bHasHit)
	{
		bHasHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End,
		FQuat::Identity, COLLISION_PROJECTILE, FCollisionShape::MakeSphere(SphereRadius));
	}
	const FVector SpawnLocation = Character->GetMesh()->GetSocketLocation(MuzzleSocketName);
	// Same as the below one.
	const FRotator SpawnRotation = FRotationMatrix::MakeFromX(bHasHit ? (HitResult.ImpactPoint - SpawnLocation) : (End - SpawnLocation)).Rotator();
	// const FRotator SpawnRotation = (bHasHit ? (HitResult.ImpactPoint - SpawnLocation) : (End - SpawnLocation)).GetSafeNormal().Rotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = Character;
	SpawnParams.Owner = Character;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* SpawnedProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClassToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
	
	Character->MoveIgnoreActorAdd(SpawnedProjectile);
	
	StopAction(Instigator);
}
