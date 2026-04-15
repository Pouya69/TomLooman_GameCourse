

#include "RoguePowerup_Action.h"

#include "ActionSystem/RogueActionComponent.h"
#include "GameFramework/Character.h"


ARoguePowerup_Action::ARoguePowerup_Action()
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	MeshComponent->SetGenerateOverlapEvents(true);
	MeshComponent->SetCollisionProfileName("PowerupOnCollision");
}

void ARoguePowerup_Action::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ARoguePowerup_Action::OnPlayerOverlapWithThis);
}

void ARoguePowerup_Action::OnPlayerOverlapWithThis(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PowerUp(Cast<ACharacter>(OtherActor));
}

void ARoguePowerup_Action::PowerUp(ACharacter* CharacterInteracting)
{
	auto ActionComp = URogueActionComponent::GetActionComponent(CharacterInteracting);
	if (!ActionComp) return;
	
	if (!ActionComp->GetAction(ActionToGrant))
		ActionComp->AddAction(this, ActionToGrant);
	
	Super::PowerUp(CharacterInteracting);
	
	Destroy();
}