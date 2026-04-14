// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetourCrowdAIController.h"
// #include "Runtime/AIModule/Classes/AIController.h"
#include "RogueAIController.generated.h"

class UAISenseConfig_Sight;
class UAISense;
struct FAIStimulus;
class UBehaviorTree;
class UAIPerceptionComponent;

UCLASS()
class TOMLOOMAN_GAMECOURSE_API ARogueAIController : public ADetourCrowdAIController
{
	GENERATED_BODY()

public:
	ARogueAIController();

	UFUNCTION(BlueprintCallable, Category="AI")
		void SetTargetActor(AActor* Actor);
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="AI")
		TObjectPtr<UBehaviorTree> DefaultBehaviour;
		
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName TargetActorName;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName MoveToLocationName;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI | Perception")
		TObjectPtr<UAISenseConfig_Sight> SightConfig;
	
	UFUNCTION()
		void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
};