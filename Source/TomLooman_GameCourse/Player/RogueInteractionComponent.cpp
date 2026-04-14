// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueInteractionComponent.h"

#include "RogueGameTypes.h"
#include "Engine/OverlapResult.h"
#include "Core/RogueInteractionInterface.h"
// #include "TomLooman_GameCourse/Core/RogueInteractionInterface.h"

// ECVF_Cheat is only for devs not shipping. Won't be available in shipping.
// You are creating your own custom CVars.
TAutoConsoleVariable<bool> CVarInteractionDebugDrawing(TEXT("game.interaction.DebugDraw"),
	false,
	TEXT("Enable debug drawing of interaction elements."), ECVF_Cheat);


URogueInteractionComponent::URogueInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	InteractionRadius = 800.f;
}

void URogueInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URogueInteractionComponent::Interact()
{
	// This is how we call Interact on interfaces for BP implementation. No need for casting.
	// In BP, we right click on even and do 'Call Parent ...'
	// This would call Interact_Implementation() first and then what we have.
	if (SelectedActor)
		IRogueInteractionInterface::Execute_Interact(SelectedActor, CastChecked<APlayerController>(GetOwner())->GetPawn());
}

void URogueInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// CastChecked is good for assertion
	const APlayerController* PC = CastChecked<APlayerController>(GetOwner());
	const FVector Center = PC->GetPawn()->GetActorLocation();
	
	// DrawDebugBox(GetWorld(), Center, FVector(20.f), FColor::Red, false);
	TArray<FOverlapResult> Overlaps;
	// We create a new file.h in Source/PROJECT_NAME
	// There we define our global stuff.
	// COLLISION_INTERACTION = ECC_GameTraceChannel1
	const ECollisionChannel CollisionChannel = COLLISION_INTERACTION;
	const FCollisionShape CollisionShape = FCollisionShape::MakeSphere(InteractionRadius);
	
	const bool bIsInteractionDebugEnabled = CVarInteractionDebugDrawing.GetValueOnGameThread();
	GetWorld()->OverlapMultiByChannel(Overlaps, Center, FQuat::Identity, CollisionChannel, CollisionShape);
	if (bIsInteractionDebugEnabled)
	{
		DrawDebugSphere(GetWorld(), Center, InteractionRadius, 32.f, FColor::White);
	}
	
	
	// Closer to camera's looking direction
	// We are using this method to find the best interactable based on the closest camera angle
	// We get dot product of character to overlap AND the ControlRotation
	// Dot product: -1 means opposite direction from camera
	// 1: Exactly same as camera direction
	// 0: left/right (perpendicular)
	
	AActor* BestActor = nullptr;
	float HighestDotResult = -1.f;
	for (FOverlapResult& OverlapResult : Overlaps)
	{
		const FVector OverlapLocation = OverlapResult.GetActor()->GetActorLocation();
		const FVector OverlapDirection = (OverlapLocation - Center).GetSafeNormal();
		
		const float DotResult = FVector::DotProduct(PC->GetControlRotation().Vector(),OverlapDirection);
		if (DotResult > HighestDotResult)
		{
			BestActor = OverlapResult.GetActor();
			HighestDotResult = DotResult;
		}
		if (bIsInteractionDebugEnabled)
		{
			FString DebugString = FString::Printf(TEXT("Dot: %f"), DotResult);
			DrawDebugString(GetWorld(), OverlapLocation, DebugString, nullptr, FColor::White, 0.f, true);
			DrawDebugBox(GetWorld(), OverlapLocation, FVector(50.f), FColor::Red, false);
		}
	}
	
	SelectedActor = BestActor;
	if (bIsInteractionDebugEnabled)
	{
		if (BestActor != nullptr)
		{
			DrawDebugBox(GetWorld(), BestActor->GetActorLocation(), FVector(60.f), FColor::Green, false);
		}
	}
	
}
