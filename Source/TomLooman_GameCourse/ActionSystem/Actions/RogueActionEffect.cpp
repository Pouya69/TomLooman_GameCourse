// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueActionEffect.h"

#include "ActionSystem/RogueActionComponent.h"

void URogueActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	
}

URogueActionEffect::URogueActionEffect()
{
	bAutoStart = true;
	Duration = 3.0f;
	Period = 1.0f;
}

void URogueActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	
	UWorld* World = GetWorld();
	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);
		World->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}
	
	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);
		World->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void URogueActionEffect::StopAction_Implementation(AActor* Instigator)
{
	const UWorld* World = GetWorld();
	
	if (World->GetTimerManager().GetTimerRemaining(DurationHandle) < KINDA_SMALL_NUMBER)
	{
		// Happens when we have a period of 1 second and duration of 3. At the 3 mark that we hit this function, we want to still trigger it.
		// We are running it before Super to make sure the tags are still in tact and are not removed etc.
		ExecutePeriodicEffect(Instigator);
	}
	
	Super::StopAction_Implementation(Instigator);
	
	World->GetTimerManager().ClearTimer(DurationHandle);
	World->GetTimerManager().ClearTimer(PeriodHandle);
	
	if (const auto OwningComp = GetOwningComponent())
	{
		OwningComp->RemoveAction(this);
	}
	
	// No need to destroy. Will be garbage collected when no one is using it.
	
}
