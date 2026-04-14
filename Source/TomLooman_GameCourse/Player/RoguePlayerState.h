// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RoguePlayerState.generated.h"

/**
 * 
 */


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ARoguePlayerCharacter*, PlayerCharacterRef, const int, DeltaCredits, const int, NewTotalCredits);

UCLASS()
class TOMLOOMAN_GAMECOURSE_API ARoguePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	// Returns the amount after addition.
	UFUNCTION(BlueprintCallable, Category="Credits")
		int AddCredits(const int AmountToAdd);
	
	// Avoid using unless for some manual work.
	UFUNCTION(BlueprintCallable, Category="Credits")
		void SetCredits(const int InNewCredits);
	
	// For spending credits. If CreditsUsed > Credits, it returns false.
	UFUNCTION(BlueprintCallable, Category="Credits")
		bool UseUpCredits(const int CreditsUsed);
	
	UFUNCTION(BlueprintCallable, Category="Credits")
		int GetCurrentCredits() const;
			
	UPROPERTY(BlueprintAssignable, Category="Credits")
		FOnCreditsChanged OnCreditsChanged;
	
protected:
	// Current Credits.
	UPROPERTY(BlueprintReadOnly, Category="Credits")
		int Credits;
};
