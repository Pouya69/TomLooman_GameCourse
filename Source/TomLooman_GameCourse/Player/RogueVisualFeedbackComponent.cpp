// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueVisualFeedbackComponent.h"

#include "Kismet/KismetMaterialLibrary.h"


URogueVisualFeedbackComponent::URogueVisualFeedbackComponent()
{
	VisualFeedbackColorName = "HitFlashColor";
	VisualFeedbackHitTimeName = "HitTime";
	VisualFeedbackHiLastingTimeName = "HitFlashSpeed";
	
	OnPowerUpVisualFeedbackSpeed = 2.f;
	OnTakeLightDamageVisualFeedbackSpeed = 3.5f;
	OnTakeHeavyDamageVisualFeedbackSpeed = 2.f;
	
	OnTakeLightDamageVisualFeedbackColor = FLinearColor(5.000000,3.905052,0.000000,1.000000);
	OnTakeHeavyDamageVisualFeedbackColor = FLinearColor(7.000000,0.529302,0.000000,1.000000);
	OnPowerUpVisualFeedbackColor = FLinearColor(0, 5.f, 0, 1.f);
	
	PrimaryComponentTick.bCanEverTick = false;
}

void URogueVisualFeedbackComponent::InitializeVisualFeedback(UMeshComponent* Mesh)
{
	for (int i = 0; i < Mesh->GetNumMaterials(); i++)
	{
		UMaterialInterface* CharacterMaterial = Mesh->GetMaterial(i);
		UMaterialInstanceDynamic* CreatedMaterialInstanceDynamic = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, CharacterMaterial);
		
		Mesh->SetMaterial(i, CreatedMaterialInstanceDynamic);
		
		MeshMaterials_MID.Add(CreatedMaterialInstanceDynamic);
	}
}

void URogueVisualFeedbackComponent::VisualFeedback(ERogueVisualFeedbackType VisualFeedbackType)
{
	ensure(!MeshMaterials_MID.IsEmpty());
	const float HitTime = GetWorld()->GetTimeSeconds();
	for (UMaterialInstanceDynamic* Character_MID : MeshMaterials_MID)
	{
		Character_MID->SetScalarParameterValue(VisualFeedbackHitTimeName, HitTime);
		switch (VisualFeedbackType)
		{
			case ERogueVisualFeedbackType::HEALED:
				Character_MID->SetScalarParameterValue(VisualFeedbackHiLastingTimeName, OnHealVisualFeedbackSpeed);
				Character_MID->SetVectorParameterValue(VisualFeedbackColorName, OnHealVisualFeedbackColor);
				break;
			case ERogueVisualFeedbackType::POWER_UP:
				Character_MID->SetScalarParameterValue(VisualFeedbackHiLastingTimeName, OnPowerUpVisualFeedbackSpeed);
				Character_MID->SetVectorParameterValue(VisualFeedbackColorName, OnPowerUpVisualFeedbackColor);
				break;
			case ERogueVisualFeedbackType::DAMAGED_LIGHT:
				Character_MID->SetScalarParameterValue(VisualFeedbackHiLastingTimeName, OnTakeLightDamageVisualFeedbackSpeed);
				Character_MID->SetVectorParameterValue(VisualFeedbackColorName, OnTakeLightDamageVisualFeedbackColor);
				break;
			case ERogueVisualFeedbackType::DAMAGED_HEAVY:
				Character_MID->SetScalarParameterValue(VisualFeedbackHiLastingTimeName, OnTakeHeavyDamageVisualFeedbackSpeed);
				Character_MID->SetVectorParameterValue(VisualFeedbackColorName, OnTakeHeavyDamageVisualFeedbackColor);
				break;
			default:
				return;
		}
	}
}
