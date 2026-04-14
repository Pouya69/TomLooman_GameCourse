// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RBTService_CheckForLowHealth.generated.h"

/**
 * 
 */
UCLASS()
class TOMLOOMAN_GAMECOURSE_API URBTService_CheckForLowHealth : public UBTService
{
	GENERATED_BODY()
	
public:
	URBTService_CheckForLowHealth();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, Category="AI")
		FBlackboardKeySelector IsOnLowHealthKey;
	
	// If Health is smaller than this value, it sets IsOnLowHealthKey to true.
	UPROPERTY(EditAnywhere, Category="AI")
		float LowHealthThreshold;
};
