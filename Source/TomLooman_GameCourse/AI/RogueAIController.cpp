// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "Player/RoguePlayerCharacter.h"


TAutoConsoleVariable<bool> CVar_PlayerSpottedDebugDraw = TAutoConsoleVariable<bool>(TEXT("game.AI.PlayerSpottedDebugDraw"),
                                                                                    false, TEXT("Draw a debug string for when the player was spotted."), ECVF_Cheat);

ARogueAIController::ARogueAIController()
{
	// PrimaryActorTick.bCanEverTick = true;
	
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 2000.f;
	SightConfig->LoseSightRadius = 3500.f;
	SightConfig->PeripheralVisionAngleDegrees = 65.f;
	SightConfig->PointOfViewBackwardOffset = 80.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
	
	MoveToLocationName = "MoveToLocation";
	TargetActorName = "TargetActor";
}

void ARogueAIController::SetTargetActor(AActor* Actor)
{
	GetBlackboardComponent()->SetValueAsObject(TargetActorName, Actor);
}

void ARogueAIController::BeginPlay()
{
	Super::BeginPlay();
	
	if (ensureMsgf(DefaultBehaviour, TEXT("Default Behaviour is not set! Set it in the RogueAIController Blueprint that you have.")))
	{
		RunBehaviorTree(DefaultBehaviour);
		PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ARogueAIController::OnTargetPerceptionUpdated);
	}
	
	/*APawn* PlayerRef = UGameplayStatics::GetPlayerPawn(this, 0);
	
	if (PlayerRef)
	{
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", PlayerRef->GetActorLocation());
		GetBlackboardComponent()->SetValueAsObject("TargetActor", PlayerRef);
	}*/
}

void ARogueAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.Type == UAISense::GetSenseID(UAISense_Sight::StaticClass()))
	{
		ARoguePlayerCharacter* PawnSensed = Cast<ARoguePlayerCharacter>(Actor);
		if (PawnSensed == nullptr)
		{
			// Not the player.
			return;
		}
		if (Stimulus.WasSuccessfullySensed())
		{
			if (PawnSensed == GetBlackboardComponent()->GetValueAsObject(TargetActorName))
			{
				// Same thing we ignore.
				return;
			}
			// Sensed
			const FVector PlayerLocation = PawnSensed->GetActorLocation();
			GetBlackboardComponent()->SetValueAsVector(MoveToLocationName, PlayerLocation);
			SetTargetActor(PawnSensed);
			
			if (CVar_PlayerSpottedDebugDraw.GetValueOnGameThread())
			{
				DrawDebugString(GetWorld(), PlayerLocation, TEXT("Player Detected"), nullptr, FColor::White, 4.f, true);
			}
			
			OnPlayerSpotted.Broadcast(this, PawnSensed);
		}
		
		else
		{
			// Forgotten
			GetBlackboardComponent()->SetValueAsObject(TargetActorName, nullptr);
		}
	}
}
