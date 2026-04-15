#include "RogueActionSystemComponent.h"

#include "RogueActionSystemInterface.h"
#include "Core/RogueGameMode.h"


static TAutoConsoleVariable<float> CVar_GlobalDamageMultiplier(TEXT("game.global.DamageMultiplier"), 1.0f, TEXT("Multiply the global damage value for ActionSystemComponent."), ECVF_Cheat);



URogueActionSystemComponent::URogueActionSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	Attributes.Health = 100.0f;
	
	Attributes.MaxRage = 100.0f;
	Attributes.RageDamageMultiplier = 3.0f;
}

void URogueActionSystemComponent::ApplyRageChange(const float RageDelta)
{
	const float OldRage = Attributes.Rage;
	Attributes.Rage = FMath::Clamp(Attributes.Rage + RageDelta * Attributes.RageDamageMultiplier, 0.0f, Attributes.MaxRage);
	
	const float ActualDelta = Attributes.Rage - OldRage;
	if (!FMath::IsNearlyZero(ActualDelta))
		OnRageChanged.Broadcast(this, Attributes.Rage, ActualDelta);
}

bool URogueActionSystemComponent::IsRageFull() const
{
	return FMath::IsNearlyEqual(Attributes.Rage, Attributes.MaxRage);
}

void URogueActionSystemComponent::InitializeAttributes(FRogueAttributeSet InAttributes)
{
	const UWorld* World = GetWorld();
	if (World && World->IsGameWorld())
		Attributes = InAttributes;
}

bool URogueActionSystemComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetAttributes().MaxHealth);
}

bool URogueActionSystemComponent::ApplyHealthChange(AActor* Instigator, float InValueChange)
{
	// For healing we let it happen.
	if (!GetOwner()->CanBeDamaged() && InValueChange < 0.0f)
	{
		// For God Mode and etc.
		return false;
	}
	
	// Global Dmg Multiplier
	if (InValueChange < 0.0f)
		InValueChange *= CVar_GlobalDamageMultiplier.GetValueOnGameThread();
	
	const float OldHealth = Attributes.Health;
	Attributes.Health = FMath::Clamp(Attributes.Health + InValueChange, 0.f, Attributes.MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("New Health: %f, Max Health: %f"), Attributes.Health, Attributes.MaxHealth);
	
	const bool bIsChanged = !FMath::IsNearlyEqual(OldHealth, Attributes.Health);
	const float Delta = Attributes.Health - OldHealth;
	OnShowDamagePopup.Broadcast(bIsChanged ? Delta : 0.f);
	if (bIsChanged)
	{
		OnHealthChanged.Broadcast(Instigator, this,Attributes.Health, OldHealth);
		if (Delta < 0.0f && Attributes.Health == 0.0f)
		{
			// Died
			if (ARogueGameMode* GameMode = GetWorld()->GetAuthGameMode<ARogueGameMode>())
			{
				GameMode->OnActorKilled(GetOwner(), Instigator);
			}
		}
	}
	else
		return false;
	return true;
}

bool URogueActionSystemComponent::IsAlive() const
{
	return Attributes.Health > 0.0f;
}

float URogueActionSystemComponent::GetHealth() const
{
	return Attributes.Health;
}

bool URogueActionSystemComponent::CanUseHealthPotion() const
{
	return !FMath::IsNearlyEqual(Attributes.Health, Attributes.MaxHealth) && IsAlive();
}

bool URogueActionSystemComponent::IsHeavyDamage(const float Damage) const
{
	return Damage > Attributes.SmallDamageThreshold;
}

const FRogueAttributeSet& URogueActionSystemComponent::GetAttributes() const
{
	return Attributes;
}

URogueActionSystemComponent* URogueActionSystemComponent::GetActionSystemComponent(AActor* Actor)
{
	if (!Actor) return nullptr;
	URogueActionSystemComponent* ActionSystemComponent = nullptr;
	if (!Actor->Implements<URogueActionSystemInterface>()) return nullptr;
	
	const bool bWasSuccessful = IRogueActionSystemInterface::Execute_GetActionSystemComponent(Actor, ActionSystemComponent);
	
	return bWasSuccessful ? ActionSystemComponent : nullptr;
}
