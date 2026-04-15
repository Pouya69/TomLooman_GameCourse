// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePlayerState.h"
#include "RoguePlayerCharacter.h"

int ARoguePlayerState::AddCredits(const int AmountToAdd)
{
	SetCredits(Credits + AmountToAdd);
	
	return Credits;
}

void ARoguePlayerState::SetCredits(const int InNewCredits)
{
	Credits = InNewCredits;
	
	OnCreditsChanged.Broadcast(Cast<ARoguePlayerCharacter>(GetPawn()), InNewCredits, Credits);
}

bool ARoguePlayerState::UseUpCredits(const int CreditsUsed)
{
	if (Credits < CreditsUsed) return false;
	
	Credits -= CreditsUsed;
	
	OnCreditsChanged.Broadcast(Cast<ARoguePlayerCharacter>(GetPawn()), CreditsUsed, Credits);
	
	return true;
}

int ARoguePlayerState::GetCurrentCredits() const
{
	return Credits;
}
