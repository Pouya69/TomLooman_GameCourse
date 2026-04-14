// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueItemChest.h"


ARogueItemChest::ARogueItemChest()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BaseMeshComp");
	SetRootComponent(BaseMeshComponent);
	BaseMeshComponent->SetCollisionProfileName(TEXT("Interactable"));
	
	LidMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("LidMeshComp");
	LidMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	LidMeshComponent->SetupAttachment(BaseMeshComponent);
	
	
	
	CurrentAnimationPitch = 0.0f;
	AnimationTargetPitch = 120.f;
	AnimationSpeed = 50.f;
}

void ARogueItemChest::Interact_Implementation(AActor* CharacterInteracting)
{
	// Play Animation
	SetActorTickEnabled(true);
}

void ARogueItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CurrentAnimationPitch = FMath::FInterpConstantTo(CurrentAnimationPitch, AnimationTargetPitch,
		DeltaTime, AnimationSpeed);
	LidMeshComponent->SetRelativeRotation(FRotator(CurrentAnimationPitch, 0, 0));
	
	if (FMath::IsNearlyEqual(CurrentAnimationPitch, AnimationTargetPitch))
	{
		SetActorTickEnabled(false);
		
		// Implemented in BP purely.
		ChestAnimationComplete();
	}
}

