// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueGameplayFunctionLibrary.h"

#include "ActionSystem/RogueActionSystemComponent.h"

TAutoConsoleVariable<float> CVar_DamageForceMult(TEXT("game.global.DamageForceMultiplier"), 10000.f,
	TEXT("Impulse Multiplier. Overall force is: Direction * DamageAmount * ForceMultiplier"), ECVF_Cheat);

static TAutoConsoleVariable<float> CVar_ParryWindowMult(TEXT("game.parry.ParryWindowMultiplier"), 1.f,
	TEXT("Increase / Decrease Parry window."), ECVF_Cheat);


bool URogueGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	URogueActionSystemComponent* ActionSystemComponent = URogueActionSystemComponent::GetActionSystemComponent(TargetActor);
	if (ActionSystemComponent == nullptr) return false;
	
	return ActionSystemComponent->ApplyHealthChange(DamageCauser, -DamageAmount);
	
}

bool URogueGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor,
	float DamageAmount, const FHitResult& HitResult)
{
	const bool bWasDamaged = !ApplyDamage(DamageCauser, TargetActor, DamageAmount);
	
	UPrimitiveComponent* HitComp = HitResult.GetComponent();
	if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
	{
		const float ForceMultiplier = CVar_DamageForceMult.GetValueOnGameThread();
		
		FVector HitDirection = HitResult.TraceEnd - HitResult.TraceStart;
		HitDirection.Normalize();
		
		HitComp->AddImpulseAtLocation(HitDirection * DamageAmount * ForceMultiplier, HitResult.ImpactPoint, HitResult.BoneName);
	}
	
	return bWasDamaged;
}
