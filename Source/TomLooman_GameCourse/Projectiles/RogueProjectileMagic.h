// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RogueProjectileBase.h"
#include "RogueProjectileMagic.generated.h"


UCLASS(Abstract)
class TOMLOOMAN_GAMECOURSE_API ARogueProjectileMagic : public ARogueProjectileBase
{
	GENERATED_BODY()
public:
	ARogueProjectileMagic();
	
	virtual void PostInitializeComponents() override;

protected:
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
