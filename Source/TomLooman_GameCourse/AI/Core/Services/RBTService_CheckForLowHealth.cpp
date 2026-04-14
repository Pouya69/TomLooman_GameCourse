// Fill out your copyright notice in the Description page of Project Settings.


#include "RBTService_CheckForLowHealth.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "ActionSystem/RogueActionSystemComponent.h"

URBTService_CheckForLowHealth::URBTService_CheckForLowHealth()
{
	NodeName = TEXT("Check For Low Health");
	
	Interval = 1.1f;
	
	LowHealthThreshold = 40.f;
}

void URBTService_CheckForLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	// Check for low health so character can heal.
	
	AAIController* Controller = OwnerComp.GetAIOwner();
	
	const float MyHealth = URogueActionSystemComponent::GetActionSystemComponent(Controller->GetPawn())->GetHealth();
	const bool bIsOnLowHealth = MyHealth < LowHealthThreshold;
	
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsOnLowHealthKey.SelectedKeyName, bIsOnLowHealth);
}
