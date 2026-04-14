// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "RogueActionComponent.generated.h"


class URogueAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOMLOOMAN_GAMECOURSE_API URogueActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URogueActionComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tags")
		FGameplayTagContainer ActiveGameplayTags;
	
	UFUNCTION(BlueprintCallable, Category="Actions")
		void AddAction(AActor* Instigator, const TSubclassOf<URogueAction> ActionClass);
	
	UFUNCTION(BlueprintCallable, Category="Actions")
		bool StartActionByName(AActor* Instigator, const FName ActionName);
	
	UFUNCTION(BlueprintCallable, Category="Actions")
		bool StopActionByName(AActor* Instigator, const FName ActionName);
	
	UFUNCTION(BlueprintCallable, Category="Actions")
		void RemoveAction(URogueAction* ActionToRemove);
	
	UFUNCTION(BlueprintCallable, Category="Actions")
		void RemoveActionByName(const FName ActionName);
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	static URogueActionComponent* GetActionComponent(AActor* Actor);
	
	

protected:
	
	UPROPERTY(BlueprintReadOnly, Category="Actions")
		TArray<URogueAction*> Actions;
	
	UPROPERTY(EditDefaultsOnly, Category="Actions")
		TArray<TSubclassOf<URogueAction>> DefaultActions;
	
	virtual void BeginPlay() override;
};
