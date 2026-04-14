// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RogueVisualFeedbackComponent.generated.h"

UENUM(BlueprintType)
enum class ERogueVisualFeedbackType : uint8
{
	POWER_UP = 0,
	HEALED,
	DAMAGED_LIGHT,
	DAMAGED_HEAVY,
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOMLOOMAN_GAMECOURSE_API URogueVisualFeedbackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URogueVisualFeedbackComponent();
	
	// Could be used when the character is hit, with colours etc.
	// Make sure the component is initialized.
	// InitializeVisualFeedback(StaticMesh) OR InitializeVisualFeedback(SkeletalMesh)
	UFUNCTION(BlueprintCallable, Category = "Visual Feedback")
		void VisualFeedback(ERogueVisualFeedbackType VisualFeedbackType);
	
	// Mandatory call for initializing the component.
	// InitializeVisualFeedback(StaticMesh) OR InitializeVisualFeedback(SkeletalMesh)
	UFUNCTION(BlueprintCallable, Category = "Visual Feedback")
		void InitializeVisualFeedback(UMeshComponent* Mesh);
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual Feedback")
		TArray<UMaterialInstanceDynamic*> MeshMaterials_MID;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Visual Feedback")
		FName VisualFeedbackColorName;
	
	UPROPERTY(VisibleAnywhere, Category = "Visual Feedback")
		FName VisualFeedbackHitTimeName;
	
	UPROPERTY(VisibleAnywhere, Category = "Visual Feedback")
		FName VisualFeedbackHiLastingTimeName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Visual Feedback | Light Damage")
		FLinearColor OnTakeLightDamageVisualFeedbackColor;
	
	UPROPERTY(EditDefaultsOnly, Category = "Visual Feedback | Light Damage")
		float OnTakeLightDamageVisualFeedbackSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Visual Feedback | Heavy Damage")
		FLinearColor OnTakeHeavyDamageVisualFeedbackColor;
	
	UPROPERTY(EditDefaultsOnly, Category = "Visual Feedback | Heavy Damage")
		float OnTakeHeavyDamageVisualFeedbackSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Visual Feedback | Heal")
		FLinearColor OnHealVisualFeedbackColor;
	
	UPROPERTY(EditDefaultsOnly, Category = "Visual Feedback | Heal")
		float OnHealVisualFeedbackSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Visual Feedback | Power Up")
		FLinearColor OnPowerUpVisualFeedbackColor;
	
	UPROPERTY(EditDefaultsOnly, Category = "Visual Feedback | Power Up")
		float OnPowerUpVisualFeedbackSpeed;
};
