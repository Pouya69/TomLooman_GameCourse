// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueProjectileBase.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ARogueProjectileBase::ARogueProjectileBase()
{
	InitialLifeSpan = 5.f;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComponent);
	SphereComponent->SetSphereRadius(16.f);
	SphereComponent->SetCollisionProfileName(TEXT("Projectile"));
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMoveComp"));
	ProjectileMovementComponent->InitialSpeed = 2000.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	
	LoopedNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LoopedNiagaraComp"));
	LoopedNiagaraComponent->SetupAttachment(SphereComponent);
	
	LoopedAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("LoopedAudioComp"));
	LoopedAudioComponent->SetupAttachment(GetRootComponent());
	
	//PrimaryActorTick.bCanEverTick = true;
}

void ARogueProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentHit.AddDynamic(this, &ARogueProjectileBase::OnActorHit);
	OnDestroyed.AddDynamic(this, &ARogueProjectileBase::OnProjectile_Destroyed);
	
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ARogueProjectileBase::OnProjectile_Destroyed_Implementation(AActor* DestroyedActor)
{
}

void ARogueProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                                     UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ExplosionEffect)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffect, GetActorLocation());
	
	if (ExplosionImpactSound)
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionImpactSound, Hit.ImpactPoint);
	
	OnProjectileHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
	
	Destroy();
}

