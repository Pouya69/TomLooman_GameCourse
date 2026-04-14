// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RogueActionSystemComponent.generated.h"


USTRUCT(BlueprintType)
struct FRogueAttributeSet
{
	GENERATED_BODY()
	
	FRogueAttributeSet()
		: Health(100.f), MaxHealth(100.f), SmallDamageThreshold(30.f) {}
	
	FRogueAttributeSet(const float InHealth, const float InMaxHealth, const float InSmallDamageThreshold)
		: Health(InHealth), MaxHealth(InMaxHealth), SmallDamageThreshold(InSmallDamageThreshold) {}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float Health;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float MaxHealth;
		
	// If Damage > SmallDamageThreshold, it is considered Heavy Damage.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float SmallDamageThreshold;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, URogueActionSystemComponent*, OwningComp, const float, NewHealth, const float, OldHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShowDamagePopup, const float, Damage);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class TOMLOOMAN_GAMECOURSE_API URogueActionSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URogueActionSystemComponent();
	
	// Only use it for custom initialization.
	UFUNCTION(BlueprintCallable, Category="Attributes")
		void InitializeAttributes(FRogueAttributeSet InAttributes);
	
	UFUNCTION(BlueprintCallable, Category="Attributes")
		bool Kill(AActor* InstigatorActor);
	
	UFUNCTION(BlueprintCallable, Category="Attributes")
		bool ApplyHealthChange(AActor* Instigator, float InValueChange);
	
	UFUNCTION(BlueprintCallable, Category="Attributes")
		bool IsAlive() const;
	
	UFUNCTION(BlueprintCallable, Category="Attributes")
		float GetHealth() const;
	
	UFUNCTION(BlueprintCallable, Category="Attributes")
		bool CanUseHealthPotion() const;
	
	// If Damage > SmallDamageThreshold, it is considered Heavy Damage.
	UFUNCTION(BlueprintCallable, Category="Attributes")
		bool IsHeavyDamage(const float Damage) const;
	
	UPROPERTY(BlueprintAssignable)
		FOnHealthChanged OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
		FOnShowDamagePopup OnShowDamagePopup;
	
	UFUNCTION(BlueprintCallable, Category="Attributes")
		const FRogueAttributeSet& GetAttributes() const;
	
	// Static Version for any actor that implements RogueActionSystemInterface
	UFUNCTION(BlueprintCallable, Category="Attributes")
		static URogueActionSystemComponent* GetActionSystemComponent(AActor* Actor);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
		FRogueAttributeSet Attributes;
};
