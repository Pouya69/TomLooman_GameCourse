// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueExplosiveBarrel.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values
ARogueExplosiveBarrel::ARogueExplosiveBarrel()
{
	// PrimaryActorTick.bCanEverTick = true;
	
	BarrelMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMeshComp"));
	SetRootComponent(BarrelMeshComponent);
	BarrelMeshComponent->SetSimulatePhysics(true);
	BarrelMeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
	
	ExplosionRadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("ExplosionRadialForceComp"));
	ExplosionRadialForceComponent->SetupAttachment(BarrelMeshComponent);
	ExplosionRadialForceComponent->ForceStrength = 80000.f;
	ExplosionRadialForceComponent->ImpulseStrength = 80000.f;
	ExplosionRadialForceComponent->Radius = 250.f;
	ExplosionRadialForceComponent->bAutoActivate = false;
	ExplosionRadialForceComponent->bIgnoreOwningActor = true;
	
	FireTrailSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FireTrailSoundComp"));
	FireTrailSoundComponent->SetupAttachment(BarrelMeshComponent);
	FireTrailSoundComponent->bAutoActivate = false;
}

void ARogueExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	OnDestroyed.AddDynamic(this, &ARogueExplosiveBarrel::Exploded);
	
}


// Called every frame
void ARogueExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARogueExplosiveBarrel::Explode()
{
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, ExplosionRadialForceComponent->GetComponentLocation());
	
	BarrelMeshComponent->AddImpulse(BarrelMeshComponent->GetUpVector() * 1000.f, NAME_None, true);
	BarrelMeshComponent->AddAngularImpulseInDegrees(BarrelMeshComponent->GetRightVector() * 1000.f, NAME_None, true);
	
	FTimerHandle DestroyTimerHandle;
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ARogueExplosiveBarrel::DestroyBarrel, ExplosionTimer);
}

void ARogueExplosiveBarrel::Exploded(AActor* DestroyedActor)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffect, ExplosionRadialForceComponent->GetComponentLocation());
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, ExplosionRadialForceComponent->GetComponentLocation(),
		FRotator::ZeroRotator, 1.8f);

	ExplosionRadialForceComponent->Activate();
	ExplosionRadialForceComponent->FireImpulse();
}

void ARogueExplosiveBarrel::DestroyBarrel()
{
	Destroy();
}

float ARogueExplosiveBarrel::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                        class AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (GetWorldTimerManager().IsTimerActive(ExplosionTimerHandle)) return ActualDamage;
	UNiagaraFunctionLibrary::SpawnSystemAttached(FireTrailEffect, BarrelMeshComponent, NAME_None, FVector::ZeroVector,
		 FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
	// UGameplayStatics::SpawnSoundAttached(FireTrailSound);
	FireTrailSoundComponent->Activate();
	
	GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &ARogueExplosiveBarrel::Explode, LaunchExplosionTimer);
	
	return ActualDamage;
}
