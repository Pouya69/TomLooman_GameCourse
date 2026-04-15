// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueThornEffect.h"

#include "ActionSystem/RogueActionComponent.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "Core/RogueGameplayFunctionLibrary.h"

URogueThornEffect::URogueThornEffect()
{
	DamagePercentageToApplyToAttacker = 0.2f;
	
	Duration = 0.0f;
	Period = 0.0f;
}

void URogueThornEffect::StartAction_Implementation(AActor* Instigator)
{
	// We still need Super for the GameplayTags
	Super::StartAction_Implementation(Instigator);
	
	if (const auto OwningComp = GetOwningComponent())
	{
		const auto ActionSystemComponent = URogueActionSystemComponent::GetActionSystemComponent(OwningComp->GetOwner());
		ActionSystemComponent->OnHealthChanged.AddDynamic(this, &URogueThornEffect::OnHealthChanged);
	}
}

void URogueThornEffect::StopAction_Implementation(AActor* Instigator)
{
	// We still need Super for the GameplayTags
	Super::StopAction_Implementation(Instigator);
	
	if (const auto OwningComp = GetOwningComponent())
	{
		const auto ActionSystemComponent = URogueActionSystemComponent::GetActionSystemComponent(OwningComp->GetOwner());
		ActionSystemComponent->OnHealthChanged.RemoveDynamic(this, &URogueThornEffect::OnHealthChanged);
	}
}

void URogueThornEffect::OnHealthChanged(AActor* InstigatorActor, URogueActionSystemComponent* OwningComp, const float NewHealth, const float OldHealth)
{
	const float Delta = NewHealth - OldHealth;
	AActor* Owner = OwningComp->GetOwner();
	
	if (!InstigatorActor || InstigatorActor == Owner || !FMath::IsNegativeOrNegativeZero(Delta)) return;  // We ignore healing or if no attacker.
	
	// Reflect damage back to attacker (Instigator) as a fraction.
	const int FinalDamage = FMath::RoundToInt(-DamagePercentageToApplyToAttacker * Delta);
	if (FinalDamage == 0) return;
	
	URogueGameplayFunctionLibrary::ApplyDamage(Owner, InstigatorActor, FinalDamage);
}
