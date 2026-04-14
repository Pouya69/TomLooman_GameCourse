// Fill out your copyright notice in the Description page of Project Settings.


#include "RBTTask_Heal.h"

#include "AIController.h"
#include "AI/RogueAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

URBTTask_Heal::URBTTask_Heal()
{
	NodeName = TEXT("Heal");
	
	HealAmount = 30.f;
}

EBTNodeResult::Type URBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		ARogueAICharacter* MyCharacter = Cast<ARogueAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
		if (!MyCharacter)
			return EBTNodeResult::Failed;
		
		const bool bHealed = MyCharacter->Heal(HealAmount);
		
		if (!bHealed)
		{
			// Already at desired health.
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsOnLowHealthKey.SelectedKeyName, false);
			return EBTNodeResult::Failed;
		}
		
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
