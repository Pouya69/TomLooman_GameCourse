// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueSprintAction.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

URogueSprintAction::URogueSprintAction()
{
	ActionName = "Sprint";
}

void URogueSprintAction::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	const auto CharacterMovementComp = Cast<ACharacter>(Instigator)->GetCharacterMovement();
	if (!CharacterMovementComp) return;
	
	CharacterMovementComp->MaxWalkSpeed += SpeedAddition;
}

void URogueSprintAction::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	const auto CharacterMovementComp = Cast<ACharacter>(Instigator)->GetCharacterMovement();
	if (!CharacterMovementComp) return;
	
	CharacterMovementComp->MaxWalkSpeed -= SpeedAddition;
}
