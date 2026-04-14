// Fill out your copyright notice in the Description page of Project Settings.


#include "RBTTask_RangedAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/RogueAICharacter.h"


URBTTask_RangedAttack::URBTTask_RangedAttack()
{
	NodeName = TEXT("Ranged Attack");
}

EBTNodeResult::Type URBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (ensure(AIController))
	{
		ARogueAICharacter* Character = Cast<ARogueAICharacter>(AIController->GetCharacter());
		if (!Character) return EBTNodeResult::Failed;
	
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName));
		if (!TargetActor) return EBTNodeResult::Failed;
	
		const bool bDidShoot = Character->Shoot(TargetActor);
	
		return bDidShoot ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Failed;
}
