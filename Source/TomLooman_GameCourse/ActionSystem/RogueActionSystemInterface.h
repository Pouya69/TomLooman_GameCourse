// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RogueActionSystemInterface.generated.h"

class URogueActionComponent;
class URogueActionSystemComponent;
// This class does not need to be modified.
UINTERFACE()
class URogueActionSystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TOMLOOMAN_GAMECOURSE_API IRogueActionSystemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Action System Component")
		bool GetActionSystemComponent(URogueActionSystemComponent*& OutActionSystemComponent);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Action System Component")
		bool GetActionComponent(URogueActionComponent*& OutActionComponent);
	
	
};
