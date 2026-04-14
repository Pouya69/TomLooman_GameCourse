// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueCoin.h"

#include "Player/RoguePlayerCharacter.h"
#include "Player/RoguePlayerState.h"


ARogueCoin::ARogueCoin()
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	MeshComponent->SetGenerateOverlapEvents(true);
	
	Amount = 1;
}

void ARogueCoin::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ARogueCoin::OnPlayerOverlapWithCoin);
}

void ARogueCoin::OnPlayerOverlapWithCoin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PowerUp(Cast<ACharacter>(OtherActor));
}

void ARogueCoin::PowerUp(ACharacter* CharacterInteracting)
{
	if (ARoguePlayerCharacter* PlayerCharacter = Cast<ARoguePlayerCharacter>(CharacterInteracting))
	{
		PlayerCharacter->GetPlayerState<ARoguePlayerState>()->AddCredits(Amount);
		
		Super::PowerUp(CharacterInteracting);
		Destroy();
	}
}
