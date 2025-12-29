// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueProjectileMagic.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
ARogueProjectileMagic::ARogueProjectileMagic()
{
	//PrimaryActorTick.bCanEverTick = true;
}

void ARogueProjectileMagic::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

void ARogueProjectileMagic::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	
	const FVector HitDirection = GetActorRotation().Vector();
	UGameplayStatics::ApplyPointDamage(OtherActor, 10.f, HitDirection, Hit,
		GetInstigatorController(), this, DmgTypeClass);
	
	// We moved Super:: to here because of Destroy()
	Super::OnActorHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}
