// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueProjectileBlackhole.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"


ARogueProjectileBlackhole::ARogueProjectileBlackhole()
{
	InitialLifeSpan = 5.f;
	
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	SphereComponent->SetGenerateOverlapEvents(true);
	SphereComponent->SetSphereRadius(15.f);
	
	BlackholeForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("BlackholeForceComp"));
	BlackholeForceComponent->RemoveObjectTypeToAffect(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	BlackholeForceComponent->SetupAttachment(GetRootComponent());
	BlackholeForceComponent->ForceStrength = -10000000.f;
	BlackholeForceComponent->Radius = 1000.f;
	BlackholeForceComponent->Falloff = ERadialImpulseFalloff::RIF_Linear;
	BlackholeForceComponent->bIgnoreOwningActor = true;
	// BlackholeForceComponent->RemoveObjectTypeToAffect(ObjectTypeQuery1);
	
	// PrimaryActorTick.bCanEverTick = true;
}

void ARogueProjectileBlackhole::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARogueProjectileBlackhole::OnBlackhole_OverlapObject);
	LoopedNiagaraComponent->SetWorldRotation(FRotator::ZeroRotator);
}

void ARogueProjectileBlackhole::OnBlackhole_OverlapObject(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Will only destroy simulated objects.
	if (!OtherComp->IsSimulatingPhysics()) return;
	
	const FVector EffectLocation = OtherComp->GetComponentLocation();
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ObjectCollisionDestroyEffect, EffectLocation);
	UGameplayStatics::PlaySoundAtLocation(this, ObjectCollisionDestroySound, EffectLocation);
	
	OtherActor->Destroy();
}

void ARogueProjectileBlackhole::OnProjectile_Destroyed_Implementation(AActor* DestroyedActor)
{
	Super::OnProjectile_Destroyed_Implementation(DestroyedActor);
	
}

void ARogueProjectileBlackhole::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                                          UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Super::OnActorHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}
