// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/RogueInteractionInterface.h"
// #include "TomLooman_GameCourse/Core/RogueInteractionInterface.h"
#include "RogueItemChest.generated.h"

UCLASS()
class TOMLOOMAN_GAMECOURSE_API ARogueItemChest : public AActor, public IRogueInteractionInterface
{
	GENERATED_BODY()

public:
	ARogueItemChest();

protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
		TObjectPtr<UStaticMeshComponent> BaseMeshComponent;
	UPROPERTY(VisibleAnywhere, Category="Components")
		TObjectPtr<UStaticMeshComponent> LidMeshComponent;
	
	UPROPERTY(VisibleAnywhere, Category="Animation")
		float CurrentAnimationPitch;
	
	UPROPERTY(EditAnywhere, Category="Animation")
		float AnimationTargetPitch;
	
	UPROPERTY(EditAnywhere, Category="Animation")
		float AnimationSpeed;
	
	UFUNCTION(BlueprintImplementableEvent)
		void ChestAnimationComplete();

public:
	// The Interact() function is only driven in BP. This is called as well.
	// In BP, we need to right click on Interact event
	// Then we press 'Add Call to Parent Function'
	// It will call this.
	virtual void Interact_Implementation() override;
	
	virtual void Tick(float DeltaTime) override;
};
