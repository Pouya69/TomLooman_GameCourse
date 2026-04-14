// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoguePowerupBase.h"
#include "RogueCoin.generated.h"

UCLASS()
class TOMLOOMAN_GAMECOURSE_API ARogueCoin : public ARoguePowerupBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARogueCoin();

protected:
	virtual void PostInitializeComponents() override;
	
	UFUNCTION()
		void OnPlayerOverlapWithCoin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	virtual void PowerUp(ACharacter* CharacterInteracting) override;
};
