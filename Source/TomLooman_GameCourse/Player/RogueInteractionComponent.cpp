// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueInteractionComponent.h"

#include "RogueGameTypes.h"
#include "Blueprint/UserWidget.h"
#include "Engine/OverlapResult.h"
#include "Core/RogueInteractionInterface.h"
#include "UI/Core/RogueWorldUserWidget.h"
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
	
	CollisionChannel = COLLISION_INTERACTION;
}

void URogueInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	// This is how we call Interact on interfaces for BP implementation. No need for casting.
	// In BP, we right click on even and do 'Call Parent ...'
	// This would call Interact_Implementation() first and then what we have.
	if (InFocus)
		IRogueInteractionInterface::Execute_Interact(InFocus, CastChecked<APlayerController>(GetOwner())->GetPawn());
}


void URogueInteractionComponent::Interact()
{
	ServerInteract(SelectedActor);
}

void URogueInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// TODO: Do not put this in tick for performance reasons.
	const APawn* Pawn = Cast<APlayerController>(GetOwner())->GetPawn();
	if (Pawn->IsLocallyControlled())
		FindBestInteractable();
}

void URogueInteractionComponent::FindBestInteractable()
{
	// TODO: Do not put this in tick for performance reasons.
	
		// CastChecked is good for assertion
	const APlayerController* PC = CastChecked<APlayerController>(GetOwner());
	const FVector Center = PC->GetPawn()->GetActorLocation();
	
	// DrawDebugBox(GetWorld(), Center, FVector(20.f), FColor::Red, false);
	TArray<FOverlapResult> Overlaps;
	// We create a new file.h in Source/PROJECT_NAME
	// There we define our global stuff.
	// COLLISION_INTERACTION = ECC_GameTraceChannel1
	const FCollisionShape CollisionShape = FCollisionShape::MakeSphere(InteractionRadius);
	
	const bool bIsInteractionDebugEnabled = CVarInteractionDebugDrawing.GetValueOnGameThread();
	GetWorld()->OverlapMultiByChannel(Overlaps, Center, FQuat::Identity, CollisionChannel, CollisionShape);
	if (bIsInteractionDebugEnabled)
	{
		DrawDebugSphere(GetWorld(), Center, InteractionRadius, 32.f, FColor::White);
	}
	
	AActor* BestActor = nullptr;
	float HighestDotResult = -1.f;
	const FVector PlayerLookingDirection = PC->GetControlRotation().Vector();
	for (FOverlapResult& OverlapResult : Overlaps)
	{
		const FVector OverlapLocation = OverlapResult.GetActor()->GetActorLocation();
		const FVector OverlapDirection = (OverlapLocation - Center).GetSafeNormal();
		
		const float DotResult = FVector::DotProduct(PlayerLookingDirection,OverlapDirection);
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
	
	if (BestActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass != nullptr))
		{
			DefaultWidgetInstance = CreateWidget<URogueWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}
		
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachTo = BestActor;
			
			if (!DefaultWidgetInstance->IsInViewport())
				DefaultWidgetInstance->AddToViewport();
		}
	}
	else
	{
		if (DefaultWidgetInstance && DefaultWidgetInstance->IsInViewport())
			DefaultWidgetInstance->RemoveFromParent();
	}
	/*
	if (SelectedActor != BestActor)
	{
		if (SelectedActor != nullptr)
			IRogueInteractionInterface::Execute_OutOfFocus(SelectedActor);
	
		if (BestActor != nullptr)
			IRogueInteractionInterface::Execute_InFocus(BestActor);
	}
	*/
	
	SelectedActor = BestActor;
	if (bIsInteractionDebugEnabled)
	{
		if (BestActor != nullptr)
		{
			DrawDebugBox(GetWorld(), BestActor->GetActorLocation(), FVector(60.f), FColor::Green, false);
		}
	}
}