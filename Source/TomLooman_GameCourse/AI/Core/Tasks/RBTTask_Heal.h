// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RBTTask_Heal.generated.h"

/**
 * 
 */
UCLASS()
class TOMLOOMAN_GAMECOURSE_API URBTTask_Heal : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	URBTTask_Heal();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere, Category="AI")
		float HealAmount;
	
	UPROPERTY(EditAnywhere, Category="AI")
		FBlackboardKeySelector IsOnLowHealthKey;
};
