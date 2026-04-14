// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RBTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class TOMLOOMAN_GAMECOURSE_API URBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()
	
public:
	URBTService_CheckAttackRange();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, Category="AI")
		FBlackboardKeySelector TargetActorKey;
	
	UPROPERTY(EditAnywhere, Category="AI")
		FBlackboardKeySelector AttackRangeKey;
};
