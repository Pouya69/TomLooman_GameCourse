// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueItemChest.h"
#include "Net/UnrealNetwork.h"


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
	OpenedPitch = 120.0f;
	AnimationSpeed = 50.f;
	
	bIsLidOpen = false;
	
	bReplicates=true;
}

void ARogueItemChest::OnRep_PlayChestAnimation()
{
	// Play Animation
	UE_LOG(LogTemp, Warning, TEXT("Opened Chest"));
	AnimationTargetPitch = bIsLidOpen ? OpenedPitch : 0.0f;
	
	SetActorTickEnabled(true);
}

void ARogueItemChest::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ARogueItemChest, bIsLidOpen);
}

void ARogueItemChest::Interact_Implementation(AActor* CharacterInteracting)
{
	bIsLidOpen = !bIsLidOpen;
	
	OnRep_PlayChestAnimation();
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
