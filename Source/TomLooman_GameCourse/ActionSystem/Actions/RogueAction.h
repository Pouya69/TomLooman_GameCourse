// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "UObject/Object.h"
#include "RogueAction.generated.h"


class URogueActionComponent;

UCLASS(Blueprintable, Abstract)
class TOMLOOMAN_GAMECOURSE_API URogueAction : public UObject
{
	GENERATED_BODY()
	
protected:
	/* Upon ActionStart, this action will grant these tags to the OwningActor. Removed when the action stops. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tags")
		FGameplayTagContainer GrantsTags;
	
	/* If the OwningActor has any of these tags, the action won't start. (AKA: It will only work if these tags do not exist in the ActiveGameplayTags */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tags")
		FGameplayTagContainer BlockedTags;
	
	
	bool bIsRunning;
	
public:
	// Starts immediately after being added to Action Component.
	UPROPERTY(EditDefaultsOnly, Category="Action")
		bool bAutoStart;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Action")
		URogueActionComponent* GetOwningComponent() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Action")
	URogueActionSystemComponent* GetActionSystemComponent() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Action")
		void StartAction(AActor* Instigator);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Action")
		void StopAction(AActor* Instigator);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, BlueprintCallable, Category="Action")
		bool CanStart() const;
	
	UFUNCTION(BlueprintPure, BlueprintCallable, Category="Action")
		bool IsRunning() const;
	
	virtual class UWorld* GetWorld() const override;
	
	UPROPERTY(EditDefaultsOnly, Category="Action")
		FName ActionName;
};
