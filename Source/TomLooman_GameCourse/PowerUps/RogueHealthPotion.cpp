// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueHealthPotion.h"

#include "ActionSystem/RogueActionSystemComponent.h"
#include "Player/RoguePlayerCharacter.h"
#include "Player/RoguePlayerState.h"


ARogueHealthPotion::ARogueHealthPotion()
{
	CooldownTime = 10.f;
	
	CreditsNeededToUse = 3;
}

void ARogueHealthPotion::StartCooldown()
{
	Super::StartCooldown();
	Mesh_MID->SetScalarParameterValue("InteractTime", GetWorld()->GetTimeSeconds());
}

void ARogueHealthPotion::PowerUp(ACharacter* CharacterInteracting)
{
	ARoguePlayerCharacter* PlayerCharacter = CastChecked<ARoguePlayerCharacter>(CharacterInteracting);
	URogueActionSystemComponent* ActionSystemComponent = URogueActionSystemComponent::GetActionSystemComponent(PlayerCharacter);
	if (!ActionSystemComponent->CanUseHealthPotion())
	{
		IRogueInteractionInterface::Execute_CannotInteract(this);
		return;
	}
	ActionSystemComponent->ApplyHealthChange(nullptr, Amount);
	
	Super::PowerUp(CharacterInteracting);
}

void ARogueHealthPotion::OnCooldown_Implementation()
{
	Super::OnCooldown_Implementation();
}

void ARogueHealthPotion::CannotInteract_Implementation()
{
	Super::CannotInteract_Implementation();
	Mesh_MID->SetScalarParameterValue("HitTime", GetWorld()->GetTimeSeconds());
}

void ARogueHealthPotion::Interact_Implementation(AActor* CharacterInteracting)
{
	if (IsOnCooldown())
	{
		IRogueInteractionInterface::Execute_OnCooldown(this);
		return;
	}
	
	ARoguePlayerCharacter* PlayerCharacter = CastChecked<ARoguePlayerCharacter>(CharacterInteracting);
	
	const bool bCanPlayerAffordThis = PlayerCharacter->GetPlayerState<ARoguePlayerState>()->UseUpCredits(CreditsNeededToUse);
	
	if (!bCanPlayerAffordThis)
	{
		// Player cannot afford this.
		IRogueInteractionInterface::Execute_CannotInteract(this);
		return;
	}
	
	PowerUp(PlayerCharacter);
}
