// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RBTTask_RangedAttack.generated.h"

class ARogueProjectileBase;
/**
 * 
 */
UCLASS()
class TOMLOOMAN_GAMECOURSE_API URBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	URBTTask_RangedAttack();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	
	UPROPERTY(EditAnywhere, Category="AI")
		FBlackboardKeySelector TargetActorKey;
};
