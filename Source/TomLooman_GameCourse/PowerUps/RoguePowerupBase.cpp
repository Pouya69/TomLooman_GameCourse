// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePowerupBase.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Player/RoguePlayerCharacter.h"

ARoguePowerupBase::ARoguePowerupBase()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionProfileName("PowerupInteractable");
	
	Amount = 30.f;
	
	bIsDynamic = true;
}

void ARoguePowerupBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	/*
	switch (PowerUpUsageType)
	{
		default:
			break;
		case EPowerUpUsageType::ON_COLLISION:
			MeshComponent->SetCollisionProfileName("PowerupOnCollision");
			break;
		case EPowerUpUsageType::INTERACT:
			MeshComponent->SetCollisionProfileName("PowerupInteractable");
			break;
	}
	*/
	
	if (GetWorld()->IsGameWorld() && bIsDynamic)
	{
		Mesh_MID = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, MeshComponent->GetMaterial(0));
		Mesh_MID->SetScalarParameterValue("CooldownTime", CooldownTime);
		MeshComponent->SetMaterial(0, Mesh_MID);
	}
	
}

void ARoguePowerupBase::Interact_Implementation(AActor* CharacterInteracting)
{
	// We will make it so it can be interactable or just by collision in child classes.
}

bool ARoguePowerupBase::IsOnCooldown() const
{
	if (CooldownTime <= 0) return false;
	return GetWorldTimerManager().IsTimerActive(CooldownTimer);
}

void ARoguePowerupBase::PowerUp(ACharacter* CharacterInteracting)
{
	UGameplayStatics::PlaySound2D(this, PowerUpSound);
	StartCooldown();
}

void ARoguePowerupBase::OnCooldown_Implementation()
{
	UGameplayStatics::PlaySound2D(this, OnCooldownSound);
}

void ARoguePowerupBase::CannotInteract_Implementation()
{
	UGameplayStatics::PlaySound2D(this, CannotPowerUpSound);
}

void ARoguePowerupBase::StartCooldown()
{
	// Start Cooldown if CooldownTime > 0
	if (CooldownTime <= 0) return;
	GetWorldTimerManager().SetTimer(CooldownTimer, CooldownTime, false);
}
