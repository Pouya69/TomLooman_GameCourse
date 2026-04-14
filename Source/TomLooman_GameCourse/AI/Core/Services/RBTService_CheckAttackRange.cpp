// Fill out your copyright notice in the Description page of Project Settings.


#include "RBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"


URBTService_CheckAttackRange::URBTService_CheckAttackRange()
{
	NodeName = TEXT("Check Attack Range");
}

void URBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	// Check distance between AI Pawn and Target Actor amd LOS
	
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
		if (!TargetActor)
		{
			// Player is not detected.
			BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, false);
			return;
		}
		AAIController* AIController = OwnerComp.GetAIOwner();
		if (!ensure(AIController)) return;
		APawn* AIPawn = AIController->GetPawn();
		check(AIPawn);
		
		const float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
		const bool bWithinRange = DistanceTo < 2000.f;
		
		bool bHasLOS = false;
		if (bWithinRange)
			bHasLOS = AIController->LineOfSightTo(TargetActor);
		
		BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithinRange && bHasLOS);
	}
}
