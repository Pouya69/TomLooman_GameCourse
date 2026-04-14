// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAction.h"

#include "ActionSystem/RogueActionComponent.h"

void URogueAction::StartAction_Implementation(AActor* Instigator)
{
	GetOwningComponent()->ActiveGameplayTags.AppendTags(GrantsTags);
	bIsRunning = true;
}

void URogueAction::StopAction_Implementation(AActor* Instigator)
{
	ensureAlways(bIsRunning);
	
	GetOwningComponent()->ActiveGameplayTags.RemoveTags(GrantsTags);
	bIsRunning = false;
}

UWorld* URogueAction::GetWorld() const
{
	if (const auto Comp = GetOwningComponent())
		return Comp->GetWorld();
	
	return nullptr;
}

URogueActionComponent* URogueAction::GetOwningComponent() const
{
	return Cast<URogueActionComponent>(GetOuter());
}

bool URogueAction::IsRunning() const
{
	return bIsRunning;
}

bool URogueAction::CanStart_Implementation() const
{
	return !IsRunning() && !GetOwningComponent()->ActiveGameplayTags.HasAny(BlockedTags);
}
