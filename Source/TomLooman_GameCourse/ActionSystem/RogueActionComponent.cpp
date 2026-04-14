#include "RogueActionComponent.h"

#include "RogueActionSystemInterface.h"
#include "Actions/RogueAction.h"


URogueActionComponent::URogueActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URogueActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const auto Action : DefaultActions)
		AddAction(Action);
}

void URogueActionComponent::AddAction(TSubclassOf<URogueAction> ActionClass)
{
	if (!ensure(ActionClass))
		return;
	
	URogueAction* NewAction = NewObject<URogueAction>(this, ActionClass);
	if (!ensure(NewAction))
		return;
	
	Actions.Add(NewAction);
}

bool URogueActionComponent::StartActionByName(AActor* Instigator, const FName ActionName)
{
	for (URogueAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart())
			{
				const FString DebugMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
				continue;
			}
			
			Action->StartAction(Instigator);
			return true;
		}
	}
	
	return false;
}

bool URogueActionComponent::StopActionByName(AActor* Instigator, const FName ActionName)
{
	for (URogueAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName && Action->IsRunning())
		{
			Action->StopAction(Instigator);
			return true;
		}
	}
	
	return false;
}

void URogueActionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	const FString DebugMsg = GetOwner()->GetName() + ": " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, DebugMsg);
}

URogueActionComponent* URogueActionComponent::GetActionComponent(AActor* Actor)
{
	if (!Actor) return nullptr;
	URogueActionComponent* ActionComponent = nullptr;
	if (!Actor->Implements<URogueActionSystemInterface>()) return nullptr;
	
	const bool bWasSuccessful = IRogueActionSystemInterface::Execute_GetActionComponent(Actor, ActionComponent);
	
	return bWasSuccessful ? ActionComponent : nullptr;
}
